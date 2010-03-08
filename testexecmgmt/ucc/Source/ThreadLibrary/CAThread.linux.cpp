/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
* System Includes
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CAThread.h" 

/*******************************************************************************
 *
 * Implementation
 *
 ******************************************************************************/
CAThread::CAThread()
{
  iThreadHandle = 0;
  iThreadState = TS_INIT;
  iProc = NULL;
}


CAThread::CAThread( string aThreadName )
{
  iThreadHandle = 0;
  iThreadState = TS_INIT;
  iProc = NULL;
  iThreadName = aThreadName;
}


CAThread::~CAThread()
{
  assert( iThreadHandle == 0 );
  assert( iThreadState != TS_ACTIVE );
}


/*******************************************************************************
 *
 * PUBLIC METHOD: StartThread
 *
 ******************************************************************************/
TThreadError CAThread::StartThread( void *aStartProc, void *aArg, int *aSystemSpecificError )
{
  int err;

  // check params
  assert( aSystemSpecificError != NULL );
  *aSystemSpecificError = 0;

  // check state
  if( iThreadState != TS_INIT ) {
    return TE_INVALIDSTATE;
  }

  // NOTE: we use the handle as a replacement for pthread_t. This is OK since they are both
  // 4-bytes integers -- however -- this obviously breaks the abstraction of the pthread_t
  // type so I validate this assumption here.
  assert( sizeof(pthread_t) == sizeof(HANDLE) );

  // create the thread
  err = pthread_create( (pthread_t*)&iThreadHandle, NULL, (void*(*)(void*))aStartProc, aArg );
  iProc = aStartProc;

  // handle errors
  if( err != 0 ) {
    *aSystemSpecificError = errno;
    return TE_ERROR;
  }

  // done
  iThreadState = TS_ACTIVE;
  return TE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: WaitForThread - To implement this method using pthreads we 
 * use two functions. pthread_getschedparam is a non-blocking call that returns
 * whether the thread is still active - pthread_join waits infinitely for the 
 * thread to complete. At the moment this implementation doesn't support wait
 * periods other than zero and infinite.
 *
 ******************************************************************************/
TThreadError CAThread::WaitForThread( int aTimeout )
{
  int err, thread_return_value;
  int policy;
  struct sched_param param;

  // check the state
  if( iThreadState != TS_ACTIVE ) {
    return TE_INVALIDSTATE;
  }
  
  // check the param
  assert( (aTimeout == 0) || (aTimeout == -1) );
  
  // in the non-blocking case we check whether the thread is active, if it is then we just return, otherwise, we then join with it.
  if( aTimeout == 0 ) {
    err = pthread_getschedparam( iThreadHandle, &policy, &param );
    if( err == 0 ) {
      return TE_TIMEOUT;
    } else if( err != ESRCH ) {
      return TE_ERROR;
    }
  }
  
  // we are either waiting indefinitely, or, we are waiting non-blocking but have just worked out that the thread is not running anymore
  err = pthread_join( iThreadHandle, (void**)&thread_return_value );
  assert( err == 0 );

  // cleanup our state
  iThreadHandle = 0;
  iThreadState = TS_DONE;
  return TE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: GetThreadState
 *
 ******************************************************************************/
TThreadState CAThread::GetThreadState()
{
  return iThreadState;
}

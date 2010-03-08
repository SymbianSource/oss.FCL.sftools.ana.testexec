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
* CAThread.windows.cpp 
* System Includes
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
	iThreadHandle = NULL;
	iThreadState = TS_INIT;
	iProc = NULL;
}


CAThread::CAThread( string aThreadName )
{
	iThreadHandle = NULL;
	iThreadState = TS_INIT;
	iProc = NULL;
	iThreadName = aThreadName;
}


CAThread::~CAThread()
{
	assert( iThreadHandle == NULL );
	assert( iThreadState != TS_ACTIVE );
}


TThreadError CAThread::StartThread( void *aStartProc, void *aArg, int *aSystemSpecificError )
{
	// check params
	assert( aSystemSpecificError != NULL );
	*aSystemSpecificError = 0;

	// create the thread
	iThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)aStartProc, aArg, 0, NULL );
	iProc = aStartProc;

	// handle errors
	if( iThreadHandle == NULL ) {
		*aSystemSpecificError = GetLastError();
		return TE_ERROR;
	}

	// done
	iThreadState = TS_ACTIVE;
	return TE_NONE;
}


TThreadError CAThread::WaitForThread( int aTimeout )
{
	int err;

	// wait for the object
	err = WaitForSingleObject( iThreadHandle, aTimeout );

	// handle the case that we got it
	if( err == WAIT_OBJECT_0 ) {
		iThreadState = TS_DONE;
		CloseHandle( iThreadHandle );
		iThreadHandle = NULL;
		return TE_NONE;
	}

	// handle a timeout
	if( err == WAIT_TIMEOUT ) {
		return TE_TIMEOUT;
	}

	// handle all other responses
	assert( !"INVALID CODE PATH" );
	return TE_NONE;
}


TThreadState CAThread::GetThreadState()
{
	return iThreadState;
}

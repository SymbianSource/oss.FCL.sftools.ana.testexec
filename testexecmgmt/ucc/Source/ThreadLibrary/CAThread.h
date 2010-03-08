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
* CAThread
*
*/



#ifndef __CATHREAD_H__
#define __CATHREAD_H__

/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <string>
using namespace std;

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
#ifndef WIN32
#define INFINITE (-1)
typedef pthread_t HANDLE;
#endif

/*******************************************************************************
 *
 * Type
 *
 ******************************************************************************/
typedef enum {
	TS_INIT,
	TS_ACTIVE,
	TS_DONE
} TThreadState;

typedef enum {
	TE_NONE,
	TE_ERROR,
	TE_TIMEOUT,
	TE_INVALIDSTATE,
} TThreadError;

/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CAThread
{
public:
	CAThread();
	CAThread( string aThreadName );
	~CAThread();

	TThreadError StartThread( void *aStartProc, void *aArg, int *aSystemSpecificError );
	TThreadError WaitForThread( int aTimeout );
	TThreadState GetThreadState();

private:
	TThreadState iThreadState;
	HANDLE iThreadHandle;
	void *iProc;
	string iThreadName;
};

#endif // __CATHREAD_H__

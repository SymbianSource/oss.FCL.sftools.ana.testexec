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
* Switches
* System Includes
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "proclib.h"

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * CAProcess
 *
 ******************************************************************************/
CAProcess::CAProcess()
{
	memset( &iProcessInfo, 0, sizeof(iProcessInfo) );
	iProcessState = PS_INIT;
}

CAProcess::~CAProcess()
{
//	assert( (iProcessState == PS_INIT) || (iProcessState == PS_REMOVED) );
}


/*******************************************************************************
 *
 * StartProcess()
 *
 ******************************************************************************/
int CAProcess::StartProcess( char *aCommandLine )
{
	int err;
	STARTUPINFO info;

	// check that the commandline is valid
	assert( aCommandLine != NULL );

	// verify the state
	assert( iProcessState == PS_INIT );

	// set the startup info to zero (required even though empty)
	memset( &info, 0, sizeof(info) );
	info.cb = sizeof(info);

	// create the process
	err = CreateProcess( NULL, aCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &info, &iProcessInfo );
	if( err == 0 ) {
		assert( iProcessInfo.hProcess == NULL );
		return GetLastError();
	}

	// update the state
	iProcessState = PS_STARTED;

	// done
	return 0;
}

/*******************************************************************************
 *
 * StopProcess()
 *
 ******************************************************************************/
int CAProcess::StopProcess( ) 
{
	int err, rv = 0;

	// check the state
	assert( iProcessState == PS_STARTED );
		
	// terminate the process
	err = TerminateProcess( iProcessInfo.hProcess, -1 );
	if( err == 0 ) {
		rv = GetLastError();
	}

	// update the state
	iProcessState = PS_STOPPED;
	return rv;
}

/*******************************************************************************
 *
 * WaitForProcess()
 *
 ******************************************************************************/
int CAProcess::WaitForProcess()
{
	int err, rv = 0;

	// check the state
	assert( (iProcessState == PS_STOPPED) || (iProcessState == PS_STARTED) );

	// now wait
	err = WaitForSingleObject( iProcessInfo.hProcess, INFINITE );
	if( err != WAIT_OBJECT_0 ) {
		rv = err;
	}

	// update the state
	iProcessState = PS_REMOVED;
	return rv;
}


/*******************************************************************************
 *
 * GetProcessStatus()
 *
 ******************************************************************************/
TProcessStatus CAProcess::GetProcessStatus()
{
	int err;

	// if the state is not started then return
	if( iProcessState != PS_STARTED ) {
		return iProcessState;
	}

	// otherwise see if the process is still running or has stopped itself
	err = WaitForSingleObject( iProcessInfo.hProcess, 0 );
	if( err != WAIT_TIMEOUT ) {
		iProcessState = PS_STOPPED;
	}

	// return the status
	return iProcessState;
}

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
* Standard Interface Values
*
*/


#include "../include/penstd.x"


/*******************************************************************************
 *
 * Constants
 *
 ******************************************************************************/
const MAXCOMMANDLINE		= 1024;


/*******************************************************************************
 *
 * Constants
 *
 ******************************************************************************/
const ERR_WAIT_PROCESS_ERROR			= -103;
const ERR_START_PROCESS_ERROR			= -105;
const ERR_STOP_PROCESS_ERROR			= -106;
const ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE		= -111;


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
struct TResult {
	int iStandardResult;
	int iExtendedCode;
	int iSystemError;
};

struct THostExecuteAsyncProcessInfo {
	int iErrorCode;
	int iErrorDetail;
	char iCommandLine[MAXCOMMANDLINE];
	int iProcessStatus;
	int iProcessExitReason;
	int iExitCode;
};
typedef opaque TVarData<>;


struct TStartupInfo {
	int iDummy;
};

/*******************************************************************************
 *
 * Interface
 *
 ******************************************************************************/
program HOSTEXECUTEASYNC {
	version HOSTEXECUTEASYNC_VERSION
	{
		/* Initialise the service */
		int SS_STARTUPRPCSERVICE( TStartupInfo )		= 1;
		int SC_SHUTDOWNRPCSERVICE( int )				= 2;
		TComponentList LIST_DEVICES( void )				= 30;

		/* Create instances */
		TResult CSTR_STARTPROCESS( string )				= 31;
		TResult DSTR_REMOVEPROCESS( int )				= 32;
		
		/* Operate on the instance */
		TResult KILLPROCESS( int )							= 5;
		TResult STOPPROCESS( int )							= 6;
		THostExecuteAsyncProcessInfo GETPROCESSINFO( int )	= 7;
		TVarData GETSTANDARDOUTPUT( int )					= 8;
		TVarData GETSTANDARDERROR( int )					= 9;	
	} = 9; 
} = 0x34630209;

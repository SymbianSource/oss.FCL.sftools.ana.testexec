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
 * Error codes
 *
 ******************************************************************************/
const ERR_WAIT_PROCESS_ERROR					= -103;
const ERR_START_PROCESS_ERROR					= -105;
const ERR_STOP_PROCESS_ERROR					= -106;
const ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE		= -111;
const ERR_SET_SIGNAL_ERROR						= -120;
const ERR_EXECUTE_PROCESS_ERROR					= -122;


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef opaque TVarData<>;

struct TStartupInfo {
	int iEmpty;
};

struct TExecuteRequest {
	char iCommandLine[MAXCOMMANDLINE];
	int iRecordStandardOut;
	int iRecordStandartError;
	int iTimeout;
};

struct TExecuteResult {
	int iResult;
	int iErrorCode;
	int iErrorDetail;
	int iProcessExitReason;
	int iProcessExitDetail;
	TVarData iStandardOut;
	TVarData iStandardError;	
};


/*******************************************************************************
 *
 * Interface
 *
 ******************************************************************************/
program HOSTEXECUTE {
	version HOSTEXECUTE_VERSION
	{
		/* Service Management */
		int SS_STARTUPRPCSERVICE( TStartupInfo )		= 1;
		int SC_SHUTDOWNRPCSERVICE( int )				= 2;
		TComponentList LIST_DEVICES( void )				= 30;

		/* Custom Interface */
		TExecuteResult ST_EXECUTECOMMAND( TExecuteRequest ) = 3;
	} = 3;
} =0x34630203;


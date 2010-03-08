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
const MAXADDRLEN				= 64;
const MAXPORTLEN				= 16;
const MAXCONFIGURATIONLENGTH	= 1024;
const MAXSESSIONNAMELEN			= 16;
const INVALID_SESSION_ID		= 0xFFFFFFFF;


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
const ERR_LOG_FILE_ERROR						= -121;
const ERR_MULTIPLE_MATCHES_IN_LOG_FILE			= -122;


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
enum TPppSessionStatus {
	SS_NOT_STARTED,
	SS_CONNECTING,
	SS_CONNECTED,
	SS_DISCONNECTING,
	SS_DISCONNECTED
};

struct TStartupInfo {
	int iEmpty;
};

struct TPppSessionConfig {
	char	iMobsterAddress[MAXADDRLEN];
	char	iMobsterPort[MAXPORTLEN];
	char	iSessionConfiguration[MAXCONFIGURATIONLENGTH];
	int		iMTID;
};

struct TPppSessionDesc {
	int iErrorCode;
	int iErrorDetail;
	TPppSessionConfig iConfig;
	TPppSessionStatus iSessionStatus;
	char iInterfaceName[MAXSESSIONNAMELEN];
	int iLocalIPAddress;
	int iRemoteIPAddress;
	int iProcessStatus;
	int	iProcessExitReason;
	int	iProcessExitCode;
};

struct TResult {
	int iStandardResult;
	int iExtendedCode;
	int iSystemError;
};

typedef opaque TVarData<>;


/*******************************************************************************
 *
 * PPPCONTROLLER Interface
 *
 ******************************************************************************/
program PPPCONTROLLER {
	version PPPCONTROLLER_VERSION {

		/* Initialise the service */
		int SS_STARTUPRPCSERVICE( TStartupInfo )		= 1;
		int SC_SHUTDOWNRPCSERVICE( int )				= 2;
		TComponentList LIST_DEVICES( void )				= 30;

		/* Create Instances */
		TResult CSTR_STARTPPPSESSION( TPppSessionConfig )	= 31;
		TResult DSTR_REMOVEPPPSESSION( int )				= 32;

		/* Operate on the instance */
		TResult KILLSESSION( int )						= 5;
		TResult STOPSESSION( int )						= 6;
		TPppSessionDesc GETSESSIONINFO( int )			= 7;
		TVarData GETPPPLOG( int )						= 8;
	} = 4;
} = 0x34630204;

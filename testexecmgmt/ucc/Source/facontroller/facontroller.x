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
 * Interface Constants
 *
 ******************************************************************************/
const X_MAXOPTIONTOKENSIZE			= 512;
const X_MAXINTERFACENAMESIZE		= 64;


/*******************************************************************************
 *
 * Error codes
 *
 ******************************************************************************/
const ERR_WAIT_PROCESS_ERROR			= -103;
const ERR_START_PROCESS_ERROR			= -105;
const ERR_STOP_PROCESS_ERROR			= -106;
const ERR_DYNAMICS_CALL_FAILED			= -107;
const ERR_CONFIG_FILE_ERROR				= -108;
const ERR_POLICY_OUTPUT_PARSING_FAILED			= -110;
const ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE		= -111;
const ERR_NO_MORE_INTERFACES					= -112;
const ERR_CREATE_INTERFACE_ERROR				= -113;
const ERR_DESTROY_INTERFACE_ERROR				= -114;
const ERR_INTEGER_ALLOCATOR_SETUP_ERROR			= -115;


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
enum TSoliticationMode {
	SM_SEND_NEVER								= -1,				
	SM_SEND_IN_RESPONSE_TO_SOLICITATION_ONLY	= 0,
	SM_SEND_REGULAR								= 1
};

struct TStartupInfo {
	int iBaseInterfaceIndex;
	int iNetworkMask;
	int iStartHostAddressRange;
	int iStopHostAddressRange;
};

struct TResult {
	int iServiceResult;
	int iSubComponentResult;
	int iData0;
	int iData1;
};

struct TOptionDesc {
	int	 iAgentID;
	char iOptionFilename[X_MAXOPTIONTOKENSIZE];
	char iOptionToken[X_MAXOPTIONTOKENSIZE];
	char iOptionValue[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockStart[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockEnd[X_MAXOPTIONTOKENSIZE];
};

struct TTimeoutRequest {
	int iAgentID;
	int iTimeout;
};

struct TGetTunnelRequest {
	int iAgentID;
	int iMobileNodeAddress;
};

struct TStartAgentRequest {
	int iAgentID;
	char iLowerInterface[X_MAXINTERFACENAMESIZE];
	TSoliticationMode iSolicitationMode;
	int iSolicitationInterval;
};

struct TFaStatusInfo {
  TResult iCallResult;
  int iTunnelCount;
  int iPendingRegistrationRequests;
  int iRequestsRejected;
  int iRequestsAccepted;
  int iReplysAccepted;
  int iReplysRejected;
  int iDiscardedUnknownExtension;
  int iDiscardedMalformed;
  int iDiscardedVendor;
  int iAdvertisementsSent;
};

struct TFaTunnelID {
	int iAgentID;
	int iMobileNodeAddress;
	int iHomeAgentAddress;
	int iID;
};

struct TFaTunnelInfo {
  TResult iCallResult;
  unsigned int iMobileNodeAddress;
  unsigned int iCareofAddress;
  unsigned int iHomeAgentAddress;
  int iPrivateHomeAgentID;
  int iCreationTime;
  int iExpirationTime;
  int iRefreshTime;
  int iLastTimestamp;
  int iSPI;
  int iTimeout;
};

typedef TFaTunnelID TFaTunnelList<>;


/*******************************************************************************
 *
 * FACONTROLLER Interface
 *
 ******************************************************************************/
program FACONTROLLER {
	version FACONTROLLER_VERSION {

		/* Standard Service Management */
		int SS_STARTUPRPCSERVICE( TStartupInfo )						= 1;
		int SC_SHUTDOWNRPCSERVICE( int )								= 2;
		TComponentList LIST_DEVICES( void )								= 30;

		/* Create instances */
		TResult CSTR_CREATEAGENT( void )								= 31;
		int DSTR_REMOVEAGENT( int )										= 32;

		/* Start and stop the mobile agent */
		TResult STARTMOBILEAGENT( TStartAgentRequest )					= 3;
		TResult STOPMOBILEAGENT( int )									= 4;
		TResult GETMOBILEAGENTSTATUS( int )								= 5;

		/* Configure the agent -- must be called before starting the agent */
		TResult SETSINGLEOPTION( TOptionDesc )							= 6;
		TResult REMOVESINGLEOPTION( TOptionDesc )						= 33;
		TResult ADDLISTOPTION( TOptionDesc )							= 7;
		TResult REMOVELISTOPTION( TOptionDesc )							= 8;

		/* Runtime commands */
		TFaStatusInfo GETSTATUS( int )									= 9;
		TResult DESTROYTUNNELID( TFaTunnelID )							= 11;
		TFaTunnelList LISTTUNNELS( int )								= 12;
		TFaTunnelInfo GETTUNNELINFO( TGetTunnelRequest )				= 15;	

		/* Set the timeout for all dynamic commands */
		void SETTIMEOUT( TTimeoutRequest )								= 13;
	} = 7;
} = 0x34630207;

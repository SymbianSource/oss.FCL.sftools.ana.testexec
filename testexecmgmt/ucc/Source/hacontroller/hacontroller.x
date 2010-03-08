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
const X_MAXOPTIONTOKENSIZE				= 512;
const X_MAXINTERFACENAMESIZE			= 64;


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
const ERR_VIRTUAL_NETWORK_CONFIGURATION_ERROR	= -116;
const ERR_NO_MORE_VIRTUAL_NETWORKS				= -117;


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
struct TStartupInfo {
	int iBaseInterfaceIndex;
	int iBaseInterfaceNetworkMask;
	int iBaseInterfaceStartHostAddressRange;
	int iBaseInterfaceStopHostAddressRange;
	int iVirtualNetworkPrefix;
	int iVirtualNetworkPrefixMask;
	int iVirtualNetworkAllocSize;
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

struct THaStatus {
  TResult iCallResult;
  int iTunnelCount;
  int iRequestsRejected;
  int iRequestsAccepted;
  int iDiscardedUnknownExtension;
  int iDiscardedMalformed;
  int iDiscardedVendor;
  int iAdvertisementsSent;
  int iHomeAgentAddress;
  int iVirtualNetworkStartAddress;
  int iVirtualNetworkSize;
};

struct THaTunnelInfo {
  TResult iCallResult;
  unsigned int iMobileNodeAddress;
  unsigned int iCareofAddress;
  unsigned int iHomeAgentAddress;
  int iCreationTime;
  int iExpirationTime;
  int iRefreshTime;
  int iLastTimestamp;
  int iSPI;
  int iTimeout;
};

struct THaTunnelID {
	int iAgentID;
	int iMobileNodeAddress;
};

typedef THaTunnelID THaTunnelList<>;

struct TTimeoutRequest {
	int iAgentID;
	int iTimeout;
};

struct TGetTunnelRequest {
	int iAgentID;
	int iMobileNodeAddress;
};

/*******************************************************************************
 *
 * HACONTROLLER Interface
 *
 ******************************************************************************/
program HACONTROLLER {
	version HACONTROLLER_VERSION {

		/* Initialise the service */
		int SS_STARTUPRPCSERVICE( TStartupInfo )		= 1;
		int SC_SHUTDOWNRPCSERVICE( int )				= 2;
		TComponentList LIST_DEVICES( void )				= 30;

		/* Create instances */
		TResult CSTR_CREATEAGENT( void )				= 31;
		TResult DSTR_REMOVEAGENT( int )					= 32;

		/* Start and stop the mobile agent */
		TResult STARTMOBILEAGENT( int )					= 3;
		TResult STOPMOBILEAGENT( int )					= 4;
		TResult GETMOBILEAGENTSTATUS( int )				= 5;

		/* Configure the agent -- must be called before starting the agent */
		TResult SETSINGLEOPTION( TOptionDesc )			= 6;
		TResult REMOVESINGLEOPTION( TOptionDesc )		= 33;
		TResult ADDLISTOPTION( TOptionDesc )			= 7;
		TResult REMOVELISTOPTION( TOptionDesc )			= 8;

		/* Dynamic commands */
		THaStatus GETSTATUS( int )						= 9;
		TResult DESTROYTUNNELID( THaTunnelID )			= 11;
		THaTunnelList LISTTUNNELS( int )				= 12;
		THaTunnelInfo GETTUNNELINFO( TGetTunnelRequest )	= 14;

		/* Set the timeout for all dynamic commands */
		void SETTIMEOUT( TTimeoutRequest )				= 13;
	} = 8;
} = 0x34630208;

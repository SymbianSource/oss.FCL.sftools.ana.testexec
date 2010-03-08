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
const X_MAXINTERFACENAMELEN		= 64;
const X_MAXNAILEN				= 128;
const X_MAXOPTIONTOKENSIZE		= 512;
const X_MAXINFOLEN              = 128;
const X_MAXERRORSTRING          = 256;


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
const ERR_POLICY_OUTPUT_PARSING_FAILED	= -110;
const ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE		= -111;
const ERR_SET_INTERFACE_ADDRESS_ERROR			= -112;


/*******************************************************************************
 *
 * Constant values passed as parameters
 *
 ******************************************************************************/
const FP_EARLY_EXPIRE	= 1;
const FP_NEWEST_FA		= 2;
const FP_EAGER			= 4;
const FP_NEWEST_ADV		= 8;

enum TMnTunnelingMode {
	TM_NO_TUNNEL,
	TM_FULL_TUNNEL,
	TM_TRIANGLE_TUNNEL
};

enum TSetInterfaceErrors {
	SIE_SOCKET_FAILED,
	SIE_IOCTL_GET_DESTINATION_ADDRESS_FAILED,
	SIE_IOCTL_SET_ADDRESS_FAILED,
	SIE_IOCTL_SET_DESTINATION_ADDRESS_FAILED
};


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
struct TResult {
	int iServiceResult;
	int iSubComponentResult;
	int iData0;
	int iData1;
};

struct THomeAddressDesc {
	int		iAgentID;
	int		iHomeAddressPrefix;
	int		iHomeAddress;
	int		iHomeAgentAddress;
	char	iInterfaceName[X_MAXINTERFACENAMELEN];
}; 

struct TOptionDesc {
	int  iAgentID;
	char iOptionToken[X_MAXOPTIONTOKENSIZE];
	char iOptionValue[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockStart[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockEnd[X_MAXOPTIONTOKENSIZE];
};

struct TTunnelingModeDesc {
	int iAgentID;
	TMnTunnelingMode iTunnelMode;
};

struct TPolicyRequest {
	int iAgentID;
	int iPolicy;
};

struct TForeignAgentInfoRequest {
	int iAgentID;
	int iForeignAgentID;
};

struct TTimeoutRequest {
	int iAgentID;
	int iTimeout;
};

struct TMobileNodeStatus {
  TResult iCallResult;
  int iConnected;
  int iTunnelUp;
  unsigned int iLocalAddress;
  unsigned int iColocatedAddress;
  unsigned int iForeignAgentAddress;
  unsigned int iHomeAgentAddress;
  unsigned int iHomeAddress;
  int iLifetimeRemaining;
  int iTunnelMode;
  int iSecondsSinceLastRequest;
  int iSecondsSinceLastReply;
  int iReplyCode;
  char iInfoText[X_MAXINFOLEN];
  char iLastErrorString[X_MAXERRORSTRING];
};

struct TForeignAgentInfo {
  TResult iCallResult;
  unsigned int iForeignAgentAddress;
  char iInterfaceName[X_MAXINTERFACENAMELEN];
  int iPriority;
  int iInUse;
  int iInterfaceIndex;
  int iLastAdvertisement;
  char iNAI[X_MAXNAILEN];
  int iAdvertisementExpiry;
};

typedef struct TForeignAgentInfo TForeignAgentList<>;


/*******************************************************************************
 *
 * MNCONTROLLER Interface
 *
 ******************************************************************************/
program MNCONTROLLER {
	version MNCONTROLLER_VERSION {

		/* Standard Service Management */
		int SS_STARTUPRPCSERVICE( void )								= 1;
		int SC_SHUTDOWNRPCSERVICE( int )								= 2;
		TComponentList LIST_DEVICES( void )								= 30;

		/* Create instances */
		TResult CSTR_CREATEAGENT( void )								= 31;
		int DSTR_REMOVEAGENT( int )										= 32;

		/* Start and stop the mobile agent */
		TResult STARTMOBILEAGENT( int )									= 3;
		TResult STOPMOBILEAGENT( int )									= 4;
		TResult GETMOBILEAGENTSTATUS( int )									= 5;

		/* Configure the agent -- must be called before starting the agent */
		TResult SETSINGLEOPTION( TOptionDesc )							= 6;
		TResult REMOVESINGLEOPTION( TOptionDesc )						= 33;
		TResult ADDLISTOPTION( TOptionDesc )							= 7;
		TResult REMOVELISTOPTION( TOptionDesc )							= 8;

		/* Custom configuration */
		TResult SETHOMEADDRESS( THomeAddressDesc )						= 23;

		/* Runtime commands */
		TResult GETCAREOFADDRESS( int )									= 9;
		TResult CONNECT( TTunnelingModeDesc )							= 12;		
		TResult DISCONNECT( int )										= 13;
		TResult CONFIRM( int )											= 16;
		TResult RESCANINTERFACES( int )									= 17;	
		TResult SETFAPOLICY( TPolicyRequest )							= 18;
		TResult GETFAPOLICY( int )										= 19;

		TMobileNodeStatus GETSTATUS( int )								= 11;		
		TForeignAgentList LISTFOREIGNAGENTS( int )							= 20;		
		TForeignAgentInfo GETFOREIGNAGENTINFO( TForeignAgentInfoRequest )	= 21;		

		/* Set the timeout for all dynamic commands */
		void SETTIMEOUT( TTimeoutRequest )								= 22;
	} = 5; 
} = 0x34630205;

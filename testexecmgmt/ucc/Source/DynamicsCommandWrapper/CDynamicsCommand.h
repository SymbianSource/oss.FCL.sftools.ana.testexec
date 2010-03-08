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
* CDynamicsCommand
*
*/



#ifndef __CDYNAMICSCOMMAND_H__
#define __CDYNAMICSCOMMAND_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <string>
using namespace std;


/****************************************************************************************
 * 
 * Constants
 * 
 ***************************************************************************************/
#define MAXIMUMERRORSTRINGLENGTH	128
#define MAXCOMMANDLINE			256
#define MAXINTERFACENAME                64
#define MAXNAILEN                       128
#define MAXINFOLEN                      128
#define MAXERRORSTRING                  256


/****************************************************************************************
 * 
 * Types - General
 * 
 ***************************************************************************************/
typedef enum {
	DC_SUCCESS,
	DC_COMMAND_TOOL_NOT_FOUND,
	DC_INVALID_ARGUMENTS,
	DC_CSH_FAILED,
	DC_CALL_FAILED,
	DC_EXECUTE_FAILED,
	DC_UNEXPECTED_OUTPUT,
} TDynamicsCallResult;

typedef enum {
  DCF_UNKNOWN,
  DCF_FUNCTION_NOT_PERMITTED,
  DCF_AGENT_UNREACHABLE
} TDynamicsCallFailedReason;

typedef struct {
	TDynamicsCallResult iResult;
	int iErrorCode;
	int iErrorDetail;
	char iDynamicsErrorString[MAXIMUMERRORSTRINGLENGTH];
} TDynamicsCallInfo;


/****************************************************************************************
 * 
 * Types - For Foreign Agent Calls
 * 
 ***************************************************************************************/
typedef struct {
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
} TForeignAgentStatusInfo;

typedef struct {
  unsigned int iMobileNodeAddress;
  unsigned int iHomeAgentAddress;
  int iTunnelID;
} TTunnelID;

typedef struct {
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
} TForeignAgentTunnelInfo;


/****************************************************************************************
 * 
 * Types - For Home Agent Calls
 * 
 ***************************************************************************************/
typedef struct {
  int iTunnelCount;
  int iRequestsRejected;
  int iRequestsAccepted;
  int iDiscardedUnknownExtension;
  int iDiscardedMalformed;
  int iDiscardedVendor;
  int iAdvertisementsSent;
} THomeAgentStatusInfo;

typedef struct {
  unsigned int iMobileNodeAddress;
  unsigned int iCareofAddress;
  unsigned int iHomeAgentAddress;
  int iCreationTime;
  int iExpirationTime;
  int iRefreshTime;
  int iLastTimestamp;
  int iSPI;
  int iTimeout;
} THomeAgentTunnelInfo;


/****************************************************************************************
 * 
 * Types - For Mobile Node Agent Calls
 * 
 ***************************************************************************************/
typedef struct {
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
  char iInfoText[MAXINFOLEN];
  char iLastErrorString[MAXERRORSTRING];
} TMobileNodeAgentStatusInfo;

typedef struct {
  unsigned int iForeignAgentAddress;
  char iInterfaceName[MAXINTERFACENAME];
  int iPriority;
  int iInUse;
  int iInterfaceIndex;
  int iLastAdvertisement;
  char iNAI[MAXNAILEN];
} TMobileNodeForeignAgentInfo;


/****************************************************************************************
 * 
 * CDynamicsCommand Class Definition
 * 
 ***************************************************************************************/
class CDynamicsCommand 
{
public:
	// General
	CDynamicsCommand();
	~CDynamicsCommand();
	void SetPipePath( string aPipePath );
	void SetTimeout( int aTimeoutInMilliseconds );

	// Foreign Agent Calls
	TDynamicsCallInfo ForeignAgentGetStatus( TForeignAgentStatusInfo *aForeignAgentInfo );
	TDynamicsCallInfo ForeignAgentDestroyTunnel( unsigned int aMobileNodeAddress );
	TDynamicsCallInfo ForeignAgentGetTunnels( int *aTunnelCount, TTunnelID *aTunnelIDArray );
	TDynamicsCallInfo ForeignAgentGetTunnelInfo( unsigned int aMobileNodeAddress, TForeignAgentTunnelInfo *aTunnelInfo );

	// Home Agent Calls
	TDynamicsCallInfo HomeAgentGetStatus( THomeAgentStatusInfo *aHomeAgentInfo );
	TDynamicsCallInfo HomeAgentDestroyTunnel( unsigned int aMobileNodeAddress , unsigned int aHomeAgentAddress = 0, int aTunnelID = 0);
	TDynamicsCallInfo HomeAgentGetTunnels( int *aTunnelCount, unsigned int *aMobileNodeAddresses );
	TDynamicsCallInfo HomeAgentGetTunnelInfo( unsigned int aMobileNodeAddress, THomeAgentTunnelInfo *aTunnelInfo );

	// Mobile Node Calls
	TDynamicsCallInfo MobileNodeAgentGetCareofAddress( struct in_addr *aCareofAddress );
	TDynamicsCallInfo MobileNodeAgentGetStatus( TMobileNodeAgentStatusInfo *aMobileNodeAgentInfo );
	TDynamicsCallInfo MobileNodeAgentConnect();
	TDynamicsCallInfo MobileNodeAgentDisconnect();
	TDynamicsCallInfo MobileNodeAgentConfirm();
	TDynamicsCallInfo MobileNodeAgentRescan();
	TDynamicsCallInfo MobileNodeAgentSetPolicy( int aPolicyVector );
	TDynamicsCallInfo MobileNodeAgentGetPolicy( int *aPolicyVector );
	TDynamicsCallInfo MobileNodeAgentGetForeignAgentList( int *aListCount, TMobileNodeForeignAgentInfo *aForeignAgentInfoList );
	TDynamicsCallInfo MobileNodeAgentGetForeignAgentInfo( unsigned int aFAAddress, TMobileNodeForeignAgentInfo *aForeignAgentInfo );

private:
	// Helper functions
	TDynamicsCallInfo DynamicsCall( char *aExecutable, char *aCommand, char *aParams, string *aStandardOutput, 
					string *aStandardError );

	int GetIntegerEntry( const char *aOutput, const char *aLabel, int *aValue );
	int GetDateEntry( const char *aOutput, const char *aLabel, int *aValue );
	int GetAddressEntry( const char *aOutput, const char *aLabel, unsigned int *aValue );
	int GetPolicyEntry( const char *aOutput, const char *aLabel, int *aValue );
	int GetStringEntry( const char *aOutput, const char *aLabel, char *aValue, int aBufferSize  );

	TDynamicsCallFailedReason GetCallFailedReason( const char *aOutput );

	TDynamicsCallInfo ParseForeignAgentListOutput( string aOutput, int aListSize, TTunnelID *aTunnelIDArray, int *aListCount );
	TDynamicsCallInfo ParseHomeAgentListOutput( string aOutput, int aListSize, unsigned int *aTunnelList, int *aListCount );
	TDynamicsCallInfo ParseMobileNodeAgentListOutput( string aOutput, int aListSize, TMobileNodeForeignAgentInfo *aForeignAgentList, 
							  int *aListCount );

	int ConvertTunnelStringToInteger( char *aTunnelMode, int *aValue );
	int CheckMatchAsBoolean( char *aInput, char *aTrue, int *aValue );
	int strstrbeforedelim( const char *aSearchSpace, const char *aToken, char aDelim );

	// Members
	int iTimeout;
	string iPipePath;
	char iCommandLine[MAXCOMMANDLINE];
};


#endif //__CDYNAMICSCOMMAND_H__

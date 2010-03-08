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
* System Includes
*
*/



#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif
#include <stdlib.h>


/****************************************************************************************
 * 
 * Local Includes
 * 
 ***************************************************************************************/
#include "CDynamicsCommand.h"
#include "../include/standard_unix.h"
#include "../ProcessLibrary/proclib.h"
#include "../include/strncpynt.h"


/****************************************************************************************
 * 
 * Definitions
 * 
 ***************************************************************************************/
#define HOME_AGENT_TOOL				"./dynha_tool"
#define FOREIGN_AGENT_TOOL			"./dynfa_tool"
#define MOBILE_NODE_AGENT_TOOL		        "./dynmn_tool"

#define CALL_FAILED_PREFIX			"Call failed: "
#define INVALID_ARGUMENTS_PREFIX	        "Invalid arguments. "
#define CSH_FAILED_PREFIX                       "csh: "
#define TARGET_NOT_FOUND                        "No such file" 
#define FOREIGN_AGENT_DESTROY_TUNNEL_PREFIX     "Destroyed tunnel"
#define MOBILE_AGENT_FOREIGN_AGENT_LIST_PREFIX  "List of heard mobility agents:"
#define MOBILE_NODE_AGENT_SETPOLICY_PREFIX      "Set policy succeeded."
#define MOBILE_NODE_AGENT_CONNECT_PREFIX        "Connect success."
#define MOBILE_NODE_AGENT_DISCONNECT_PREFIX     "Disconnected"
#define MOBILE_NODE_AGENT_CONFIRM_PREFIX        "Confirmed."
#define CALL_FAILED_FUNCTION_NOT_PERMITTED      "function not permitted"
#define CALL_FAILED_AGENT_UNREACHABLE           "agent unreachable"

#define TOKENSIZE                               256
#define MAX_TIMEOUT			        120000
#define MIN_TIMEOUT				1


/****************************************************************************************
 * 
 * Macro Functions
 * 
 ***************************************************************************************/
#define IS_WHITESPACE(c)  (((c) == ' ')||((c) == '\t'))
#define MIN(x,y)          (((x) < (y)) ? (x) : (y))


/****************************************************************************************
 * 
 * File-scope static variables
 * 
 ***************************************************************************************/
char *iTunnelModeString[] = { "no tunnel", "full tunnel", "triangle tunnel", "full tunnel direct to HA", NULL };
char *iPolicyString[] = { "Early-expire", "Newest-FA", "Eager-switching", "Newest-ADV", NULL };


/****************************************************************************************
 * 
 * PUBLIC METHOD: General
 * 
 ***************************************************************************************/
CDynamicsCommand::CDynamicsCommand()
{
	iTimeout = MAX_TIMEOUT;
}


CDynamicsCommand::~CDynamicsCommand()
{
}


void CDynamicsCommand::SetPipePath( string aPipePath )
{
	iPipePath = aPipePath;
}


void CDynamicsCommand::SetTimeout( int aTimeout )
{
	if( (aTimeout >= MIN_TIMEOUT) && (aTimeout <= MAX_TIMEOUT) ) {
		iTimeout = aTimeout;
	}
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: ForeignAgentGetStatus
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ForeignAgentGetStatus( TForeignAgentStatusInfo *aForeignAgentInfo )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;

  // check params
  assert( aForeignAgentInfo != NULL );
  
  // run the dynamics command line 
  rv = DynamicsCall( FOREIGN_AGENT_TOOL, "status", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output 
  res |= GetIntegerEntry( standard_output.c_str(), "tunnels", &(aForeignAgentInfo->iTunnelCount) );
  res |= GetIntegerEntry( standard_output.c_str(), "pending reg.req.", &(aForeignAgentInfo->iPendingRegistrationRequests) );
  res |= GetIntegerEntry( standard_output.c_str(), "request rejected", &(aForeignAgentInfo->iRequestsRejected) );
  res |= GetIntegerEntry( standard_output.c_str(), "request accepted", &(aForeignAgentInfo->iRequestsAccepted) );
  res |= GetIntegerEntry( standard_output.c_str(), "reply accepted", &(aForeignAgentInfo->iReplysAccepted) );
  res |= GetIntegerEntry( standard_output.c_str(), "reply rejected", &(aForeignAgentInfo->iReplysRejected) );
  res |= GetIntegerEntry( standard_output.c_str(), "discard(unk. ext)", &(aForeignAgentInfo->iDiscardedUnknownExtension) );
  res |= GetIntegerEntry( standard_output.c_str(), "discard(malformed)", &(aForeignAgentInfo->iDiscardedMalformed) );
  res |= GetIntegerEntry( standard_output.c_str(), "discard(vendor)", &(aForeignAgentInfo->iDiscardedVendor) );
  res |= GetIntegerEntry( standard_output.c_str(), "advertisement sent", &(aForeignAgentInfo->iAdvertisementsSent) );
  
  // check for errors
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  
  // done
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: ForeignAgentDestroyTunnel
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ForeignAgentDestroyTunnel( unsigned int aMobileNodeAddress )
{
  TDynamicsCallInfo rv;
  int match;
  string standard_output, standard_error;
  char *mnaddr;
  struct in_addr iaddr;
  char params[MAXCOMMANDLINE];
 
  // create the params string
  iaddr.ADDRESS_INTEGER = aMobileNodeAddress;
  mnaddr = inet_ntoa( iaddr );
  snprintf( params, MAXCOMMANDLINE, "%s", mnaddr );

  // run the dynamics command line 
  rv = DynamicsCall( FOREIGN_AGENT_TOOL, "destroy", params, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // check for success - otherwise give general error
  match = strncmp( FOREIGN_AGENT_DESTROY_TUNNEL_PREFIX, standard_output.c_str(), strlen(FOREIGN_AGENT_DESTROY_TUNNEL_PREFIX) );
  if( match != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: ForeignAgentGetTunnels
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ForeignAgentGetTunnels( int *aTunnelCount, TTunnelID *aTunnelIDArray )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int list_size;

  // check the params
  assert( aTunnelCount != NULL );
  assert( aTunnelIDArray != NULL );
  list_size = *aTunnelCount;
  *aTunnelCount = 0;

  // run the dynamics command line 
  rv = DynamicsCall( FOREIGN_AGENT_TOOL, "list", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output and return
  rv =  ParseForeignAgentListOutput( standard_output, list_size, aTunnelIDArray, aTunnelCount );
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: ForeignAgentGetTunnelInfo
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ForeignAgentGetTunnelInfo( unsigned int aMobileNodeAddress, TForeignAgentTunnelInfo *aTunnelInfo )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;
  char params[MAXCOMMANDLINE];
  struct in_addr mobile_node_address;

  // check params
  assert( aTunnelInfo != NULL );

  // write the params
  mobile_node_address.ADDRESS_INTEGER = aMobileNodeAddress;
  snprintf( params, MAXCOMMANDLINE, " %s", inet_ntoa(mobile_node_address) );

  // run the dynamics command line 
  rv = DynamicsCall( FOREIGN_AGENT_TOOL, "show", params, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output 
  res |= GetAddressEntry( standard_output.c_str(), "mobile addr", &(aTunnelInfo->iMobileNodeAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "care-of addr", &(aTunnelInfo->iCareofAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "home agent addr", &(aTunnelInfo->iHomeAgentAddress) );
  res |= GetIntegerEntry( standard_output.c_str(), "private HA id", &(aTunnelInfo->iPrivateHomeAgentID) );
  res |= GetDateEntry( standard_output.c_str(), "creation time", &(aTunnelInfo->iCreationTime) );
  res |= GetDateEntry( standard_output.c_str(), "expiration time", &(aTunnelInfo->iExpirationTime) );
  res |= GetDateEntry( standard_output.c_str(), "refresh time", &(aTunnelInfo->iRefreshTime) );
  res |= GetDateEntry( standard_output.c_str(), "last timestamp", &(aTunnelInfo->iLastTimestamp) );
  res |= GetIntegerEntry( standard_output.c_str(), "SPI", &(aTunnelInfo->iSPI) );
  res |= GetIntegerEntry( standard_output.c_str(), "timeout", &(aTunnelInfo->iTimeout) );
  
  // check for errors
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  
  // done
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: HomeAgentGetStatus
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::HomeAgentGetStatus( THomeAgentStatusInfo *aHomeAgentInfo )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;

  // check params
  assert( aHomeAgentInfo != NULL );
  
  // run the dynamics command line 
  rv = DynamicsCall( HOME_AGENT_TOOL, "status", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output 
  res |= GetIntegerEntry( standard_output.c_str(), "tunnels", &(aHomeAgentInfo->iTunnelCount) );
  res |= GetIntegerEntry( standard_output.c_str(), "request rejected", &(aHomeAgentInfo->iRequestsRejected) );
  res |= GetIntegerEntry( standard_output.c_str(), "request accepted", &(aHomeAgentInfo->iRequestsAccepted) );
  res |= GetIntegerEntry( standard_output.c_str(), "discard(unk. ext)", &(aHomeAgentInfo->iDiscardedUnknownExtension) );
  res |= GetIntegerEntry( standard_output.c_str(), "discard(malformed)", &(aHomeAgentInfo->iDiscardedMalformed) );
  res |= GetIntegerEntry( standard_output.c_str(), "discard(vendor)", &(aHomeAgentInfo->iDiscardedVendor) );
  res |= GetIntegerEntry( standard_output.c_str(), "advertisement sent", &(aHomeAgentInfo->iAdvertisementsSent) );
  
  // check for errors
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  
  // done
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: HomeAgentDestroyTunnel
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::HomeAgentDestroyTunnel( unsigned int aMobileNodeAddress, unsigned int aHomeAgentAddress, 
							    int aTunnelID )
{
  TDynamicsCallInfo rv;
  int match;
  string standard_output, standard_error;
  char *mnaddr;
  struct in_addr iaddr;
  char params[MAXCOMMANDLINE];

  // create the params string
  iaddr.ADDRESS_INTEGER = aMobileNodeAddress;
  mnaddr = inet_ntoa( iaddr );
  snprintf( params, MAXCOMMANDLINE, "%s", mnaddr );

  // run the dynamics command line 
  rv = DynamicsCall( HOME_AGENT_TOOL, "destroy", params, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // check for success - otherwise give general error
  match = strncmp( FOREIGN_AGENT_DESTROY_TUNNEL_PREFIX, standard_output.c_str(), strlen(FOREIGN_AGENT_DESTROY_TUNNEL_PREFIX) );
  if( match != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: HomeAgentGetTunnels
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::HomeAgentGetTunnels( int *aTunnelCount, unsigned int *aMobileNodeAddresses )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int list_size;

  // check the params
  assert( aTunnelCount != NULL );
  assert( aMobileNodeAddresses != NULL );
  list_size = *aTunnelCount;
  *aTunnelCount = 0;

  // run the dynamics command line 
  rv = DynamicsCall( HOME_AGENT_TOOL, "list", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output and return
  rv =  ParseHomeAgentListOutput( standard_output, list_size, aMobileNodeAddresses, aTunnelCount );
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: HomeAgentGetTunnelInfo
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::HomeAgentGetTunnelInfo( unsigned int aMobileNodeAddress, THomeAgentTunnelInfo *aTunnelInfo )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;
  char params[MAXCOMMANDLINE];
  struct in_addr mobile_node_address;

  // check params
  assert( aTunnelInfo != NULL );

  // write the params
  mobile_node_address.ADDRESS_INTEGER = aMobileNodeAddress;
  snprintf( params, MAXCOMMANDLINE, " %s", inet_ntoa(mobile_node_address) );

  // run the dynamics command line 
  rv = DynamicsCall( HOME_AGENT_TOOL, "show", params, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output   
  res |= GetAddressEntry( standard_output.c_str(), "mobile addr", &(aTunnelInfo->iMobileNodeAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "care-of addr", &(aTunnelInfo->iCareofAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "home agent addr", &(aTunnelInfo->iHomeAgentAddress) );
  res |= GetDateEntry( standard_output.c_str(), "creation time", &(aTunnelInfo->iCreationTime) );
  res |= GetDateEntry( standard_output.c_str(), "expiration time", &(aTunnelInfo->iExpirationTime) );
  res |= GetDateEntry( standard_output.c_str(), "refresh time", &(aTunnelInfo->iRefreshTime) );
  res |= GetDateEntry( standard_output.c_str(), "last timestamp", &(aTunnelInfo->iLastTimestamp) );
  res |= GetIntegerEntry( standard_output.c_str(), "SPI", &(aTunnelInfo->iSPI) );
  res |= GetIntegerEntry( standard_output.c_str(), "timeout", &(aTunnelInfo->iTimeout) );
  
  // check for errors
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  
  // done
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentGetCareofAddress
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentGetCareofAddress( struct in_addr *aCareofAddress )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;
  unsigned int addr;

  // check params
  assert( aCareofAddress != NULL );

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "careof", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output   
  res = GetAddressEntry( standard_output.c_str(), "Care-of address:", &addr );
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  aCareofAddress->s_addr = addr;
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentGetStatus
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentGetStatus( TMobileNodeAgentStatusInfo *aMobileNodeAgentInfo )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;
  char buff[MAXCOMMANDLINE];
  // check params
  assert( aMobileNodeAgentInfo != NULL );
  
  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "status", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output 
  res |= GetStringEntry( standard_output.c_str(), "state", buff, MAXCOMMANDLINE );
  res |= CheckMatchAsBoolean( buff, "Connected", &(aMobileNodeAgentInfo->iConnected) );
  res |= GetStringEntry( standard_output.c_str(), "tunnel is", buff, MAXCOMMANDLINE );
  res |= CheckMatchAsBoolean( buff, "up", &(aMobileNodeAgentInfo->iTunnelUp) );
  res |= GetAddressEntry( standard_output.c_str(), "local addr", &(aMobileNodeAgentInfo->iLocalAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "co-addr", &(aMobileNodeAgentInfo->iColocatedAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "FA-addr", &(aMobileNodeAgentInfo->iForeignAgentAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "HA-addr", &(aMobileNodeAgentInfo->iHomeAgentAddress) );
  res |= GetAddressEntry( standard_output.c_str(), "Home addr", &(aMobileNodeAgentInfo->iHomeAddress) );
  res |= GetStringEntry( standard_output.c_str(), "info text", aMobileNodeAgentInfo->iInfoText, MAXINFOLEN );
  res |= GetStringEntry( standard_output.c_str(), "tunneling mode", buff, MAXCOMMANDLINE );
  res |= ConvertTunnelStringToInteger( buff, &(aMobileNodeAgentInfo->iTunnelMode) );

  // the following are optional - they only appear in the output when we have a connection
  GetStringEntry( standard_output.c_str(), "last warning", aMobileNodeAgentInfo->iLastErrorString, MAXERRORSTRING );
  GetIntegerEntry( standard_output.c_str(), "reply code", &(aMobileNodeAgentInfo->iReplyCode) );
  GetIntegerEntry( standard_output.c_str(), "lifetime left", &(aMobileNodeAgentInfo->iLifetimeRemaining) );
  GetIntegerEntry( standard_output.c_str(), "last request", &(aMobileNodeAgentInfo->iSecondsSinceLastRequest) );
  GetIntegerEntry( standard_output.c_str(), "last reply", &(aMobileNodeAgentInfo->iSecondsSinceLastReply) );
  
  // check for errors
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  
  // done
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentConnect
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentConnect()
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int match;

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "connect", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // check output
  match = strncmp( MOBILE_NODE_AGENT_CONNECT_PREFIX, standard_output.c_str(), strlen(MOBILE_NODE_AGENT_CONNECT_PREFIX) );
  if( match != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
    STRNCPY_NULL_TERMINATE( rv.iDynamicsErrorString, standard_output.c_str(), MAXIMUMERRORSTRINGLENGTH );
  }
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentDisconnect
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentDisconnect()
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int match;

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "disconnect", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // check output
  match = strncmp( MOBILE_NODE_AGENT_DISCONNECT_PREFIX, standard_output.c_str(), strlen(MOBILE_NODE_AGENT_DISCONNECT_PREFIX) );
  if( match != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentConfirm
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentConfirm()
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int match;

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "confirm", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // check output
  match = strncmp( MOBILE_NODE_AGENT_CONFIRM_PREFIX, standard_output.c_str(), strlen(MOBILE_NODE_AGENT_CONFIRM_PREFIX) );
  if( match != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentRescan
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentRescan()
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "rescan", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentSetPolicy
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentSetPolicy( int aPolicyVector )
{
  TDynamicsCallInfo rv, res;
  string standard_output, standard_error;
  char params[MAXCOMMANDLINE];
  int i, match;

  // init the rv to success
  memset( &rv, 0, sizeof(rv) );
  rv.iResult = DC_SUCCESS;  

  // we can only set one policy at a time - so set each time
  for( i = 0; iPolicyString[i] != NULL; i++ ) { 

    // set the parameter string
    snprintf( params, MAXCOMMANDLINE, " %s %s", iPolicyString[i], ((aPolicyVector&(1<<i)) ? "on" : "off") );

    // run the dynamics command line
    res = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "policy", params, &standard_output, &standard_error );

    // check output
    match = strncmp( MOBILE_NODE_AGENT_SETPOLICY_PREFIX, standard_output.c_str(), strlen(MOBILE_NODE_AGENT_SETPOLICY_PREFIX) );
    if( match != 0 ) {
      res.iResult = DC_UNEXPECTED_OUTPUT;
    }

    // return the first error
    if( (res.iResult != DC_SUCCESS) && (rv.iResult == DC_SUCCESS) ) {
      rv = res;
      rv.iErrorDetail = i;
    }
  }

  // done
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentGetPolicy
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentGetPolicy( int *aPolicyVector )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res, on, i;

  // check the params
  assert( aPolicyVector != NULL );
  *aPolicyVector = 0;
  memset( &rv, 0, sizeof(rv) );
  
  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "policy", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output
  for( i = 0; iPolicyString[i] != NULL; i++ ) {
    res = GetPolicyEntry( standard_output.c_str(), iPolicyString[i], &on );
    if( res != 0 ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      return rv;
    }
    if( on != 0 ) {
      (*aPolicyVector) |= (1<<i);
    }
  } 

  // done
  rv.iResult = DC_SUCCESS;
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentGetForeignAgentList
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentGetForeignAgentList( int *aListCount, 
									TMobileNodeForeignAgentInfo *aForeignAgentInfoList )
{
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int list_size;

  // check params 
  assert( aListCount != NULL );
  assert( aForeignAgentInfoList != NULL );
  list_size = *aListCount;
  *aListCount = 0;

  // init the list
  memset( aForeignAgentInfoList, 0, (sizeof(*aForeignAgentInfoList)*list_size) );

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "list", NULL, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output
  rv = ParseMobileNodeAgentListOutput( standard_output, list_size, aForeignAgentInfoList, aListCount );
  return rv;
}


/****************************************************************************************
 * 
 * PUBLIC METHOD: MobileNodeAgentGetForeignAgentInfo
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::MobileNodeAgentGetForeignAgentInfo( unsigned int aFAAddress, 
									TMobileNodeForeignAgentInfo *aForeignAgentInfo )
{ 
  TDynamicsCallInfo rv;
  string standard_output, standard_error;
  int res = 0;
  char params[MAXCOMMANDLINE];
  struct in_addr foreign_agent_address;

  // check params
  assert( aForeignAgentInfo != NULL );
  memset( aForeignAgentInfo, 0, sizeof(*aForeignAgentInfo) );

  // write the params
  foreign_agent_address.ADDRESS_INTEGER = aFAAddress;
  snprintf( params, MAXCOMMANDLINE, " %s", inet_ntoa(foreign_agent_address) );

  // run the dynamics command line 
  rv = DynamicsCall( MOBILE_NODE_AGENT_TOOL, "show", params, &standard_output, &standard_error );
  if( rv.iResult != DC_SUCCESS ) {
    return rv;
  }

  // parse the output 
  res |= GetAddressEntry( standard_output.c_str(), "FA", &(aForeignAgentInfo->iForeignAgentAddress) );
  res |= GetStringEntry( standard_output.c_str(), "Interface", aForeignAgentInfo->iInterfaceName, MAXINTERFACENAME );
  res |= GetIntegerEntry( standard_output.c_str(), "Priority", &(aForeignAgentInfo->iPriority) );
  aForeignAgentInfo->iInUse = 1;
  res |= GetIntegerEntry( standard_output.c_str(), "Interface index", &(aForeignAgentInfo->iInterfaceIndex) );
  res |= GetIntegerEntry( standard_output.c_str(), "Last adv.", &(aForeignAgentInfo->iInterfaceIndex) );
  res |= GetStringEntry( standard_output.c_str(), "NAI", aForeignAgentInfo->iNAI, MAXNAILEN );

  
  // check for errors
  if( res != 0 ) {
    rv.iResult = DC_UNEXPECTED_OUTPUT;
  }
  
  // done
  return rv;

}


/****************************************************************************************
 * 
 * SECTION: HELPERS
 * 
 ***************************************************************************************/

/****************************************************************************************
 * 
 * PRIVATE METHOD: GetDateEntry
 * 
 ***************************************************************************************/
int CDynamicsCommand::GetDateEntry( const char *aOutput, const char *aLabel, int *aValue )
{
  char *ptr;
  struct tm stime;
  time_t ttime;

  // check params
  assert( aOutput != NULL );
  assert( aLabel != NULL );
  assert( aValue != NULL );

  // set default
  *aValue = 0;

  // look for the label in the output
  ptr = strstr( aOutput, aLabel );
  if( ptr == NULL ) {
    return -1;
  }

  // skip over any spaces
  ptr += strlen(aLabel);
  for( ; IS_WHITESPACE(*ptr); ptr++ ) 
    ;

  // convert the string address into an int
  strptime( ptr, "%A %B %d %H:%M:%S %Y", &stime );
  ttime = mktime( &stime );
  *aValue = ttime;
  return 0;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: GetAddressEntry
 * 
 ***************************************************************************************/
int CDynamicsCommand::GetAddressEntry( const char *aOutput, const char *aLabel, unsigned int *aValue )
{
  int err;
  char *ptr;
  struct in_addr iaddr;

  // check params
  assert( aOutput != NULL );
  assert( aLabel != NULL );
  assert( aValue != NULL );

  // set default
  *aValue = 0;

  // look for the label in the output
  ptr = strstr( aOutput, aLabel );
  if( ptr == NULL ) {
    return -1;
  }

  // skip over any spaces
  ptr += strlen(aLabel);
  for( ; IS_WHITESPACE(*ptr); ptr++ ) 
    ;

  // if the current char is no a digit then there is a problem
  if( !isdigit(*ptr) ) {
    return -1;
  }

  // convert the string address into an in_addr 
  err = inet_aton( ptr, &iaddr );
  if( err == 0 ) {
    return -1;
  }
  *aValue = iaddr.ADDRESS_INTEGER;
  return 0;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: GetIntegerEntry
 * 
 ***************************************************************************************/
int CDynamicsCommand::GetIntegerEntry( const char *aOutput, const char *aLabel, int *aValue )
{
  char *ptr;

  // check params
  assert( aOutput != NULL );
  assert( aLabel != NULL );
  assert( aValue != NULL );

  // set the default value
  *aValue = 0;

  // look for the label in the output
  ptr = strstr( aOutput, aLabel );
  if( ptr == NULL ) {
    return -1;
  }

  // skip over any spaces
  ptr += strlen(aLabel);
  for( ; IS_WHITESPACE(*ptr); ptr++ ) 
    ;

  // if the current char is no a digit then there is a problem
  if( (!isdigit(*ptr)) && ((*ptr) != '-') ) {
    return -1;
  }

  // otherwise return the integer
  *aValue = atoi(ptr);
  return 0;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: GetPolicyEntry
 * 
 ***************************************************************************************/
int CDynamicsCommand::GetPolicyEntry( const char *aOutput, const char *aLabel, int *aValue )
{
  char *ptr;

  // check params
  assert( aOutput != NULL );
  assert( aLabel != NULL );
  assert( aValue != NULL );

  // set the default
  *aValue = 0;

  // look for the label in the output
  ptr = strstr( aOutput, aLabel );
  if( ptr == NULL ) {
    return -1;
  }

  // skip over any spaces
  ptr += strlen(aLabel);
  for( ; IS_WHITESPACE(*ptr); ptr++ ) 
    ;

  // if the string is not 'ON' or 'OFF' then we are confused
  if( (ptr[0] != 'O') || ((ptr[1] != 'N') && (ptr[1] != 'F')) ) {
    return -1;
  }

  // this should either be OFF => O or ON => 1
  *aValue = ((ptr[1] == 'N') ? 1 : 0);
  return 0;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: GetStringEntry
 * 
 ***************************************************************************************/
int CDynamicsCommand::GetStringEntry( const char *aOutput, const char *aLabel, char *aValue, int aBufferSize  )
{
  char *ptr, *start, *end;
  int string_length;

  // check params
  assert( aOutput != NULL );
  assert( aLabel != NULL );
  assert( aValue != NULL );
  assert( aBufferSize > 0 );

  // set the default
  aValue[0] = 0;

  // look for the label in the output
  ptr = strstr( aOutput, aLabel );
  if( ptr == NULL ) {
    return -1;
  }

  // skip over any spaces
  ptr += strlen(aLabel);
  for( ; IS_WHITESPACE(*ptr); ptr++ ) 
    ;

  // get the length of the substr to copy - don't copy the newline
  start = ptr;
  end = strchr( start, '\n' );
  if( end == NULL ) {
    end = strchr( start, 0 );
  }
  string_length = end - start;
  string_length = MIN( string_length, (aBufferSize-1) );
  
  // now copy the string and return
  memcpy( aValue, start, string_length );
  aValue[string_length] = 0;
  return 0;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: ParseForeignAgentListOutput: Parse ((IP_ADDR + SPACE + IP_ADDR + SPACE + INTEGER + '\n')* + '\n')
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ParseForeignAgentListOutput( string aOutput, int aListSize, TTunnelID *aTunnelIDArray, int *aListCount )
{
  TDynamicsCallInfo rv;
  const char *cline, *mobile_node_address_str, *home_agent_address_str, *tunnel_id_str;
  int tunnel_id, err;
  struct in_addr mobile_node_address, home_agent_address;

  // intialise the result
  memset( &rv, 0, sizeof(rv) );
  rv.iResult = DC_SUCCESS;

  // parse a line at a time
  cline = aOutput.c_str();
  while( 1 ) {
    
    // check for end of output 
    if( (cline == NULL) || (*cline == 0) || (*cline == '\n') ) {
      break;
    }

    // check for full output buffer
    if( (*aListCount) == aListSize ) {
      break;
    }

    // otherwise go and find the tokens
    mobile_node_address_str = cline;
    home_agent_address_str = strchr( mobile_node_address_str, ' ' );
    if( home_agent_address_str == NULL ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      break;
    }
    home_agent_address_str++;
    tunnel_id_str = strchr( home_agent_address_str, ' ' );
    if( tunnel_id_str == NULL ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      break;
    }
    tunnel_id_str++;

    // try and convert these into the correct format
    err = inet_aton( mobile_node_address_str, &mobile_node_address );
    if( err == 0 ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      break;
    }
    err = inet_aton( home_agent_address_str, &home_agent_address );
    if( err == 0 ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      break;
    }
    tunnel_id = atoi( tunnel_id_str );

    // now place these into the output array
    (aTunnelIDArray[*aListCount]).iMobileNodeAddress = mobile_node_address.ADDRESS_INTEGER;
    (aTunnelIDArray[*aListCount]).iHomeAgentAddress = home_agent_address.ADDRESS_INTEGER;
    (aTunnelIDArray[*aListCount]).iTunnelID = tunnel_id;
    (*aListCount) += 1;

    // finally, update the 
    cline = strchr( cline, '\n' );
    if( cline == NULL ) {
      break;
    }
    cline++;
  }
    
  // done 
  return rv;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: ParseHomeAgentListOutput: Parse: (X tunnels:\n(IP_ADDR+\n)*\n)
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ParseHomeAgentListOutput( string aOutput, int aListSize, unsigned int *aTunnelList, int *aListCount )
{
  TDynamicsCallInfo rv;
  const char *cline;
  int err, i;
  struct in_addr saddr;

  // check params
  assert( aListCount != NULL );
  assert( aTunnelList != NULL );
  *aListCount = 0;

  // initialise the result
  memset( &rv, 0, sizeof(rv) );
  rv.iResult = DC_SUCCESS;

  // now parse each line
  for( cline = aOutput.c_str(), i = 0; i < aListSize; i++ ) {

    // if this line starts with a newline then we are at the end
    if( (cline == NULL) || (*cline == 0) || (*cline == '\n') ) {
      break;
    }
    
    // get the IP address of the mobile node
    err = inet_aton( cline, &saddr );
    if( err == 0 ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      break;
    }
    
    // store this in the list
    aTunnelList[(*aListCount)] = saddr.s_addr;
    (*aListCount)++;

    // now move to the next line
    cline = strchr( cline, '\n' );
    if( cline == NULL ) {
      break;
    }
    cline++;
  }
    
    
  // done 
  return rv;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: ParseMobileNodeAgentListOutput: 
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::ParseMobileNodeAgentListOutput( string aOutput, int aListSize, 
								    TMobileNodeForeignAgentInfo *aForeignAgentList, int *aListCount )
{
  TDynamicsCallInfo rv;
  const char *cline;
  int match, priority, in_use_flag, err;
  struct in_addr saddr;
  char foreign_address_string[TOKENSIZE], interface_name_string[TOKENSIZE], white_space[TOKENSIZE];

  // check params
  assert( aForeignAgentList != NULL );
  assert( aListCount != NULL );
  memset( &rv, 0, sizeof(rv) );
  rv.iResult = DC_SUCCESS;
  *aListCount = 0;

  // the first line should be "List of heard mobility agents:\n"
  cline = aOutput.c_str();
  assert( cline != NULL );
  match = strncmp( MOBILE_AGENT_FOREIGN_AGENT_LIST_PREFIX, cline, strlen(MOBILE_AGENT_FOREIGN_AGENT_LIST_PREFIX) );
  if( match != 0 ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      return rv;
  }

  // now process each line 
  while( 1 ) {
    
    // move to the next line
    cline = strchr( cline, '\n' );
    cline++;
    
    // if are at the end of input then we are done
    if( *cline == 0 ) {
      break;
    }

    // if the list is full then we are done
    if( (*aListCount) >= aListSize ) {
      break;
    }
    
    // parse the line: IP_ADDR + WHITESPACE + INTERFACENAME + WHITESPACE + prio + 100 + ...
    err = sscanf( cline, "%s%[ \t]%s%[ \t]prio%[ \t]%d", foreign_address_string, white_space, interface_name_string, 
		  white_space, white_space, &priority );
    if( err != 6 ) {
      rv.iResult = DC_UNEXPECTED_OUTPUT;
      break;
    }

    // check for flags in the line - IN-USE
    in_use_flag = strstrbeforedelim( cline, "IN-USE", '\n' );

    // save these values in the list making appropriate conversions
    inet_aton( foreign_address_string, &saddr );
    aForeignAgentList[(*aListCount)].iForeignAgentAddress = saddr.ADDRESS_INTEGER;
    STRNCPY_NULL_TERMINATE( aForeignAgentList[(*aListCount)].iInterfaceName, interface_name_string, MAXINTERFACENAME );
    aForeignAgentList[(*aListCount)].iInUse = ((in_use_flag == 0) ? 1 : 0);
    aForeignAgentList[(*aListCount)].iPriority = priority;

    // fill in the fields not got here
    aForeignAgentList[(*aListCount)].iInterfaceIndex = 0;
    aForeignAgentList[(*aListCount)].iLastAdvertisement = 0;
    aForeignAgentList[(*aListCount)].iNAI[0] = 0;

    // update the list count
    (*aListCount) += 1;
  }      
    
  // done 
  return rv;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: DynamicsCall
 * 
 ***************************************************************************************/
TDynamicsCallInfo CDynamicsCommand::DynamicsCall( char *aExecutable, char *aCommand, char *aParams, 
						  string *aStandardOutput, string *aStandardError )
{
  int command_length, match, errcode;
  TDynamicsCallInfo rv;
  CAProcess proc;
  TCAProcessError perr;
  char dynamics_error_string[MAXIMUMERRORSTRINGLENGTH], *ptr, *submatch;

	// check params
	assert( aExecutable != NULL );
	assert( aCommand != NULL );
	assert( aStandardOutput != NULL );
	assert( aStandardError != NULL );

	// init the rv
	memset( &rv, 0, sizeof(rv) );

	// create the command line for this call
	snprintf( iCommandLine, MAXCOMMANDLINE, "%s %s", aExecutable, aCommand );

	// append the params if set
	if( aParams != NULL ) {
		command_length = strlen( iCommandLine );
		snprintf( &(iCommandLine[command_length]), (MAXCOMMANDLINE - command_length), " %s", aParams );
	}

	// append the config file if set
	if( iPipePath.length() != 0 ) {
		command_length = strlen( iCommandLine );
		snprintf( &(iCommandLine[command_length]), (MAXCOMMANDLINE - command_length), " -p %s", iPipePath.c_str() );
	}

	// now run the command line and trap the output
	//	fprintf( stderr, "COMMANDLINE: '%s'\n", iCommandLine );
	perr = proc.Execute( iCommandLine, &errcode, iTimeout, aStandardOutput, aStandardError );
	if( perr != CAE_NONE ) {
		rv.iResult = DC_EXECUTE_FAILED;
		rv.iErrorCode = (int)perr;
		rv.iErrorDetail = errcode;
		return rv;
	}

	// see if there was a 'csh:' error
	match = strncmp( CSH_FAILED_PREFIX, aStandardError->c_str(), strlen(CSH_FAILED_PREFIX) );
	if( match == 0 ) {
	  submatch = strstr( aStandardError->c_str(), TARGET_NOT_FOUND );
	  rv.iResult = ((submatch == NULL) ? DC_CSH_FAILED: DC_COMMAND_TOOL_NOT_FOUND);
	  rv.iErrorCode = 0;
	  rv.iErrorDetail = 0;
	  if( rv.iResult == DC_CSH_FAILED ) {
	    STRNCPY_NULL_TERMINATE( dynamics_error_string, aStandardError->c_str(), MAXIMUMERRORSTRINGLENGTH );
	    ptr = &(dynamics_error_string[strlen(CSH_FAILED_PREFIX)]);
	    STRNCPY_NULL_TERMINATE( rv.iDynamicsErrorString, ptr, MAXIMUMERRORSTRINGLENGTH );
	  }
	  return rv;
	}

	// see if there was a 'Call failed: ' error
	match = strncmp( CALL_FAILED_PREFIX, aStandardOutput->c_str(), strlen(CALL_FAILED_PREFIX) );
	if( match == 0 ) {
		rv.iResult = DC_CALL_FAILED;
		rv.iErrorCode = GetCallFailedReason( aStandardOutput->c_str() );
		rv.iErrorDetail = 0;
		if( rv.iErrorCode == DCF_UNKNOWN ) {
		  STRNCPY_NULL_TERMINATE( dynamics_error_string, aStandardOutput->c_str(), MAXIMUMERRORSTRINGLENGTH );
		  ptr = &(dynamics_error_string[strlen(CALL_FAILED_PREFIX)]);
		  STRNCPY_NULL_TERMINATE( rv.iDynamicsErrorString, ptr, MAXIMUMERRORSTRINGLENGTH );
		} else {
		  rv.iDynamicsErrorString[0] = 0;
		}
		return rv;
	}

	// see if there was an 'Invalid arguments. ' error
	match = strncmp( INVALID_ARGUMENTS_PREFIX, aStandardOutput->c_str(), strlen(INVALID_ARGUMENTS_PREFIX) );
	if( match == 0 ) {
		rv.iResult = DC_INVALID_ARGUMENTS;
		rv.iErrorCode = 0;
		rv.iErrorDetail = 0;
		STRNCPY_NULL_TERMINATE( dynamics_error_string, aStandardOutput->c_str(), MAXIMUMERRORSTRINGLENGTH );
		ptr = &(dynamics_error_string[strlen(INVALID_ARGUMENTS_PREFIX)]);
		STRNCPY_NULL_TERMINATE( rv.iDynamicsErrorString, ptr, strlen(INVALID_ARGUMENTS_PREFIX) );
		return rv;
	}
	
	// otherwise everything looks ok - return the output for the upper layer to translate
	rv.iResult = DC_SUCCESS;
	rv.iErrorCode = 0;
	rv.iErrorDetail = 0;
	(rv.iDynamicsErrorString)[0] = 0;
	return rv;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: GetCallFailedReason
 * 
 ***************************************************************************************/
TDynamicsCallFailedReason CDynamicsCommand::GetCallFailedReason( const char *aOutput )
{
  int match;
  const char *err_string;

  // validate that this is a call failed 
  match = strncmp( CALL_FAILED_PREFIX, aOutput, strlen(CALL_FAILED_PREFIX) );
  assert( match == 0 );

  // get the error string
  err_string = &(aOutput[strlen(CALL_FAILED_PREFIX)]);

  // check for 'function not permitted'
  match = strncmp( CALL_FAILED_FUNCTION_NOT_PERMITTED, err_string, strlen(CALL_FAILED_FUNCTION_NOT_PERMITTED) );
  if( match == 0 ) {
    return DCF_FUNCTION_NOT_PERMITTED;
  }

  // check for 'agent unreachable'
  match = strncmp( CALL_FAILED_AGENT_UNREACHABLE, err_string, strlen(CALL_FAILED_AGENT_UNREACHABLE) );
  if( match == 0 ) {
    return DCF_AGENT_UNREACHABLE;
  }

  // can't interpret
  return DCF_UNKNOWN;
}



/****************************************************************************************
 * 
 * PRIVATE METHOD: ConvertTunnelStringToInteger
 * 
 ***************************************************************************************/
int CDynamicsCommand::ConvertTunnelStringToInteger( char *aTunnelMode, int *aValue )
{
  int rv = -1, i, match;

  // check params
  assert( aTunnelMode != NULL );
  assert( aValue != NULL );
  *aValue = -1;

  // look for a match
  for( i = 0; iTunnelModeString[i] != NULL; i++ ) {
    match = strncmp( iTunnelModeString[i], aTunnelMode, strlen(iTunnelModeString[i]) );
    if( match == 0 ) {
      *aValue = i;
      rv = 0;
    }
  }

  // done
  return rv;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: CheckMatchAsBoolean
 * 
 ***************************************************************************************/
int CDynamicsCommand::CheckMatchAsBoolean( char *aInput, char *aTrue, int *aValue )
{
  int match;

  // check params
  assert( aInput != NULL );
  assert( aTrue != NULL );
  assert( aValue != NULL );

  // check match
  match = strcmp( aInput, aTrue );
  *aValue = ((match == 0) ? 1 : 0);
  return 0;
}


/****************************************************************************************
 * 
 * PRIVATE METHOD: strstrbeforedelim
 * 
 ***************************************************************************************/
int CDynamicsCommand::strstrbeforedelim( const char *aSearchSpace, const char *aToken, char aDelim )
{
  char *next_delim_position, *next_substr_position;
 
  // check params
  assert( aSearchSpace != NULL );
  assert( aToken != NULL );
  
  // look for the substr - if not found then not found
  next_substr_position = strstr( aSearchSpace, aToken );
  if( next_substr_position == NULL ) {
    return -1;
  }

  // look for the delim - if not found then the substr was found
  next_delim_position = strchr( aSearchSpace, aDelim );
  if( next_delim_position == NULL ) {
    return 0;
  }

  // see which one came first
  if( next_substr_position < next_delim_position ) {
    return 0;
  }
  return -1;
}

 

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




#include <assert.h>
#include <time.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "strncpynt.h"
#include "CHomeAgentServiceStub.h"
#include "CCHacontroller.h"
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAXBUFFERSIZE	2048


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/
																		
/*******************************************************************************
 *
 * Constructor / Destructor
 *
 ******************************************************************************/
CHomeAgentServiceStub::CHomeAgentServiceStub()
{
	iClientHomeAgent = new CCHacontroller();
	assert( iClientHomeAgent != NULL );
}


CHomeAgentServiceStub::~CHomeAgentServiceStub()
{
	delete iClientHomeAgent;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CHomeAgentServiceStub::StartUccsService( char *aHostName, int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iClientHomeAgent->Connect( aHostName );

	// return the result
	return client_stub_error;
}


int CHomeAgentServiceStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iClientHomeAgent->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CHomeAgentServiceStub::IssueCommand( CDataRecord* aRequest )
{
	int i, err, client_stub_error, method_id = -1, virtual_network_address;
	char *tmp_string;
	CDataRecord *request_reply;

	TStartupInfo arg_startup_info;
	int sarg_integer;
	TOptionDesc sarg_option;
	THaTunnelID sarg_tunnel_id;
	TTimeoutRequest sarg_timeout;
	TGetTunnelRequest sarg_get_tunnel;

	THaStatus rv_hastatus;
	TResult rv_result;
	TComponentList rv_component_list;
	int rv_integer;
	THaTunnelList rv_tunnels;
	THaTunnelInfo rv_tunnel_info;
 	
	// check params
	assert( aRequest != NULL );

	// create a standard reply 
	request_reply = CreateBaseReply( aRequest );
	assert( request_reply != NULL );

	// get and check the method_id
	err = request_reply->GetFieldAsInt( "METHODID", &method_id );
	if( method_id == -1 ) {
		UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
		return request_reply;
	}

	// now dispatch and call the appropriate method
	switch( method_id ) {

	// ss_startuprpcservice
	case 1:

		// extract the parameters
		GETINTEGERARGUMENT( "BASEINTERFACE", &(arg_startup_info.iBaseInterfaceIndex), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "BASENETMASK", &(arg_startup_info.iBaseInterfaceNetworkMask), 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "BASESTARTRANGE", &(arg_startup_info.iBaseInterfaceStartHostAddressRange), 3, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "BASEENDRANGE", &(arg_startup_info.iBaseInterfaceStopHostAddressRange), 4, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "VIRTUALNETWORKPREFIX", &tmp_string, 5, 0, aRequest, request_reply );
		arg_startup_info.iVirtualNetworkPrefix = inet_addr( tmp_string );
		GETINTEGERARGUMENT( "VIRTUALNETWORKMASK", &(arg_startup_info.iVirtualNetworkPrefixMask), 6, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "VIRTUALNETWORKSIZE", &(arg_startup_info.iVirtualNetworkAllocSize), 7, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->ss_startuprpcservice( arg_startup_info, &rv_integer );
		UpdateCompletionCode( request_reply, client_stub_error );		
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set any return information
		request_reply->NewField( "RESULT", rv_integer );
		break;
	

	// sc_shutdownrpcservice
	case 2:
				
		// extract the parameters
		GETINTEGERARGUMENT( "FORCE", &sarg_integer, 1, 1, aRequest, request_reply );
		
		// make the call and update the return value
		client_stub_error = iClientHomeAgent->sc_shutdownrpcservice( sarg_integer, &rv_integer );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;


	// list_devices
	case 30:

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->list_devices( &rv_component_list );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "AGENTCOUNT", rv_component_list.TComponentList_len );
		for( i = 0; i < rv_component_list.TComponentList_len; i++ ) {
			AddIteratedIntegerFieldName( "AGENTID", i, (rv_component_list.TComponentList_val)[i], request_reply );
		}
		break;		


	// cstr_createagent
	case 31:

		//make the call and update the return value
		client_stub_error = iClientHomeAgent->cstr_createagent( &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// dstr_removeagent
	case 32:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientHomeAgent->dstr_removeagent( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// startmobileagent
	case 3:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientHomeAgent->startmobileagent( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// stopmobileagent
	case 4:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientHomeAgent->stopmobileagent( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// getmobileagentstatus
	case 5:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->getmobileagentstatus( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "HOME_AGENT_PROCESS_STATUS", rv_result.iData0 );
		break;


	// setsingleoption
	case 6:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_option.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "TOKEN", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionToken, tmp_string, X_MAXOPTIONTOKENSIZE );
		GETSTRINGARGUMENT( "VALUE", &tmp_string, 3, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionValue, tmp_string, X_MAXOPTIONTOKENSIZE );
		sarg_option.iOptionBlockStart[0] = sarg_option.iOptionBlockEnd[0] = 0;

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->setsingleoption( sarg_option, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// removesingleoption
	case 33:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_option.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "TOKEN", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionToken, tmp_string, X_MAXOPTIONTOKENSIZE );
		sarg_option.iOptionValue[0] = sarg_option.iOptionBlockStart[0] = sarg_option.iOptionBlockEnd[0] = 0;

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->removesingleoption( sarg_option, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// addlistoption
	case 7:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_option.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "VALUE", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionValue, tmp_string, X_MAXOPTIONTOKENSIZE );
		GETSTRINGARGUMENT( "BLOCKSTART", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionBlockStart, tmp_string, X_MAXOPTIONTOKENSIZE );
		GETSTRINGARGUMENT( "BLOCKEND", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionBlockEnd, tmp_string, X_MAXOPTIONTOKENSIZE );
		sarg_option.iOptionToken[0] = 0;
	
		// make the call and update the return value
		client_stub_error = iClientHomeAgent->addlistoption( sarg_option, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// removelistoption
	case 8:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_option.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "TOKEN", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionToken, tmp_string, X_MAXOPTIONTOKENSIZE );
		GETSTRINGARGUMENT( "BLOCKSTART", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionBlockStart, tmp_string, X_MAXOPTIONTOKENSIZE );
		GETSTRINGARGUMENT( "BLOCKEND", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_option.iOptionBlockEnd, tmp_string, X_MAXOPTIONTOKENSIZE );
		sarg_option.iOptionValue[0] = 0;
	
		// make the call and update the return value
		client_stub_error = iClientHomeAgent->removelistoption( sarg_option, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// getstatus
	case 9:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->getstatus( sarg_integer, &rv_hastatus ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_hastatus.iCallResult.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_hastatus.iCallResult.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_hastatus.iCallResult.iData0 );
		request_reply->NewField( "RESULT_DATA1", rv_hastatus.iCallResult.iData1 );
		request_reply->NewField( "STATUS_HOME_AGENT_ADDRESS", IntToAddress(rv_hastatus.iHomeAgentAddress) );
		request_reply->NewField( "STATUS_VIRTUAL_NETWORK_SIZE", rv_hastatus.iVirtualNetworkSize );
		for( i = 0; i < rv_hastatus.iVirtualNetworkSize; i++ ) {
			virtual_network_address = ntohl( rv_hastatus.iVirtualNetworkStartAddress );
			virtual_network_address |= i;
			virtual_network_address = htonl( virtual_network_address );
			AddIteratedStringFieldName( "VIRTUAL_NETWORK_ADDRESS", i, IntToAddress(virtual_network_address), request_reply );
		}
		request_reply->NewField( "STATUS_TUNNEL_COUNT", rv_hastatus.iTunnelCount );
		request_reply->NewField( "STATUS_REQUESTS_ACCEPTED", rv_hastatus.iRequestsAccepted );
		request_reply->NewField( "STATUS_REQUESTS_REJECTED", rv_hastatus.iRequestsRejected );
		request_reply->NewField( "STATUS_DISCARDED_MALFORMED", rv_hastatus.iDiscardedMalformed );
		request_reply->NewField( "STATUS_DISCARDED_UNKNOWN_EXTENSION", rv_hastatus.iDiscardedUnknownExtension );
		request_reply->NewField( "STATUS_DISCARDED_VENDOR", rv_hastatus.iDiscardedVendor );
		request_reply->NewField( "STATUS_ADVERTISEMENTS_SENT", rv_hastatus.iAdvertisementsSent );


		break;
		

	// destroytunnelid
	case 11:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_tunnel_id.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "MOBILE_NODE_ADDRESS", &tmp_string, 2, 0, aRequest, request_reply );
		sarg_tunnel_id.iMobileNodeAddress = AddressToInt( tmp_string );

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->destroytunnelid( sarg_tunnel_id, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		request_reply->NewField( "RESULT_DATA1", rv_result.iData1 );
		break;


	// listtunnels
	case 12:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->listtunnels( sarg_integer, &rv_tunnels ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "TUNNEL_COUNT", rv_tunnels.THaTunnelList_len );
		for( i = 0; i < (signed)(rv_tunnels.THaTunnelList_len); i++ ) {
			AddIteratedIntegerFieldName( "TUNNEL_MOBILE_NODE_ADDRESS", i, ((rv_tunnels.THaTunnelList_val)[i]).iMobileNodeAddress, request_reply );
		}		

		// free the memory (if any was allocated)
		if( rv_tunnels.THaTunnelList_len > 0 ) {
			XdrFree( (char**)&(rv_tunnels.THaTunnelList_val), (int*)&(rv_tunnels.THaTunnelList_len) );
		}
		break;


	// gettunnelinfo
	case 14:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_get_tunnel.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "MOBILE_NODE_HOME_ADDRESS", &tmp_string, 2, 0, aRequest, request_reply );
		sarg_get_tunnel.iMobileNodeAddress = AddressToInt( tmp_string );

		// make the call and update the return value
		client_stub_error = iClientHomeAgent->gettunnelinfo( sarg_get_tunnel, &rv_tunnel_info ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_tunnel_info.iCallResult.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_tunnel_info.iCallResult.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_tunnel_info.iCallResult.iData0 );
		request_reply->NewField( "RESULT_DATA1", rv_tunnel_info.iCallResult.iData1 );
		request_reply->NewField( "MOBILE_NODE_ADDRESS", IntToAddress(rv_tunnel_info.iMobileNodeAddress) );
		request_reply->NewField( "CAREOF_ADDRESS", IntToAddress(rv_tunnel_info.iCareofAddress) );
		request_reply->NewField( "HOME_AGENT_ADDRESS", IntToAddress(rv_tunnel_info.iHomeAgentAddress) );
		request_reply->NewField( "CREATION_TIME", ctime((time_t*)&(rv_tunnel_info.iCreationTime)) );
		request_reply->NewField( "EXPIRATION_TIME", ctime((time_t*)&(rv_tunnel_info.iExpirationTime)) );
		request_reply->NewField( "REFRESH_TIME", ctime((time_t*)&(rv_tunnel_info.iRefreshTime)) );
		request_reply->NewField( "LAST_TIMESTAMP", ctime((time_t*)&(rv_tunnel_info.iLastTimestamp)) );
		request_reply->NewField( "SPI", rv_tunnel_info.iSPI );
		request_reply->NewField( "TIMEOUT", rv_tunnel_info.iTimeout );
		break;


	// settimeout
	case 13:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_timeout.iAgentID), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "TIMEOUT", &(sarg_timeout.iTimeout), 2, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientHomeAgent->settimeout( sarg_timeout ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}
		break;


	// Any other method id results in an invalid method id result
	default:
		UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
		break;
	}

	// everything should be handled above 
	return request_reply;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CHomeAgentServiceStub::GetLastRPCError( int *aIntError )
{
	return iClientHomeAgent->GetLastRPCError( aIntError );
}


/*******************************************************************************
 *
 * GetStatus()
 *
 ******************************************************************************/
int CHomeAgentServiceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}

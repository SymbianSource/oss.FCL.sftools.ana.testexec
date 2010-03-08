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
#include "CForeignAgentServiceStub.h"
#include "CCFacontroller.h"
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
CForeignAgentServiceStub::CForeignAgentServiceStub()
{
	iClientForeignAgent = new CCFacontroller();
	assert( iClientForeignAgent != NULL );
}


CForeignAgentServiceStub::~CForeignAgentServiceStub()
{
	delete iClientForeignAgent;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CForeignAgentServiceStub::StartUccsService( char *aHostName, int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	//copy to class variable so that startRPCservice can use it later on
	iHostName = aHostName;

	// connect to the remote service
	client_stub_error = iClientForeignAgent->Connect( iHostName );

	// return the result
	return client_stub_error;
}


int CForeignAgentServiceStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iClientForeignAgent->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CForeignAgentServiceStub::IssueCommand( CDataRecord* aRequest )
{
	int i, err, client_stub_error, method_id = -1;
	CDataRecord *request_reply;
	char *tmp_string;

	int rv_integer, sarg_integer;
	TComponentList rv_component_list;
	TOptionDesc sarg_option;
	TResult rv_result;
	TStartupInfo sarg_startup_info;
	TFaStatusInfo rv_fastatus;
	TFaTunnelID sarg_tunnel_id;
	TFaTunnelList rv_tunnel_list;
	TTimeoutRequest sarg_timeout;
	TStartAgentRequest sarg_start_agent_request;
	TFaTunnelID *tmp_tunnel_id;
	TGetTunnelRequest sarg_get_tunnel;
	TFaTunnelInfo rv_tunnel_info;

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
		GETINTEGERARGUMENT( "BASEINTERFACEINDEX", &(sarg_startup_info.iBaseInterfaceIndex), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "NETMASK", &(sarg_startup_info.iNetworkMask), 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "STARTHOSTADDRESSRANGE", &(sarg_startup_info.iStartHostAddressRange), 3, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "STOPHOSTADDRESSRANGE", &(sarg_startup_info.iStopHostAddressRange), 4, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientForeignAgent->ss_startuprpcservice( sarg_startup_info, &rv_integer );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;

	// sc_shutdownrpcservice
	case 2:
			
		// extract the parameters
		GETINTEGERARGUMENT( "FORCE", &sarg_integer, 1, 1, aRequest, request_reply );
		
		// make the call and update the return value
		client_stub_error = iClientForeignAgent->sc_shutdownrpcservice( sarg_integer, &rv_integer );
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
		client_stub_error = iClientForeignAgent->list_devices( &rv_component_list );
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
		client_stub_error = iClientForeignAgent->cstr_createagent( &rv_result ); 
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
		client_stub_error = iClientForeignAgent->dstr_removeagent( sarg_integer, &rv_integer ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;

	// startmobileagent
	case 3:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_start_agent_request.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "INTERFACE", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_start_agent_request.iLowerInterface, tmp_string, X_MAXINTERFACENAMESIZE );
		GETINTEGERARGUMENT( "SOLICITATIONMODE", (int*)&(sarg_start_agent_request.iSolicitationMode), 3, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "INTERVAL", &(sarg_start_agent_request.iSolicitationInterval), 4, 1, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientForeignAgent->startmobileagent( sarg_start_agent_request, &rv_result ); 
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
		client_stub_error = iClientForeignAgent->stopmobileagent( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		break;

	// getmobileagentstatus
	case 5:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientForeignAgent->getmobileagentstatus( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "FOREIGN_AGENT_PROCESS_STATUS", rv_result.iData0 );
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
		client_stub_error = iClientForeignAgent->setsingleoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientForeignAgent->removesingleoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientForeignAgent->addlistoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientForeignAgent->removelistoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientForeignAgent->getstatus( sarg_integer, &rv_fastatus ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_fastatus.iCallResult.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_fastatus.iCallResult.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_fastatus.iCallResult.iData0 );
		request_reply->NewField( "RESULT_DATA1", rv_fastatus.iCallResult.iData1 );
		request_reply->NewField( "STATUS_ADVERTISEMENTS_SENT", rv_fastatus.iAdvertisementsSent );
		request_reply->NewField( "STATUS_DISCARDED_MALFORMED", rv_fastatus.iDiscardedMalformed );
		request_reply->NewField( "STATUS_DISCARDED_UNKNOWN_EXTENSION", rv_fastatus.iDiscardedUnknownExtension );
		request_reply->NewField( "STATUS_DISCARDED_VENDOR", rv_fastatus.iDiscardedVendor );
		request_reply->NewField( "STATUS_PENDING_REGISTRATION_REQUESTS", rv_fastatus.iPendingRegistrationRequests );
		request_reply->NewField( "STATUS_REPLYS_ACCEPTED", rv_fastatus.iReplysAccepted );
		request_reply->NewField( "STATUS_REPLYS_REJECTED", rv_fastatus.iReplysRejected );
		request_reply->NewField( "STATUS_REQUESTS_ACCEPTED", rv_fastatus.iRequestsAccepted );
		request_reply->NewField( "STATUS_REQUESTS_REJECTED", rv_fastatus.iRequestsRejected );
		request_reply->NewField( "STATUS_TUNNEL_COUNT", rv_fastatus.iTunnelCount );
		break;
		

	// destroytunnelid
	case 11:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_tunnel_id.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "MNADDR", &tmp_string, 2, 0, aRequest, request_reply );
		sarg_tunnel_id.iMobileNodeAddress = AddressToInt( tmp_string );

		// make the call
		client_stub_error = iClientForeignAgent->destroytunnelid( sarg_tunnel_id, &rv_result ); 
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

		//make the call and update the return value
		client_stub_error = iClientForeignAgent->listtunnels( sarg_integer, &rv_tunnel_list ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "TUNNEL_COUNT", rv_tunnel_list.TFaTunnelList_len );
		for( i = 0; i < (signed)(rv_tunnel_list.TFaTunnelList_len); i++ ) {
			tmp_tunnel_id = &((rv_tunnel_list.TFaTunnelList_val)[i]);
			AddIteratedIntegerFieldName( "TUNNEL_ID", i, tmp_tunnel_id->iID, request_reply );
			AddIteratedStringFieldName( "TUNNEL_MOBILE_NODE_HOME_ADDRESS", i, IntToAddress(tmp_tunnel_id->iMobileNodeAddress), request_reply );
			AddIteratedStringFieldName( "TUNNEL_HOME_AGENT_ADDRESS", i, IntToAddress(tmp_tunnel_id->iHomeAgentAddress), request_reply );
		}		

		// free the memory (if any was allocated)
		if( rv_tunnel_list.TFaTunnelList_len > 0 ) {
			XdrFree( (char**)&(rv_tunnel_list.TFaTunnelList_val), (int*)&(rv_tunnel_list.TFaTunnelList_len) );
		}
		break;

	// settimeout
	case 13:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_timeout.iAgentID), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "TIMEOUT", &(sarg_timeout.iTimeout), 2, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientForeignAgent->settimeout( sarg_timeout ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}
		break;

	// gettunnelinfo
	case 15:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_get_tunnel.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "MOBILE_NODE_HOME_ADDRESS", &tmp_string, 2, 0, aRequest, request_reply );
		sarg_get_tunnel.iMobileNodeAddress = AddressToInt( tmp_string );

		//make the call and update the return value
		client_stub_error = iClientForeignAgent->gettunnelinfo( sarg_get_tunnel, &rv_tunnel_info ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result
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
		request_reply->NewField( "HOME_AGENT_ID", rv_tunnel_info.iPrivateHomeAgentID );
		request_reply->NewField( "SPI", rv_tunnel_info.iSPI );
		request_reply->NewField( "TIMEOUT", rv_tunnel_info.iTimeout );
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
char *CForeignAgentServiceStub::GetLastRPCError( int *aIntError )
{
	return iClientForeignAgent->GetLastRPCError( aIntError );
}


/*******************************************************************************
 *
 * GetStatus()
 *
 ******************************************************************************/
int CForeignAgentServiceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}

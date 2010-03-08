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


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "strncpynt.h"
#include "CMobileAgentServiceStub.h"
#include "CCMncontroller.h"
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
CMobileAgentServiceStub::CMobileAgentServiceStub()
{
	iClientMobileAgent = new CCMncontroller();
	assert( iClientMobileAgent != NULL );
}


CMobileAgentServiceStub::~CMobileAgentServiceStub()
{
	delete iClientMobileAgent;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CMobileAgentServiceStub::StartUccsService( char *aHostName, int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	//copy to class variable so that startRPCservice can use it later on
	iHostName = aHostName;

	// connect to the remote service
	client_stub_error = iClientMobileAgent->Connect( iHostName );

	// return the result
	return client_stub_error;
}


int CMobileAgentServiceStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iClientMobileAgent->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CMobileAgentServiceStub::IssueCommand( CDataRecord* aRequest )
{
	int i, err, client_stub_error, method_id = -1;
	char *tmp_string;
	CDataRecord *request_reply;

	int rv_integer, sarg_integer;
	TComponentList rv_component_list;
	TOptionDesc sarg_option;
	TResult rv_result;
	TTunnelingModeDesc sarg_tunnel_mode;
	TPolicyRequest sarg_policy_request;
	TMobileNodeStatus rv_mnstatus;
	TForeignAgentList rv_foreign_agent_list;
	TForeignAgentInfo rv_foreign_agent_info, *tmp_foreign_agent_info;
	TTimeoutRequest sarg_timeout;
	TForeignAgentInfoRequest sarg_foreign_agent_info;
	THomeAddressDesc sarg_home_address;

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

		// make the call and update the return value
		client_stub_error = iClientMobileAgent->ss_startuprpcservice( &rv_integer );
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
		client_stub_error = iClientMobileAgent->sc_shutdownrpcservice( sarg_integer, &rv_integer );
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
		client_stub_error = iClientMobileAgent->list_devices( &rv_component_list );
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
		client_stub_error = iClientMobileAgent->cstr_createagent( &rv_result ); 
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

	// dstr_removeagent
	case 32:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->dstr_removeagent( sarg_integer, &rv_integer ); 
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
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->startmobileagent( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		break;

	// stopmobileagent
	case 4: 

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->stopmobileagent( sarg_integer, &rv_result ); 
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
		client_stub_error = iClientMobileAgent->getmobileagentstatus( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "MOBILE_NODE_AGENT_PROCESS_STATUS", rv_result.iData0 );
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
		client_stub_error = iClientMobileAgent->setsingleoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientMobileAgent->removesingleoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientMobileAgent->addlistoption( sarg_option, &rv_result ); 
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
		client_stub_error = iClientMobileAgent->removelistoption( sarg_option, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_result.iData0 );
		break;


	// sethomeaddress
	case 23:
		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_home_address.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "HOME_ADDRESS", &tmp_string, 2, 0, aRequest, request_reply );
		sarg_home_address.iHomeAddress = inet_addr( tmp_string );
		GETINTEGERARGUMENT( "HOME_ADDRESS_PREFIX", &(sarg_home_address.iHomeAddressPrefix), 3, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "HOME_AGENT_ADDRESS", &tmp_string, 4, 0, aRequest, request_reply );
		sarg_home_address.iHomeAgentAddress = inet_addr( tmp_string );
		GETSTRINGARGUMENT( "INTERFACE", &tmp_string, 5, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_home_address.iInterfaceName, tmp_string, X_MAXINTERFACENAMELEN );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->sethomeaddress( sarg_home_address, &rv_result ); 
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

	// getcareofaddress
	case 9:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->getcareofaddress( sarg_integer, &rv_result ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_result.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", IntToAddress(rv_result.iData0) );
		request_reply->NewField( "RESULT_DATA1", rv_result.iData1 );
		break;
		
	
	// getstatus
	case 11:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->getstatus( sarg_integer, &rv_mnstatus ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_mnstatus.iCallResult.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_mnstatus.iCallResult.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_mnstatus.iCallResult.iData0 );
		request_reply->NewField( "RESULT_DATA1", rv_mnstatus.iCallResult.iData1 );
		request_reply->NewField( "STATUS_CONNECTED", rv_mnstatus.iConnected );
		request_reply->NewField( "STATUS_TUNNEL_UP", rv_mnstatus.iTunnelUp );
		request_reply->NewField( "STATUS_LOCAL_ADDRESS", IntToAddress(rv_mnstatus.iLocalAddress) );
		request_reply->NewField( "STATUS_COLOCATED_ADDRESS", IntToAddress(rv_mnstatus.iColocatedAddress) );
		request_reply->NewField( "STATUS_FOREIGN_AGENT_ADDRESS", IntToAddress(rv_mnstatus.iForeignAgentAddress) );
		request_reply->NewField( "STATUS_HOME_AGENT_ADDRESS", IntToAddress(rv_mnstatus.iHomeAgentAddress) );
		request_reply->NewField( "STATUS_HOME_ADDRESS", IntToAddress(rv_mnstatus.iHomeAddress) );
		request_reply->NewField( "STATUS_LIFETIME_REMAINING", rv_mnstatus.iLifetimeRemaining);
		request_reply->NewField( "STATUS_LAST_REGISTRATION_REPLY_CODE", rv_mnstatus.iReplyCode );
		request_reply->NewField( "STATUS_SECONDS_SINCE_LAST_REPLY", rv_mnstatus.iSecondsSinceLastReply );
		request_reply->NewField( "STATUS_SECONDS_SINCE_LAST_REQUEST", rv_mnstatus.iSecondsSinceLastRequest );
		request_reply->NewField( "STATUS_INFO_TEXT", rv_mnstatus.iInfoText );
		request_reply->NewField( "STATUS_LAST_ERROR_STRING", rv_mnstatus.iLastErrorString );
		break;


	// connect
	case 12:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_tunnel_mode.iAgentID), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "TUNNELMODE", (int*)&(sarg_tunnel_mode.iTunnelMode), 2, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->connect( sarg_tunnel_mode, &rv_result ); 
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


	// disconnect
	case 13:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->disconnect( sarg_integer, &rv_result ); 
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


	// confirm
	case 16:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->confirm( sarg_integer, &rv_result ); 
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


	// rescaninterfaces
	case 17:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->rescaninterfaces( sarg_integer, &rv_result ); 
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

	
	// setfapolicy
	case 18:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_policy_request.iAgentID), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "POLICY", &(sarg_policy_request.iPolicy), 2, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->setfapolicy( sarg_policy_request, &rv_result ); 
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


	// getfapolicy 
	case 19:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->getfapolicy( sarg_integer, &rv_result ); 
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


	// listforeignagents
	case 20:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &sarg_integer, 1, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->listforeignagents( sarg_integer, &rv_foreign_agent_list ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "FOREIGNAGENTCOUNT", rv_foreign_agent_list.TForeignAgentList_len );
		for( i = 0; i < (signed)(rv_foreign_agent_list.TForeignAgentList_len); i++ ) {
			tmp_foreign_agent_info = &((rv_foreign_agent_list.TForeignAgentList_val)[i]);
			AddIteratedStringFieldName(	"FOREIGN_AGENT_ADDRESS", i, IntToAddress(tmp_foreign_agent_info->iForeignAgentAddress), request_reply );
			AddIteratedStringFieldName(	"INTERFACE_NAME", i, tmp_foreign_agent_info->iInterfaceName, request_reply );
			AddIteratedIntegerFieldName( "PRIORITY", i, tmp_foreign_agent_info->iPriority, request_reply );
			AddIteratedStringFieldName(	"NAI", i, tmp_foreign_agent_info->iNAI, request_reply );
			AddIteratedIntegerFieldName( "INUSE", i, tmp_foreign_agent_info->iInUse, request_reply );
			AddIteratedIntegerFieldName( "INTERFACE_INDEX", i, tmp_foreign_agent_info->iInterfaceIndex, request_reply );
			AddIteratedIntegerFieldName( "LAST_ADVERTISEMENT", i, tmp_foreign_agent_info->iLastAdvertisement, request_reply );
			AddIteratedIntegerFieldName( "ADVERTISEMENT_EXPIRY", i, tmp_foreign_agent_info->iAdvertisementExpiry, request_reply );
		}

		// free the memory (if any was allocated)
		if( rv_foreign_agent_list.TForeignAgentList_len > 0 ) {
			XdrFree( (char**)&(rv_foreign_agent_list.TForeignAgentList_val), (int*)&(rv_foreign_agent_list.TForeignAgentList_len) );
		}
		break;


	// getforeignagentinfo
	case 21:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_foreign_agent_info.iAgentID), 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "FOREIGN_AGENT_ADDRESS", &tmp_string, 2, 0, aRequest, request_reply );
		sarg_foreign_agent_info.iForeignAgentID = AddressToInt( tmp_string );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->getforeignagentinfo( sarg_foreign_agent_info, &rv_foreign_agent_info ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return value
		request_reply->NewField( "RESULT", rv_foreign_agent_info.iCallResult.iServiceResult );
		request_reply->NewField( "RESULT_SUB_COMPONENT", rv_foreign_agent_info.iCallResult.iSubComponentResult );
		request_reply->NewField( "RESULT_DATA0", rv_foreign_agent_info.iCallResult.iData0 );
		request_reply->NewField( "RESULT_DATA1", rv_foreign_agent_info.iCallResult.iData1 );
		request_reply->NewField( "FOREIGN_AGENT_ADDRESS", IntToAddress(rv_foreign_agent_info.iForeignAgentAddress) );
		request_reply->NewField( "INTERFACE_NAME", rv_foreign_agent_info.iInterfaceName );
		request_reply->NewField( "PRIORITY", rv_foreign_agent_info.iPriority );
		request_reply->NewField( "NAI", rv_foreign_agent_info.iNAI );
		request_reply->NewField( "INUSE", rv_foreign_agent_info.iInUse );
		request_reply->NewField( "INTERFACE_INDEX",	rv_foreign_agent_info.iInterfaceIndex );
		request_reply->NewField( "LAST_ADVERTISEMENT", rv_foreign_agent_info.iLastAdvertisement );
		request_reply->NewField( "ADVERTISEMENT_EXPIRY", rv_foreign_agent_info.iAdvertisementExpiry );
		break;


	// settimeout
	case 22:

		// extract the parameters
		GETINTEGERARGUMENT( "AGENTID", &(sarg_timeout.iAgentID), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "TIMEOUT", &(sarg_timeout.iTimeout), 2, 0, aRequest, request_reply );

		//make the call and update the return value
		client_stub_error = iClientMobileAgent->settimeout( sarg_timeout ); 
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}
		break;

	// any other method id will result in an RCC_INVALIDMETHODID result
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
char *CMobileAgentServiceStub::GetLastRPCError( int *aIntError )
{
	return iClientMobileAgent->GetLastRPCError( aIntError );
}


/*******************************************************************************
 *
 * GetStatus()
 *
 ******************************************************************************/
int CMobileAgentServiceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}

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
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"
#include "CPppControllerStub.h"
#include "CCPppcontroller.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/

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
CPppControllerServiceStub::CPppControllerServiceStub()
{
	iClientPppController = new CCPppcontroller();
	assert( iClientPppController != NULL );
}


CPppControllerServiceStub::~CPppControllerServiceStub()
{
	delete iClientPppController;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CPppControllerServiceStub::StartUccsService( char *aHostName, int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iClientPppController->Connect( aHostName );

	// return the result
	return client_stub_error;

}


int CPppControllerServiceStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iClientPppController->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CPppControllerServiceStub::IssueCommand( CDataRecord* aRequest )
{
	int i, err, client_stub_error, method_id = -1;
	char *tmp_string;
	CDataRecord *request_reply;

	int rv_integer, sarg_integer;
	TComponentList rv_component_list;
	TResult rv_result;
	TVarData rv_var_data;
	TPppSessionDesc rv_session_desc;
	TPppSessionConfig sarg_session_config;
	TStartupInfo sarg_startup_info;

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

		// setup the args
		sarg_startup_info.iEmpty = 0;

		// make the call and update the return value
		client_stub_error = iClientPppController->ss_startuprpcservice( sarg_startup_info, &rv_integer );
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
		client_stub_error = iClientPppController->sc_shutdownrpcservice( sarg_integer, &rv_integer );
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
		client_stub_error = iClientPppController->list_devices( &rv_component_list );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "DEVICECOUNT", rv_component_list.TComponentList_len );
		for( i = 0; i < rv_component_list.TComponentList_len; i++ ) {
			AddIteratedIntegerFieldName( "DEVICEID", i, (rv_component_list.TComponentList_val)[i], request_reply );
		}
		break;		

	// cstr_startpppsession
	case 31:

		// extract the parameters
		GETSTRINGARGUMENT( "MADDR", &tmp_string, 1, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_session_config.iMobsterAddress, tmp_string, MAXADDRLEN );
		GETSTRINGARGUMENT( "MPORT", &tmp_string, 2, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_session_config.iMobsterPort, tmp_string, MAXPORTLEN );
		GETINTEGERARGUMENT( "MTID", &(sarg_session_config.iMTID), 3, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "CONFIG", &tmp_string, 4, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_session_config.iSessionConfiguration, tmp_string, MAXCONFIGURATIONLENGTH );

		// make the call and update the return value
		client_stub_error = iClientPppController->cstr_startpppsession( sarg_session_config, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );		
		break;
		

	// dstr_removepppsession
	case 32:

		// extract the parameters
		GETINTEGERARGUMENT( "SID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientPppController->dstr_removepppsession( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );		
		break;


	// killsession
	case 5:

		// extract the parameters
		GETINTEGERARGUMENT( "SID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientPppController->killsession( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );		
		break;


	// stopsession
	case 6:

		// extract the parameters
		GETINTEGERARGUMENT( "SID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientPppController->stopsession( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );		
		break;


	// getsessioninfo
	case 7:

		// extract the parameters
		GETINTEGERARGUMENT( "SID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientPppController->getsessioninfo( sarg_integer, &rv_session_desc );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result values
		request_reply->NewField( "RESULT", rv_session_desc.iErrorCode );
		request_reply->NewField( "RESULT_DETAIL", rv_session_desc.iErrorDetail );
		request_reply->NewField( "SESSION_MADDR", rv_session_desc.iConfig.iMobsterAddress );
		request_reply->NewField( "SESSION_MPORT", rv_session_desc.iConfig.iMobsterPort );
		request_reply->NewField( "SESSION_MTID",	rv_session_desc.iConfig.iMTID );
		request_reply->NewField( "SESSION_CONFIG",	rv_session_desc.iConfig.iSessionConfiguration );
		request_reply->NewField( "SESSION_STATUS",	rv_session_desc.iSessionStatus );
		request_reply->NewField( "SESSION_INTERFACE_NAME",	rv_session_desc.iInterfaceName );
		request_reply->NewField( "SESSION_LOCAL_IP_ADDRESS",	IntToAddress(rv_session_desc.iLocalIPAddress) );
		request_reply->NewField( "SESSION_REMOTE_IP_ADDRESS",	IntToAddress(rv_session_desc.iRemoteIPAddress) );
		request_reply->NewField( "SESSION_PROCESS_STATUS",	rv_session_desc.iProcessStatus );
		request_reply->NewField( "SESSION_PROCESS_EXIT_REASON",	rv_session_desc.iProcessExitReason );
		request_reply->NewField( "SESSION_PROCESS_EXIT_CODE",	rv_session_desc.iProcessExitCode );
		break;


	// getppplog
	case 8:

		// extract the parameters
		GETINTEGERARGUMENT( "SID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientPppController->getppplog( sarg_integer, &rv_var_data );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the result values
		request_reply->NewField( "LOG",	rv_var_data.TVarData_val );
		request_reply->NewField( "LOG_LENGTH", (rv_var_data.TVarData_len - 1));

		// free the memory (if any was allocated)
		if( rv_var_data.TVarData_len > 0 ) {
			XdrFree( &(rv_var_data.TVarData_val), (int*)&(rv_var_data.TVarData_len) );
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
char *CPppControllerServiceStub::GetLastRPCError( int *aIntError )
{
	return iClientPppController->GetLastRPCError( aIntError );
}


/*******************************************************************************
 *
 * GetStatus()
 *
 ******************************************************************************/
int CPppControllerServiceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


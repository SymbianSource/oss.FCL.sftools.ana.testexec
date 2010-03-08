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
#include "CHostExecuteAsyncStub.h"
#include "CCHostexecuteasync.h"
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"

																		
/*******************************************************************************
 *
 * Constructor / Destructor
 *
 ******************************************************************************/
CHostExecuteAsyncStub::CHostExecuteAsyncStub()
{
	iClientHostExecuteClient = new CCHostexecuteasync();
	assert( iClientHostExecuteClient != NULL );
}


CHostExecuteAsyncStub::~CHostExecuteAsyncStub()
{
	delete iClientHostExecuteClient;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CHostExecuteAsyncStub::StartUccsService( char *aHostName,  int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iClientHostExecuteClient->Connect( aHostName );

	// return the result
	return client_stub_error;
}


int CHostExecuteAsyncStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iClientHostExecuteClient->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CHostExecuteAsyncStub::IssueCommand( CDataRecord *aRequest )
{
	int i, err, client_stub_error, method_id = -1, length_to_output;
	char *tmp_string;
	CDataRecord *request_reply;

	TStartupInfo arg_startup_info;
	int sarg_integer;

	int rv_integer;
	THostExecuteAsyncProcessInfo rv_proc_info;
	TComponentList rv_component_list;
	TResult rv_result;
	TVarData rv_vardata;

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
		arg_startup_info.iDummy = 0;

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->ss_startuprpcservice( arg_startup_info, &rv_integer );
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
		client_stub_error = iClientHostExecuteClient->sc_shutdownrpcservice( sarg_integer, &rv_integer );
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
		client_stub_error = iClientHostExecuteClient->list_devices( &rv_component_list );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "PROCESSCOUNT", rv_component_list.TComponentList_len );
		for( i = 0; i < rv_component_list.TComponentList_len; i++ ) {
			AddIteratedIntegerFieldName( "PID", i, (rv_component_list.TComponentList_val)[i], request_reply );
		}
		break;		


	// cstr_startprocess
	case 31:

		// extract the parameters
		GETSTRINGARGUMENT( "CMD", &tmp_string, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->cstr_startprocess( tmp_string, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
		break;		


	// dstr_removeprocess
	case 32:

		// extract the parameters
		GETINTEGERARGUMENT( "PID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->dstr_removeprocess( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
		break;		


	// killprocess
	case 5:

		// extract the parameters
		GETINTEGERARGUMENT( "PID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->killprocess( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
		break;		


	// stopprocess
	case 6:

		// extract the parameters
		GETINTEGERARGUMENT( "PID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->stopprocess( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
		break;		


	// getprocessinfo
	case 7:

		// extract the parameters
		GETINTEGERARGUMENT( "PID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->getprocessinfo( sarg_integer, &rv_proc_info );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_proc_info.iErrorCode );
		request_reply->NewField( "RESULT_DETAIL", rv_proc_info.iErrorDetail );
		request_reply->NewField( "PROCESS_COMMAND", rv_proc_info.iCommandLine );
		request_reply->NewField( "PROCESS_STATUS", rv_proc_info.iProcessStatus );
		request_reply->NewField( "PROCESS_EXIT_REASON", rv_proc_info.iProcessExitReason );
		request_reply->NewField( "PROCESS_EXIT_CODE", rv_proc_info.iExitCode );
		break;		


	// getstandardoutput
	case 8:

		// extract the parameters
		GETINTEGERARGUMENT( "PID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->getstandardoutput( sarg_integer, &rv_vardata );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		length_to_output = ((rv_vardata.TVarData_len > 0) ? (rv_vardata.TVarData_len - 1) : rv_vardata.TVarData_len);
		request_reply->NewField( "STDOUT", (char*)(rv_vardata.TVarData_val) );
		request_reply->NewField( "LENGTH", length_to_output );

		// free the memory (if any was allocated)
		if( rv_vardata.TVarData_len > 0 ) {
			XdrFree( (char**)&(rv_vardata.TVarData_val), (int*)&(rv_vardata.TVarData_len) );
		}
		break;		


	// getstandarderror
	case 9:

		// extract the parameters
		GETINTEGERARGUMENT( "PID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecuteClient->getstandarderror( sarg_integer, &rv_vardata );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		length_to_output = ((rv_vardata.TVarData_len > 0) ? (rv_vardata.TVarData_len - 1) : rv_vardata.TVarData_len);
		request_reply->NewField( "STDERR", (char*)(rv_vardata.TVarData_val) );
		request_reply->NewField( "LENGTH", length_to_output );

		// free the memory (if any was allocated)
		if( rv_vardata.TVarData_len > 0 ) {
			XdrFree( (char**)&(rv_vardata.TVarData_val), (int*)&(rv_vardata.TVarData_len) );
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
 * GetStatus()
 *
 ******************************************************************************/
int CHostExecuteAsyncStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CHostExecuteAsyncStub::GetLastRPCError( int *aIntError )
{
	return iClientHostExecuteClient->GetLastRPCError( aIntError );
}


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
#include "CHostExecuteStub.h"
#include "CCHostexecute.h"
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"


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
CHostExecuteStub::CHostExecuteStub()
{
	iClientHostExecute = new CCHostexecute();
	assert( iClientHostExecute != NULL );
}


CHostExecuteStub::~CHostExecuteStub()
{
	delete iClientHostExecute;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CHostExecuteStub::StartUccsService( char *aHostName,  int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iClientHostExecute->Connect( aHostName );

	// return the result
	return client_stub_error;
}


int CHostExecuteStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iClientHostExecute->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CHostExecuteStub::IssueCommand( CDataRecord* aRequest )
{
	int i, err, client_stub_error, method_id = -1;
	char *tmp_string;
	CDataRecord *request_reply;

	TStartupInfo sarg_startup_info;
	int rv_integer, sarg_integer;
	TExecuteRequest sarg_execute_request;
	TExecuteResult rv_execute_result;
	TComponentList rv_component_list;

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
		sarg_startup_info.iEmpty = 0;

		// make the call and update the return value
		client_stub_error = iClientHostExecute->ss_startuprpcservice( sarg_startup_info, &rv_integer );
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
		client_stub_error = iClientHostExecute->sc_shutdownrpcservice( sarg_integer, &rv_integer );
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
		client_stub_error = iClientHostExecute->list_devices( &rv_component_list );
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

	// st_executecommand
	case 3:

		// extract the parameters
		GETSTRINGARGUMENT( "COMMAND", &tmp_string, 1, 0, aRequest, request_reply );
		STRNCPY_NULL_TERMINATE( sarg_execute_request.iCommandLine, tmp_string, MAXCOMMANDLINE );
		GETINTEGERARGUMENT( "TIMEOUT", &(sarg_execute_request.iTimeout), 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "GETSTDOUT", &(sarg_execute_request.iRecordStandardOut), 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "GETSTDERR", &(sarg_execute_request.iRecordStandartError), 4, 1, aRequest, request_reply );

		// make the call and update the return value
		client_stub_error = iClientHostExecute->st_executecommand( sarg_execute_request, &rv_execute_result );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// extract the parameters
		request_reply->NewField( "RESULT", rv_execute_result.iResult );
		request_reply->NewField( "RESULT_DETAIL", rv_execute_result.iErrorCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_execute_result.iErrorDetail );
		request_reply->NewField( "PROCESS_EXIT_REASON", rv_execute_result.iProcessExitReason );
		request_reply->NewField( "PROCESS_EXIT_DETAIL", rv_execute_result.iProcessExitDetail );
		request_reply->NewField( "STANDARD_OUTPUT_LENGTH", rv_execute_result.iStandardOut.TVarData_len );
		request_reply->NewField( "STANDARD_OUTPUT", rv_execute_result.iStandardOut.TVarData_val );
		request_reply->NewField( "STANDARD_ERROR_LENGTH", rv_execute_result.iStandardError.TVarData_len );
		request_reply->NewField( "STANDARD_ERROR", rv_execute_result.iStandardError.TVarData_val );
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
int CHostExecuteStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CHostExecuteStub::GetLastRPCError( int *aIntError )
{
	return iClientHostExecute->GetLastRPCError( aIntError );
}

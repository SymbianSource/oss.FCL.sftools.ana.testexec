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
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"
#include "CTestServiceStub.h"
#include "CCTestservice.h"


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
CTestServiceStub::CTestServiceStub() 
{
	iClientTest = new CCTestservice();
	assert( iClientTest != NULL );
}


CTestServiceStub::~CTestServiceStub()
{
	delete iClientTest;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CTestServiceStub::StartUccsService( char *aHostName, int *aErrorInfo, int *aUnused )
{
	int client_stub_error;
	*aErrorInfo = *aUnused = 0;

	// connect the stub to the remote service
	client_stub_error = iClientTest->Connect( aHostName );

	// return the result
	return client_stub_error;
}


int CTestServiceStub::StopUccsService( int *aErrorInfo, int *aUnused )
{
	int client_stub_error;
	*aErrorInfo = *aUnused = 0;

	// disconnect the stub from the remote service
	client_stub_error = iClientTest->Disconnect();

	// return the result
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CTestServiceStub::IssueCommand( CDataRecord* aRequest )
{
	int i, client_result, method_id = -1;
	CDataRecord *request_reply;
	
	TStartupInfo sarg_startupinfo;
	int sarg_integer, rv_integer;
	TResult rv_result;
	TComponentList rv_component_list;

	// check params
	assert( aRequest != NULL );

	// create a standard reply 
	request_reply = CreateBaseReply( aRequest );
	assert( request_reply != NULL );

	// get and check the method_id
	request_reply->GetFieldAsInt( "METHODID", &method_id );
	if( method_id == -1 ) {
		UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
		return request_reply;
	}

	// now dispatch and call the appropriate method
	switch( method_id ) {

	// ss_startuprpcservice
	case 1:

		// extract the parameters
		sarg_startupinfo.iEmpty = 0;

		// make the call and update the return value
		client_result = iClientTest->ss_startuprpcservice( sarg_startupinfo, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
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
		client_result = iClientTest->sc_shutdownrpcservice( sarg_integer, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;

	// list_devices
	case 3:

		// make the call and update the return value
		client_result = iClientTest->list_devices( &rv_component_list );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "DEVICECOUNT", rv_component_list.TComponentList_len );
		for( i = 0; i < rv_component_list.TComponentList_len; i++ ) {
			AddIteratedIntegerFieldName( "DEVICEID", i, (rv_component_list.TComponentList_val)[i], request_reply );
		}
		break;


	// st_discard
	case 10:

		// extract the parameters
		sarg_integer = 10;

		// make the call and update the return value
		client_result = iClientTest->st_discard( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_EXTENDED_CODE", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
		break;


	// st_block
	case 11:

		// extract the parameters
		sarg_integer = 11;

		// make the call and update the return value
		client_result = iClientTest->st_block( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_EXTENDED_CODE", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
		break;


	// st_wait
	case 12:

		// extract the parameters
		sarg_integer = 12;

		// make the call and update the return value
		client_result = iClientTest->st_wait( sarg_integer, &rv_result );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_result.iStandardResult );
		request_reply->NewField( "RESULT_EXTENDED_CODE", rv_result.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv_result.iSystemError );
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
int CTestServiceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CTestServiceStub::GetLastRPCError( int *aIntError )
{
	return iClientTest->GetLastRPCError( aIntError );
}

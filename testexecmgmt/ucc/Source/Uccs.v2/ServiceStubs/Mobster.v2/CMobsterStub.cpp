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
#include "CMobsterStub.h"
#include "CCMobster.h"


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
CMobsterServiceStub::CMobsterServiceStub() 
{
	iClientMobster = new CCMobster();
	assert( iClientMobster != NULL );
}


CMobsterServiceStub::~CMobsterServiceStub()
{
	delete iClientMobster;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CMobsterServiceStub::StartUccsService( char *aHostName, int *aErrorInfo, int *aUnused )
{
	int client_stub_error;
	*aErrorInfo = *aUnused = 0;

	// connect the stub to the remote service
	client_stub_error = iClientMobster->Connect( aHostName );

	// return the result
	return client_stub_error;
}


int CMobsterServiceStub::StopUccsService( int *aErrorInfo, int *aUnused )
{
	int client_stub_error;
	*aErrorInfo = *aUnused = 0;

	// disconnect the stub from the remote service
	client_stub_error = iClientMobster->Disconnect();

	// return the result
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CMobsterServiceStub::IssueCommand( CDataRecord* aRequest )
{
	int i, client_result, method_id = -1;
	CDataRecord *request_reply;
	
	char *sarg_string;
	int sarg_integer, rv_integer;
	TDeviceDesc sarg_devicedesc, rv_devicedesc;
	TChannelAddress sarg_channel_address;
	TUUAddress sarg_uu_address;
	TComponentList rv_component_list;
	TVarData rv_vardata;

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
		GETINTEGERARGUMENT( "PORT", &(sarg_channel_address.iPort), 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_result = iClientMobster->ss_startuprpcservice( sarg_channel_address, &rv_integer );
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
		client_result = iClientMobster->sc_shutdownrpcservice( sarg_integer, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;

	// list_devices
	case 9:

		// make the call and update the return value
		client_result = iClientMobster->list_devices( &rv_component_list );
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


	// cstr_startdevice
	case 3:

		// extract the parameters
		GETINTEGERARGUMENT( "DATALINKCONFIG",	(int*)&(sarg_devicedesc.iDatalinkConfig), 1, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "FILTERCONFIG",		&(sarg_devicedesc.iFilterConfig),	2, 0, aRequest, request_reply );

		// make the call and update the return value
		client_result = iClientMobster->cstr_startdevice( sarg_devicedesc, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;


	// dstr_removedevice
	case 7:

		// extract the parameters
		GETINTEGERARGUMENT( "DEVICEID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_result = iClientMobster->dstr_removedevice( sarg_integer, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "RESULT", rv_integer );
		break;


	// GetDeviceInfo
	case 4:

		// extract the parameters
		GETINTEGERARGUMENT( "DEVICEID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_result = iClientMobster->getdeviceinfo( sarg_integer, &rv_devicedesc );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "DATALINKCONFIG",	rv_devicedesc.iDatalinkConfig );
		request_reply->NewField( "FILTERCONFIG",	rv_devicedesc.iFilterConfig );
		request_reply->NewField( "MTID",			rv_devicedesc.iMTID );
		request_reply->NewField( "TEID",			rv_devicedesc.iTEID );
		request_reply->NewField( "AIR_INPORT",		rv_devicedesc.iLocalAirInterfacePort );
		request_reply->NewField( "AIR_OUTADDR",		rv_devicedesc.iRemoteAirInterfaceAddress );
		request_reply->NewField( "AIR_OUTPORT",		rv_devicedesc.iRemoteAirInterfacePort );
		request_reply->NewField( "DEVICESTATUS",	rv_devicedesc.iDeviceStatus );
		request_reply->NewField( "DEVICEEXITREASON",	rv_devicedesc.iDeviceExitReason );
		request_reply->NewField( "DEVICEEXITSUBREASON",	rv_devicedesc.iDeviceExitSubReason );
		request_reply->NewField( "DEVICEEXITCODE",		rv_devicedesc.iDeviceExitDetail );
		break;


	// getdevicelog
	case 5:

		// extract the parameters
		GETINTEGERARGUMENT( "DEVICEID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_result = iClientMobster->getdevicelog( sarg_integer, &rv_vardata );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the result (this copies)
 		request_reply->NewField( "DEVICELOG",			rv_vardata.TVarData_val );
		request_reply->NewField( "DEVICELOGLENGTH",		rv_vardata.TVarData_len );

		// free the memory (if any was allocated)
		if( rv_vardata.TVarData_len > 0 ) {
			XdrFree( &(rv_vardata.TVarData_val), (int*)&(rv_vardata.TVarData_len) );
		}
		break;


	// stopdevice
	case 6:

		// extract the parameters
		GETINTEGERARGUMENT( "DEVICEID", &sarg_integer, 1, 0, aRequest, request_reply );

		// make the call and update the return value
		client_result = iClientMobster->stopdevice( sarg_integer, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the result
		request_reply->NewField( "RESULT", rv_integer );
		break;


	// setremoteuuaddress
	case 10:

		// extract the parameters
		GETINTEGERARGUMENT( "DEVICEID",			&(sarg_uu_address.iDeviceID),		1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT(	"REMOTE_ADDRESS",	&(sarg_string),						2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "REMOTE_PORT",		&(sarg_integer),					3, 0, aRequest, request_reply );
		sarg_uu_address.iRemoteAddress = AddressToInt(sarg_string);
		sarg_uu_address.iRemotePort = htons(sarg_integer);

		// make the call and update the return value
		client_result = iClientMobster->setremoteuuaddress( sarg_uu_address, &rv_integer );
		UpdateCompletionCode( request_reply, client_result );
		if( client_result != ERR_NONE ) {
			break;
		}

		// set the result
		request_reply->NewField( "RESULT", rv_integer );
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
int CMobsterServiceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CMobsterServiceStub::GetLastRPCError( int *aIntError )
{
	return iClientMobster->GetLastRPCError( aIntError );
}

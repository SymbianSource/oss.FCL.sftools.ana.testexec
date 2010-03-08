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
#include "CUuinterfaceStub.h"
#include "CCUuinterface.h"
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
CUuinterfaceStub::CUuinterfaceStub()
{
	iUuClient = new CCUuinterface();
	assert( iUuClient != NULL );
}


CUuinterfaceStub::~CUuinterfaceStub()
{
	delete iUuClient;
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CUuinterfaceStub::StartUccsService( char *aHostName,  int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	// check params
	assert( aHostName != NULL );
	assert( aErrorCode != NULL );
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iUuClient->Connect( aHostName );

	// return the result
	return client_stub_error;
}


int CUuinterfaceStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;

	// check params
	assert( aErrorCode != NULL );
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iUuClient->Disconnect();
	
	// done
	return client_stub_error;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CUuinterfaceStub::IssueCommand( CDataRecord* aRequest )
{
	int call_result, err, client_stub_error, method_id = -1;
	
	int value_int, value_frac, sigma_int, sigma_frac, cor_int, cor_frac;
	int max_value_int, max_value_frac;
	int source_port, destination_port;
	char *source_address, *destination_address;
	char *protocol;

	CDataRecord *request_reply;

	TStartupInfo sarg_startup_info;
	int rv_integer, sarg_integer;

	TResult rv;
	TComponentList rv_component_list;

	struct TConfigValue val;

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
		client_stub_error = iUuClient->ss_startuprpcservice( sarg_startup_info, &rv_integer );
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
		client_stub_error = iUuClient->sc_shutdownrpcservice( sarg_integer, &rv_integer );
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
		client_stub_error = iUuClient->list_devices( &rv_component_list );
		UpdateCompletionCode( request_reply, client_stub_error );
		if( client_stub_error != ERR_NONE ) {
			break;
		}

		// set the return values
		request_reply->NewField( "COUNT", rv_component_list.TComponentList_len );
		assert ( rv_component_list.TComponentList_len== 0 );
		break;		

	// st_setdelay
	case 3:
		// Extract the parameters
		GETSTRINGARGUMENT( "SRC", &source_address, 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "DST", &destination_address, 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "SRCPORT", &source_port, 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DSTPORT", &destination_port, 4, 1, aRequest, request_reply );
		GETSTRINGARGUMENT( "PROTOCOL", &protocol, 5, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DELAYI", &value_int, 6, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "DELAYF", &value_frac, 7, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "SIGMAI", &sigma_int, 8, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "SIGMAF", &sigma_frac, 9, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CORRI", &cor_int, 7, 10, aRequest, request_reply );
		GETINTEGERARGUMENT( "CORRF", &cor_frac, 11, 1, aRequest, request_reply );

		// Create the config values stub
		//setPortValues ( &source_port, &destination_port );
		CreateConfigValue( &val, 
							source_address, destination_address, 
							protocol, 
							source_port, destination_port, 
							value_int, value_frac, 
							sigma_int, sigma_frac,
							cor_int, cor_frac,
							0,0,0,0 );

		// Make the call and update the return values
		call_result = iUuClient->st_setdelay( val, &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

		break;

	// st_setdroppercentage
	case 4:
		//Extract the parameters
		GETSTRINGARGUMENT( "SRC", &source_address, 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "DST", &destination_address, 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "SRCPORT", &source_port, 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DSTPORT", &destination_port, 4, 1, aRequest, request_reply );
		GETSTRINGARGUMENT( "PROTOCOL", &protocol, 5, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DROPI", &value_int, 6, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "DROPF", &value_frac, 7, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CORRI", &cor_int, 8, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CORRF", &cor_frac, 9, 1, aRequest, request_reply );

		// Create the config values stub
		//setPortValues ( &source_port, &destination_port );
		CreateConfigValue( &val, 
							source_address, destination_address, 
							protocol,
				            source_port, destination_port,
							value_int, value_frac, 
							0,0,
							cor_int, cor_frac,
							0,0,0,0 );

		// Make the call and update the return values
		call_result = iUuClient->st_setdroppercentage( val, &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

		break;

	//st_setduplicatepercentage
	case 5:
		//Extract the parameters
		GETSTRINGARGUMENT( "SRC", &source_address, 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "DST", &destination_address, 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "SRCPORT", &source_port, 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DSTPORT", &destination_port, 4, 1, aRequest, request_reply );
		GETSTRINGARGUMENT( "PROTOCOL", &protocol, 5, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DUPI", &value_int, 6, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "DUPF", &value_frac, 7, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CORRI", &cor_int, 8, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CORRF", &cor_frac, 9, 1, aRequest, request_reply );
		
		// Create the config values stub
		//setPortValues ( &source_port, &destination_port );
		CreateConfigValue( &val,
							source_address, destination_address, 
							protocol,
				            source_port, destination_port,
							value_int, value_frac, 
							0,0,
							cor_int, cor_frac,
							0,0,0,0 );

		// Make the call and update the return values
		call_result = iUuClient->st_setduplicatepercentage( val, &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

		break;

	// st_setbandwidth
	case 6:
		//Extract the parameters
		GETSTRINGARGUMENT( "SRC", &source_address, 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "DST", &destination_address, 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "SRCPORT", &source_port, 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DSTPORT", &destination_port, 4, 1, aRequest, request_reply );
		GETSTRINGARGUMENT( "PROTOCOL", &protocol, 5, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "BWI", &value_int, 6, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "BWF", &value_frac, 7, 1, aRequest, request_reply );

		// Create the config values stub
		//setPortValues ( &source_port, &destination_port );
		CreateConfigValue( &val,
							source_address, destination_address, 
							protocol,
							source_port, destination_port,
							value_int, value_frac, 
							0,0,0,0,0,0,0,0 );

		// Make the call and update the return values
		call_result = iUuClient->st_setbandwidth( val, &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

		break;

	// st_setdrd
	case 7:
		//Extract the parameters
		GETSTRINGARGUMENT( "SRC", &source_address, 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "DST", &destination_address, 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "SRCPORT", &source_port, 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DSTPORT", &destination_port, 4, 1, aRequest, request_reply );
		GETSTRINGARGUMENT( "PROTOCOL", &protocol, 5, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "MINI", &value_int, 6, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "MINF", &value_frac, 7, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "MAXI", &max_value_int, 8, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "MAXF", &max_value_frac, 9, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CONGI", &cor_int, 10, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "CONGF", &cor_frac, 11, 1, aRequest, request_reply );
	
		// Create the config values stub
		//setPortValues ( &source_port, &destination_port );
		CreateConfigValue( &val, 
							source_address, destination_address, 
							protocol, 
							source_port, destination_port,
							value_int, value_frac, 
							0,0,0,0,
							max_value_int, max_value_frac,
							cor_int, cor_frac );

		// Make the call and update the return values
		call_result = iUuClient->st_setdrd( val, &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );
 
		break;

	// st_clearsettings
	case 8:
		//Extract the parameters
		GETSTRINGARGUMENT( "SRC", &source_address, 1, 0, aRequest, request_reply );
		GETSTRINGARGUMENT( "DST", &destination_address, 2, 0, aRequest, request_reply );
		GETINTEGERARGUMENT( "SRCPORT", &source_port, 3, 1, aRequest, request_reply );
		GETINTEGERARGUMENT( "DSTPORT", &destination_port, 4, 1, aRequest, request_reply );
		GETSTRINGARGUMENT( "PROTOCOL", &protocol, 5, 1, aRequest, request_reply );

		// Create the config values stub
		//setPortValues ( &source_port, &destination_port );
		CreateConfigValue( &val,
							source_address, destination_address, 
							protocol,
							source_port, destination_port,
							0,0,0,0,0,0,0,0,0,0 );

		// Make the call and update the return values
		call_result = iUuClient->st_clearsettings( val, &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

		break;
	
	// st_stopservice
	case 9:
		// Make the call and update the return values			
		call_result = iUuClient->st_stopservice( &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}
		
		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

		break;

	// st_reset
	case 10:
		// Make the call and update the return values
		call_result = iUuClient->st_reset( &rv );
		UpdateCompletionCode( request_reply, call_result );
		if( call_result != ERR_NONE ) {
			break;
		}

		// Set the return values
		request_reply->NewField( "RESULT", rv.iStandardResult );
		request_reply->NewField( "RESULT_DETAIL", rv.iExtendedCode );
		request_reply->NewField( "RESULT_SYSTEM_ERROR", rv.iSystemError );

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
int CUuinterfaceStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CUuinterfaceStub::GetLastRPCError( int *aIntError )
{
	return iUuClient->GetLastRPCError( aIntError );
}

/*******************************************************************************
 *
 * CreateConfigValue()
 *
 ******************************************************************************/
void CUuinterfaceStub::CreateConfigValue( TConfigValue* aConfigValue,
										char* aSrc, char* aDst, 
										char* aProtocol, 
										int aSrcPort, int aDstPort, 
										int aValueIntegerPart, int aValueFractionPart, 
										int aSigmaIntegerPart, int aSigmaFractionPart,
										int aCorrelationIntegerPart, int aCorrelationFractionPart,
										int aMaximumValueIntegerPart, int aMaximumValueFractionPart,
										int aCongestionIntegerPart, int aCongestionFractionPart )
{
	int slen;

	// check the params
	assert( aSrc != NULL );
	assert( aDst != NULL );

	// copy the source address
	STRNCPY_NULL_TERMINATE( aConfigValue->iSourceAddress, aSrc, X_MAX_ADDRESS_LENGTH);

	// copy the destination address
	STRNCPY_NULL_TERMINATE( aConfigValue->iDestinationAddress, aDst, X_MAX_ADDRESS_LENGTH);

	// set the protocol
	slen = strlen( aProtocol );
	if( (aProtocol != NULL) && (strnicmp(aProtocol,UDP,slen) == 0) ) {
		aConfigValue->iProtocol = X_UDP;
	} else {
		aConfigValue->iProtocol = X_TCP;
	}

	// set all the values
	aConfigValue->iSrcPort = aSrcPort;
	aConfigValue->iDstPort = aDstPort;
	aConfigValue->iValueIntegerPart = aValueIntegerPart;
	aConfigValue->iValueFractionPart = aValueFractionPart;
	aConfigValue->iSigmaIntegerPart = aSigmaIntegerPart;
	aConfigValue->iSigmaFractionPart = aSigmaFractionPart;
	aConfigValue->iCorrelationIntegerPart = aCorrelationIntegerPart;
	aConfigValue->iCorrelationFractionPart = aCorrelationFractionPart;
	aConfigValue->iMaximumValueIntegerPart = aMaximumValueIntegerPart;
	aConfigValue->iMaximumValueFractionPart = aMaximumValueFractionPart;
	aConfigValue->iCongestionIntegerPart = aCongestionIntegerPart;
	aConfigValue->iCongestionFractionPart = aCongestionFractionPart;

	// done
	
}

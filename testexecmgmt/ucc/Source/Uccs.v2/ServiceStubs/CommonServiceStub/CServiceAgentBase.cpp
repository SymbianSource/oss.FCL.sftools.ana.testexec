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
#include <winsock2.h>
#include <rpc/rpc.h>

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CServiceAgentBase.h"
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"
#include "../../../SocketLibrary/socket_helper.h"
#include "../../../include/penstd.h"


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
CServiceAgentBase::CServiceAgentBase()
{
}


CServiceAgentBase::~CServiceAgentBase()
{
}


/*******************************************************************************
 *
 * SECTION: Methods to construct and manipulated the reply
 *
 ******************************************************************************/
/*******************************************************************************
 *
 * PROTECTED METHOD: CreateBaseReply -- IService::IssueCommand() is required to
 * return a valid request reply record. This record MUST MUST MUST contain a
 * set of basic fields. This method creates _all_ the basic fields, some will 
 * have to be updated later.
 *
 ******************************************************************************/
CDataRecord *CServiceAgentBase::CreateBaseReply( CDataRecord *aRequest )
{
	CDataRecord *service_request_reply;
	int err;
	int service_id;
	int method_id; 
	char *service_host;
	char *service_name;
	char *call;

	// check params
	assert( aRequest != NULL );

	// create the reply object
	service_request_reply = new CDataRecord();
	assert( service_request_reply != NULL );

	// get the service name
	err = aRequest->GetFieldAsString( "SVCNAME", &service_name );
	if( err != UCCS_OK )
	{
		// If the service name does not exist then resort to the legacy method

		// get the service id
		err = aRequest->GetFieldAsInt( "SVCID", &service_id );
		assert( err == UCCS_OK );
		err = aRequest->GetFieldAsString( "SVCHOST", &service_host );
		assert( err == UCCS_OK );
		service_request_reply->NewField( STD_REPLY_FIELD_SERVICE_ID, service_id );
		service_request_reply->NewField( STD_REPLY_FIELD_SERVICE_HOST, service_host );
	}
	else
	{
		service_request_reply->NewField( STD_REPLY_FIELD_SERVICE_NAME, service_name );
	}

	// get the methodid -- default to -1 if not there -- error must be caught by caller
	method_id = -1;
	aRequest->GetFieldAsInt( "METHODID", &method_id );

	// now set all the values
	service_request_reply->NewField( STD_REPLY_FIELD_METHODID, method_id );
	service_request_reply->NewField( STD_REPLY_FIELD_REQUESTCOMPLETIONCODE, ERR_INVALID_ERROR_CODE);
	service_request_reply->NewField( STD_REPLY_FIELD_REQUESTCOMPLETIONDESC, GetPenstdErrorString(ERR_INVALID_ERROR_CODE) );

	// Check to see if this request is a new generic stub call and log the CALL value
	err = aRequest->GetFieldAsString( "CALL", &call );
	if( err == UCCS_OK )
	{
		service_request_reply->NewField( STD_REPLY_FIELD_CALL, call );
	}

	return service_request_reply;
}


/*******************************************************************************
 *
 * PROTECTED METHOD: UpdateCompletionCode
 *
 ******************************************************************************/
void CServiceAgentBase::UpdateCompletionCode( CDataRecord *aReply, int aError )
{
	int completion_code, err, rpc_error_int;
	char *rpc_error_string;

	// verify params
	assert( aReply != NULL );

	// a reply can only have it's completion code set once 
	err = aReply->GetFieldAsInt( "REQUESTCOMPLETIONCODE", &completion_code );
	assert( err == UCCS_OK );
	assert( completion_code == ERR_INVALID_ERROR_CODE );

	// now update the record
	err = aReply->ChangeFieldData( STD_REPLY_FIELD_REQUESTCOMPLETIONCODE, aError );
	assert( err == UCCS_OK );
	err = aReply->ChangeFieldData( STD_REPLY_FIELD_REQUESTCOMPLETIONDESC, GetPenstdErrorString(aError) );
	assert( err == UCCS_OK );

	// add any custom fields
	if( aError == ERR_RPC_ERROR ) {
		rpc_error_string = GetLastRPCError( &rpc_error_int );
		aReply->NewField( "RPC_ERROR_STRING", rpc_error_string );
		aReply->NewField( "RPC_ERROR_INT", rpc_error_int );
	}
}


/*******************************************************************************
 *
 * PROTECTED METHOD: GetLastRPCError
 *
 ******************************************************************************/
char *CServiceAgentBase::GetLastRPCError( int *aIntError )
{
	*aIntError = NULL;
	return "(unimplemented)";
}


/*******************************************************************************
 *
 * SECTION: Stub Helpers
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * PROTECTED METHOD: GetStringArgument
 *
 ******************************************************************************/
int CServiceAgentBase::GetStringArgument( char *aFieldName, char **aOutput, int aParamIndex, int aOptional, CDataRecord *aRequest, CDataRecord *aReply )
{
	int err;

	// check params
	assert( aFieldName != NULL );
	assert( aOutput != NULL );

	// get the field 
	err = aRequest->GetFieldAsString( aFieldName, aOutput );

	// if missing and optional then ok -- DEFAULT IS ALWAYS BE NULL
	if( (err != UCCS_OK) && (aOptional != 0) ) {
		*aOutput = NULL;
		return 0;
	}

	// otherwise a missing value is not allowed
	if( err != UCCS_OK ) {
		aReply->NewField( "MISSINGPARAMINDEX", aParamIndex );
		UpdateCompletionCode( aReply, ERR_MISSING_PARAMETER );
		return -1;
	}
		
	// otherwise return OK
	return 0;
}


/*******************************************************************************
 *
 * PROTECTED METHOD: GetIntegerArgument
 *
 ******************************************************************************/
int CServiceAgentBase::GetIntegerArgument( char *aFieldName, int *aOutput, int aParamIndex, int aOptional, CDataRecord *aRequest, CDataRecord *aReply )
{
	int err;

	// check params
	assert( aFieldName != NULL );
	assert( aOutput != NULL );

	// get the field 
	err = aRequest->GetFieldAsInt( aFieldName, aOutput );

	// if missing and optional then ok -- DEFAULT MUST ALWAYS BE NULL
	if( (err != UCCS_OK) && (aOptional != 0) ) {
		*aOutput = NULL;
		return 0;
	}

	// otherwise a missing value is not allowed
	if( err != UCCS_OK ) {
		aReply->NewField( "MISSINGPARAMINDEX", aParamIndex );
		UpdateCompletionCode( aReply, ERR_MISSING_PARAMETER );
		return -1;
	}
		
	// otherwise return OK
	return 0;
}

/*******************************************************************************
 *
 * PROTECTED METHOD: AddIteratedIntegerFieldName
 *
 ******************************************************************************/
void CServiceAgentBase::AddIteratedIntegerFieldName( char *aFieldname, int aIndex, int aValue, CDataRecord *aReply )
{
	int err;
	char fieldname[MAXBUFFERSIZE];

	sprintf( fieldname, "%s_%d", aFieldname, aIndex );
	err = aReply->NewField( fieldname, aValue );
	assert( err == UCCS_OK );
}


/*******************************************************************************
 *
 * PROTECTED METHOD: AddIteratedStringFieldName
 *
 ******************************************************************************/
void CServiceAgentBase::AddIteratedStringFieldName( char *aFieldname, int aIndex, char *aValue, CDataRecord *aReply )
{
	int err;
	char fieldname[MAXBUFFERSIZE];

	sprintf( fieldname, "%s_%d", aFieldname, aIndex );
	err = aReply->NewField( fieldname, aValue );
	assert( err == UCCS_OK );
}


/*******************************************************************************
 *
 * PROTECTED METHOD: AddressToInt
 *
 ******************************************************************************/
int CServiceAgentBase::AddressToInt( char *aAddress )
{
	int is_ip, addr;
	struct hostent *hostname;

	// see if this is an IP address - if so then convert it and return
	is_ip = is_ip_address( aAddress );
	if( is_ip != 0 ) {
		return inet_addr(aAddress);
	}

	// otherwise try and resolve it as a hostname (must be ipv4)
	hostname = gethostbyname( aAddress );
	if( hostname != NULL ) {
		if( hostname->h_length == 4 ) {
			addr = *((int*)(hostname->h_addr));
			return addr;
		}
	}

	// if this doesn't work then we just send zero to let if fail
	return 0;
}


/*******************************************************************************
 *
 * PROTECTED METHOD: IntToAddress
 *
 ******************************************************************************/
char *CServiceAgentBase::IntToAddress( int aAddr )
{
	int haddr = ntohl(aAddr);
	sprintf( iAddressString, "%d.%d.%d.%d", ((haddr>>24)&0x000000FF),
		((haddr>>16)&0x000000FF),
		((haddr>>8)&0x000000FF),
		((haddr>>0)&0x000000FF) );
	return iAddressString;
}


/*******************************************************************************
 *
 * PROTECTED METHOD: XdrFree
 *
 ******************************************************************************/
void CServiceAgentBase::XdrFree( char **ptr, int *len )
{
	XDR x;

	// check params
	assert( ptr != NULL );
	assert( *ptr!= NULL );
	assert( len != NULL );

	// set the operation
	x.x_op = XDR_FREE;

	// now call the free function
	xdr_bytes( &x, ptr, len, 0xFFFFFFFF );

	// done
	*ptr = NULL;
	*len = 0;
	return;
}



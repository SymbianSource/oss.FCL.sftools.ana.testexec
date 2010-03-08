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
#include <windows.h>

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "..\..\Core\UCCS_ServiceValues.h"
#include "..\..\Core\UCCS_ErrorCodes.h"
#include "CInternalStub.h"
#include "..\..\..\include\penstd.h"

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
CInternalStub::CInternalStub()
{
}


CInternalStub::~CInternalStub()
{
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CInternalStub::StartUccsService( char *aHostName, int *aLocalError, int *aRemoteError )
{
	*aLocalError = *aRemoteError = 0;
	return 0;
}


int CInternalStub::StopUccsService( int *aLocalError, int *aRemoteError )
{
	*aLocalError = *aRemoteError = 0;
	return 0;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CInternalStub::IssueCommand( CDataRecord* aRequest )
{
	int err;
	int method_id;
	int sarg_integer;
	char *sarg_string;
	CDataRecord *request_reply;

	// check params
	assert( aRequest != NULL );

	// create a standard reply 
	request_reply = CreateBaseReply( aRequest );
	assert( request_reply != NULL );

	// get and check the method_id
	err = request_reply->GetFieldAsInt( "METHODID", &method_id );
	assert( err == UCCS_OK );
	if( method_id == -1 ) {
		UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
		return request_reply;
	}

	// now dispatch and call the appropriate method
	switch( method_id ) {

	// pause
	case 1:
		// extract the parameters
		GETINTEGERARGUMENT( "PERIOD", &sarg_integer, 1, 0, aRequest, request_reply );

		// implement the call
		Sleep( sarg_integer );

		// update the result
		UpdateCompletionCode( request_reply, ERR_NONE );
		break;
		
	// print message 
	case 3:
		// extract the parameters
		GETSTRINGARGUMENT( "MSG", &sarg_string, 1, 0, aRequest, request_reply );

		// implement the call
		fprintf( stderr, "%s\n", sarg_string );
		fflush( stderr );

		// update the result
		UpdateCompletionCode( request_reply, ERR_NONE );
		break;

	// null
	case 4:
		UpdateCompletionCode( request_reply, ERR_NONE );
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
int CInternalStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}

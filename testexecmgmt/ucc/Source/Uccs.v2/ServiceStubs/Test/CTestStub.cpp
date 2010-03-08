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
* Filename: CTestStub.cpp
* ServiceStub for unit testing the Usecase Controller. 
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
#include "../../Core/UCCS_ServiceValues.h"
#include "../../Core/UCCS_ErrorCodes.h"
#include "CTestStub.h"
#include "../../../include/penstd.h"

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
CTestStub::CTestStub()
{
}


CTestStub::~CTestStub()
{
}


/*******************************************************************************
 *
 * Startup / Shutdown
 *
 ******************************************************************************/
int CTestStub::StartUccsService( char *aHostName, int *aErrorDetail, int *aUnused )
{
	*aErrorDetail = *aUnused = 0;
	return 0;
}


int CTestStub::StopUccsService( int *aErrorDetail, int *aUnused )
{
	*aErrorDetail = *aUnused = 0;
	return 0;
}


/*******************************************************************************
 *
 * IssueCommand
 *
 ******************************************************************************/
CDataRecord* CTestStub::IssueCommand( CDataRecord* aRequest )
{
	int err = 0;
	int method_id = -1;
	CDataRecord* request_reply = NULL;

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
	
	// create result field
	case 2:		
		err = request_reply->NewField( "TEST_RESULT_FIELD", "TEST_RESULT_VALUE" );
		assert( err == UCCS_OK );
		UpdateCompletionCode( request_reply, ERR_NONE );			
		break;

	// Any other method id results in an invalid method id result
	case 1:
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
int CTestStub::GetStatus()
{
	assert( !"GetStatus() - is not implemented" );
	return -1;
}


/*******************************************************************************
 *
 * GetLastRPCError()
 *
 ******************************************************************************/
char *CTestStub::GetLastRPCError( int *aIntError )
{
	assert( !"GetLastRPCError() - is not implemented" );
	return NULL;
}

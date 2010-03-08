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
* CGenericStub.cpp
*
*/




#include <assert.h>

#include "CGenericStub.h"
#include "CCGenericStub.h"
#include "GenericStub.h"

#define CALL_ID		"CallID"

CGenericStub::CGenericStub( char* aIniFile, char* aServiceName )
	: iIniFile(NULL)
{
	// Open the interface ini file
	iIniFile = new CUCCIniFile( aIniFile );
	assert( iIniFile != NULL );

	// Retrieve the IID and version
	int IID;
	bool ret = iIniFile->KeyValue("ServiceIID", aServiceName, IID);
	assert( ret == true );
	assert( IID != 0 );
	int version;
	ret = iIniFile->KeyValue("Version", aServiceName, version);
	assert( ret == true );
	assert( version != 0 );
	iGenericStub = new CCGenericstub( IID, version );
}

CGenericStub::~CGenericStub()
{
	delete iGenericStub;
	delete iIniFile;
}

int CGenericStub::StartUccsService( char *aHostName,  int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	
	*aErrorCode = 0;
	
	// connect to the remote service
	client_stub_error = iGenericStub->Connect( aHostName );

	// return the result
	return client_stub_error;
}

int CGenericStub::StopUccsService( int *aErrorCode, int *aUnused )
{
	int client_stub_error;
	*aErrorCode = 0;

	// shutdown the stub -- force it to close
	client_stub_error = iGenericStub->Disconnect();

	// done
	return client_stub_error;
}

CDataRecord* CGenericStub::IssueCommand( CDataRecord* aRequest )
{
	int call_result, err, client_stub_error, method_id = -1;
	
	CDataRecord *request_reply;

	int rv_integer, sarg_integer;

	// create a standard reply 
	request_reply = CreateBaseReply( aRequest );

	// get and check the method_id
	err = request_reply->GetFieldAsInt( "METHODID", &method_id );
	if( method_id == -1 )
	{
		UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
		return request_reply;
	}

	// now dispatch and call the appropriate method
	switch( method_id )
	{
		// ss_startuprpcservice
		case 1:
			{
				// extract the parameters
				TStartupInfo sarg_startup_info;
				sarg_startup_info.iEmpty = 0;

				// make the call and update the return value
				client_stub_error = iGenericStub->ss_startuprpcservice( sarg_startup_info, &rv_integer );
				UpdateCompletionCode( request_reply, client_stub_error );		
				if( client_stub_error != ERR_NONE )
				{
					break;
				}
				// set any return information
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// sc_shutdownrpcservice
		case 2: 
			{
				// extract the parameters
				GETINTEGERARGUMENT( "FORCE", &sarg_integer, 1, 1, aRequest, request_reply );

				// make the call and update the return value
				client_stub_error = iGenericStub->sc_shutdownrpcservice( sarg_integer, &rv_integer );
				UpdateCompletionCode( request_reply, client_stub_error );
				if( client_stub_error != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;

		// RunCommand
		case 10:
			{
				// Extract the call name first
				char* callName = NULL;
				GETSTRINGARGUMENT( "CALL", &callName, 1, 0, aRequest, request_reply );

				if( callName == NULL )
				{
					UpdateCompletionCode( request_reply, ERR_INVALID_CALL );
					return request_reply;
				}

				// Lookup the parameters for this call from the ini file
				// Compare that they are all provided if required
				vector<string> parameters = iIniFile->SectionParameters(callName);

				// Retrieve the callID from the section by using the call name.
				//   NOTE: Every call section must have a callID
				TCall call;
				bool ret = iIniFile->KeyValue(CALL_ID, callName, call.iCallID);
				if( !ret )
				{
					UpdateCompletionCode( request_reply, ERR_INVALID_CALL );
					return request_reply;
				}

				call.iParams.iParams_len = parameters.size()-1; // Don't count the CallID
				call.iParams.iParams_val = new TParam[parameters.size()-1];

				// Package up the call to be sent to the service
				int paramNum = 2; // Start at 2 as the "CALL" is 1.
				int callParamCount = 0;
				for( int i=0 ; i<parameters.size() ; i++ )
				{
					if( parameters[i] != CALL_ID )
					{
						int		required;
						ret = iIniFile->KeyValue(parameters[i], callName, required);
						if( !ret )
						{
							delete[] call.iParams.iParams_val;
							UpdateCompletionCode( request_reply, ERR_MISSING_PARAMETER );
							return request_reply;
						}

						char*	paramValue = NULL;

						// Retrieve the parameter from the i/p stream (eg. script, console, serial comms, ip comms, etc)
						if (GetStringArgument((char *) parameters[i].c_str(), &paramValue, paramNum, true, aRequest, request_reply))
							break;
	
						// Check to see if some data is present for the parameter
						if( paramValue==NULL )
						{
							// If the parameter was required then retun an error
							if( required==1 )
							{
								delete[] call.iParams.iParams_val;
								UpdateCompletionCode( request_reply, ERR_MISSING_PARAMETER );
								return request_reply;
							}
							// Else just decrement the number of parameters being sent to the service
							call.iParams.iParams_len--;
						}
						else
						{
							// Update the parameter name within the call
							if( parameters[i].size() > 0 && parameters[i].size() <= MAXPARAMNAMELENGTH )
							{
								strcpy(	call.iParams.iParams_val[callParamCount].iName,
										parameters[i].c_str() );
								
							}
							// Update the parameter value within the call
							if( strlen(paramValue) <= MAXPARAMVALUELENGTH )
							{
								strcpy(	call.iParams.iParams_val[callParamCount].iValue,
										paramValue );
							}

							// Increment the call parameter to update next
							callParamCount++;
						}
						// Increment the script parameter number to retrieve next
						paramNum++;
					}
				}

				// Make the call and update the return values
				call_result = iGenericStub->run_command( call, &rv_integer );

				// Cleanup the parameters now
				delete[] call.iParams.iParams_val;

				UpdateCompletionCode( request_reply, call_result );
				if( call_result != ERR_NONE )
				{
					break;
				}
				// set the return values
				request_reply->NewField( "RESULT", rv_integer );
			}
			break;
			
		// Any other method id results in an invalid method id result
		default:
			{
				UpdateCompletionCode( request_reply, ERR_INVALID_METHOD );
			}
			break;
	}

	// everything should be handled above 
	return request_reply;
}

int CGenericStub::GetStatus()
{
	return -1;
}

char *CGenericStub::GetLastRPCError( int *aIntError )
{
	return iGenericStub->GetLastRPCError( aIntError );
}

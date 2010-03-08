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
*
*/



#include <math.h>

#include "CRemoteInterface.h"
#include "CTCPTransport.h"

// Constant values
const char	KSpace[]				= { " " };
const char	KQuote[]				= { "\"" };
const char	KEquals[]				= { "=" };
const char	KRunCmd[]				= { "runcmd" };
const char	KServiceName[]			= { "svcname" };
const char	KCall[]					= { "call" };
const char	KMethodID[]				= { "methodid" };
const char	KStartServiceID[]		= { "1" };
const char	KStopServiceID[]		= { "2" };
const char	KDefaultMethodID[]		= { "10" };

CRemoteInterface::CRemoteInterface( const string &aDestination )
	: iTransport(NULL), iIsSetup(false), iDestination(aDestination), iConnected(false)
	{
	iRandomSeed = rand();
	}

CRemoteInterface::~CRemoteInterface()
	{
	if( iTransport )
		{
		iTransport->Disconnect();
		delete iTransport;
		iTransport = NULL;
		iConnected = false;
		}
	}

void CRemoteInterface::SetupStartService( const string &aServiceName )
	{
	// Setup the default start to the command line
	DefaultSetupCall( aServiceName, KStartServiceID );

	// The command line is now setup
	iIsSetup = true;
	}

void CRemoteInterface::SetupStopService( const string &aServiceName )
	{
	// Setup the default start to the command line
	DefaultSetupCall( aServiceName, KStopServiceID );

	// The command line is now setup
	iIsSetup = true;
	}

void CRemoteInterface::SetupCall( const string &aServiceName, const string &aCall )
	{
	// Setup the default start to the command line
	DefaultSetupCall( aServiceName, KDefaultMethodID );

	// Add the call
	iCommandLine += KCall;
	iCommandLine += KEquals;
	iCommandLine += KQuote;
	iCommandLine += aCall;
	iCommandLine += KQuote;
	iCommandLine += KSpace;

	// The command line is now setup
	iIsSetup = true;
	}

void CRemoteInterface::AddParam( const string &aName, const string &aValue )
	{
	// If the command line is setup append the next parameter
	if( iIsSetup )
		{
		iCommandLine += aName;
		iCommandLine += KEquals;
		iCommandLine += KQuote;
		iCommandLine += aValue;
		iCommandLine += KQuote;
		iCommandLine += KSpace;
		}
	}

int CRemoteInterface::Send()
	{
	int ret = 0;

	if( !iTransport )
		{
		iTransport = new CTCPTransport();
		}

	// If the command line is setup then continue with the send
	if( iIsSetup && iTransport)
		{
		if( !iConnected )
			{
			// Connect to the UCC
			ret = iTransport->Connect( iDestination );
			iConnected = true;
			}
		if( !ret )
			{
			if ( iCommandLine.size() > MAXCOMMANDLINELEN )
				{
				ret = TRI_COMMANDLINETOOLONG;
				}
			else
				{
				TRunCommandRequest* run_req = new TRunCommandRequest();
				if( run_req != NULL )
					{
					memset( run_req->iCommandLine, 0x00, sizeof(run_req->iCommandLine) );
					memcpy( run_req->iCommandLine, iCommandLine.c_str(), iCommandLine.size());

					TPHeader header;
					header.iCmdID = CMD_REQ_RUNCOMMAND;
					header.iDataLen = sizeof(*run_req);
					header.iUid = (int)iRand_UID;

					// Send the header first
					ret = iTransport->RequestSend( (char*)&header, sizeof(header) );
					ret?ret=TRI_ERRSENDING:ret=0;
					if( !ret )
						{
						// Now send the rest of the data
						ret = iTransport->RequestSend( (char*)run_req, sizeof(*run_req) );
						ret?ret=TRI_ERRSENDING:ret=0;
						}

					// Cleanup
					delete run_req;
					}
				else
					{
					ret = TRI_ERRINTIALISING;
					}
				}
			}
		if( !ret )
			{
			// Retrieve the header
			TPHeader header;
			ret = iTransport->RequestReceive( (char*)&header, sizeof(header) );

			if( !ret )
				{
				TRunCommandReply run_rep;
				// Retrieve the result
				ret = iTransport->RequestReceive( (char*)&run_rep, sizeof(run_rep) );
				ret?ret=TRI_ERRRECV:ret=run_rep.iResult;
				}
			}
		}

	return ret;
	}

void CRemoteInterface::DefaultSetupCall( const string &aServiceName, const string &aMethodID )
	{
	// New call started so clear out the command line buffer
	iCommandLine.erase();

	// Add the runcmd
	iCommandLine += KRunCmd;
	iCommandLine += KSpace;

	// Add the service name
	iCommandLine += KServiceName;
	iCommandLine += KEquals;
	iCommandLine += KQuote;
	iCommandLine += aServiceName;
	iCommandLine += KQuote;
	iCommandLine += KSpace;

	// Append the methodid to the command line
	iCommandLine += KSpace;
	iCommandLine += KMethodID;
	iCommandLine += KEquals;
	iCommandLine += KQuote;
	iCommandLine += aMethodID;
	iCommandLine += KQuote;
	iCommandLine += KSpace;
	}

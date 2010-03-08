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



#include "CRemoteInterface.h"

// Test constant values
const char	KRemoteHost[]		= { "10.16.163.13:666" };
const char	KServiceName[]		= { "TestService" };

int main()
	{
	// Some basic unit tests which call the test service via the UCC
	CRemoteInterface rif( KRemoteHost );

	// Start the service
	rif.SetupStartService( KServiceName );
	int ret = rif.Send();
	if( ret )
		{
		printf( "*** FAILED ***" );
		return ret;
		}

	// TestCallOne
	printf( "Sending TestCallOne\n" );
	rif.SetupCall( KServiceName, "TestCallOne" );
	rif.AddParam( "PARAM1", "HELLO" );
	rif.AddParam( "PARAM2", "WORLD" );
	ret = rif.Send();
	if( ret )
		{
		printf( "*** FAILED ***" );
		return ret;
		}

	// TestCallTwo
	printf( "Sending TestCallTwo\n" );
	rif.SetupCall( KServiceName, "TestCallTwo" );
	rif.AddParam( "PARAM1", "HELLO" );
	rif.AddParam( "PARAM2", "WORLD" );
	rif.AddParam( "PARAM3", "AGAIN" );
	ret = rif.Send();
	if( ret )
		{
		printf( "*** FAILED ***" );
		return ret;
		}

	// TestCallThree
	printf( "Sending TestCallThree\n" );
	rif.SetupCall( KServiceName, "TestCallThree" );
	rif.AddParam( "PARAM1", "PARAM1" );
	rif.AddParam( "PARAM2", "PARAM2" );
	rif.AddParam( "PARAM3", "PARAM3" );
	rif.AddParam( "PARAM4", "PARAM4" );
	rif.AddParam( "PARAM5", "PARAM5" );
	rif.AddParam( "PARAM6", "PARAM6" );
	rif.AddParam( "PARAM7", "PARAM7" );
	rif.AddParam( "PARAM8", "PARAM8" );
	rif.AddParam( "PARAM9", "PARAM9" );
	rif.AddParam( "PARAM10", "PARAM10" );
	ret = rif.Send();
	if( ret )
		{
		printf( "*** FAILED ***" );
		return ret;
		}

	// Stop the service
	rif.SetupStopService( KServiceName );
	ret = rif.Send();
	if( ret )
		{
		printf( "*** FAILED ***" );
		return ret;
		}

	return 0;
	}

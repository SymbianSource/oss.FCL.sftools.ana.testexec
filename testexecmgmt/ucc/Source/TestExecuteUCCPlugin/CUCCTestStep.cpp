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



/**
 @file CUCCTestStep.cpp
*/

#include <e32base.h>

#include "assert.h"
#include "CUCCTestStep.h"
#include "CUCCSDeviceProtocol.h"

_LIT( delim, "|" );

CUCCTestStep::CUCCTestStep() 
	{
	}

CUCCTestStep::~CUCCTestStep()
	{
	}

CUCCSDeviceControl *CUCCTestStep::CreateApplicationControlInstance( int *aUID, TBool aUIDRequired )
	{
	int r = 0;
	TBool rv;
	TBuf<50> initialisation_string;
	TPtrC temp_string;
	CUCCSDeviceControl *app_instance = NULL;

	// check params
	assert( aUID != NULL );
	*aUID = -1;

	// Note: I've changed the tags of the ini files, but am keeping support for the old tags 
	TDPTransport transType = TDP_NONE;
	
	// Get the com port from the ini file and add it to the initialisation string
	if( GetStringFromConfig( _L("UCCControl"), _L("CommPortIndex"), temp_string ) )
		{
		initialisation_string.Append( temp_string );
		initialisation_string.Append( delim );
		initialisation_string.Append( _L("115200") );
		initialisation_string.Append( delim );
		transType = TDP_SERIAL;
		}
	else if( GetStringFromConfig( _L("UCCControl"), _L("RemoteHost"), temp_string ) )
		{
		initialisation_string.Append( temp_string );
		initialisation_string.Append( delim );
		if( GetIntFromConfig( _L("UCCControl"), _L("IAP"), r) )
			{
			initialisation_string.Append( _L("IAP:") );
			initialisation_string.AppendNum(r);
			initialisation_string.Append( delim );
			}
		if( r == 0 && GetIntFromConfig( _L("UCCControl"), _L("SNAP"), r) )
			{
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
			initialisation_string.Append( _L("SNAP:") );
			initialisation_string.AppendNum(r);
			initialisation_string.Append( delim );
#else
			r = 0;
			INFO_PRINTF1(_L("Access Point selection is not supported.."));
#endif // SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
			}
		transType = TDP_TCP;
		}
	else
		{
		return NULL;
		}

	// Get the test id (use case number) from the ini file
	// Only if it is required.
	if( aUIDRequired )
		{
		rv = GetIntFromConfig( _L("UCCControl"), _L("UseCaseID"), (*aUID) );
		if( rv == 0 )
			{
			rv = GetIntFromConfig( _L("Test_Common"), _L("testID"), (*aUID) );
			if( rv == 0 )
				{
				return NULL;
				}
			}
		}

	// create the application control object
	app_instance = CUCCSDeviceControl::New();	
	assert (app_instance != NULL);

	// connect to the uccs
	r = app_instance->connectL( (TDesC16*)&initialisation_string, transType );
	if( r != 0)
		{
		delete app_instance;
		return NULL;
		}

	// done return the info
	return app_instance;
	}

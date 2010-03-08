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



#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>


/**********************************************************************************************
 *
 * Local Includes
 *
 *********************************************************************************************/
#include "CApplicationControlNull.h"
#include "../Core/UCCS_ErrorCodes.h"


/**********************************************************************************************
 *
 * Defines
 *
 *********************************************************************************************/
#define MAXTOKENS 64


/**********************************************************************************************
 *
 * Constructor
 *
 *********************************************************************************************/
CApplicationControlNull::CApplicationControlNull( MUccsControl *aCallBackService, IOutput *aOutput )
{
	assert( aCallBackService != NULL );
	assert( aOutput != NULL );
	iOutput = aOutput;
	iCallBackService = aCallBackService;
}

/**********************************************************************************************
 *
 * Destructor
 *
 *********************************************************************************************/
CApplicationControlNull::~CApplicationControlNull()
{
}


/**********************************************************************************************
 *
 * Run() -- the main routine called to run the serial listener.
 *
 *********************************************************************************************/
TCommandControlError CApplicationControlNull::Start( TRemoteControlTransport aTransport, char* aPortname, int *aErrorCode, int *aScriptError )
{
	int err, i;
	static int execution_count = 0;

	// check params
	assert( aScriptError != NULL );
	*aScriptError = 0;

	// check the parameters are valid
	if( aPortname == NULL ) {
		return EAC_INVALIDPORT;
	}

	// only run once -- then exit
	if( execution_count > 0 ) {
		return EAC_QUIT;
	}
	execution_count++;

	// parse the params
	err = ParseArgumentString( aPortname );
	if( err == -1 ) {
		return EAC_INVALIDPORT;
	}

	// start the use-case
	err = iCallBackService->StartUsecase( iArgs.iUsecaseID );
	if( err != UCCS_OK ) {
		*aErrorCode = err;
		return EAC_STARTUSECASEFAILED;
	}

	// run rendezvous the set number of times - wait for user input before each one 
	// if the interactive flag is set. The last rendezvous is never interactive.
	for( i = 0; i < iArgs.iRendezvousCount; i++ ) {
		if( (iArgs.iInteractiveFlag != 0) && (i != (iArgs.iRendezvousCount-1)) ) {
			fprintf( stdout, "Press enter to rendezvous.\n" );
			getchar();
		}
		err = iCallBackService->Rendezvous( iArgs.iUsecaseID );
		if( err != UCCS_OK ) {
			*aErrorCode = err;
			return EAC_RENDEZVOUSFAILED;
		}
	}

	// put in a wait so that the other thread has time to finish up
	Sleep( 1000 );

	// end use-case 
	err = iCallBackService->EndUsecase( iArgs.iUsecaseID, 0, aScriptError );
	*aErrorCode = err; 
	if( ( err != UCCS_OK ) ){
		return EAC_ENDUSECASEFAILED;
	}

	// done
	return EAC_SUCCESS;
}


/**********************************************************************************************
 *
 * ParseArgumentString() - portname should be a string:
 * "<usecaseid>[:<rendezvous count>[:<interactive_flag>]]"
 *
 *********************************************************************************************/
int CApplicationControlNull::ParseArgumentString( char *str )
{
	char *ptr, *tokens[MAXTOKENS];
	int token_count = 0, i;

	// check params
	assert( str != NULL );

	// tokenise the string based on the ':' delimiter
	for( ptr = str, i = 0; i < MAXTOKENS; i++ ) {
		tokens[i] = ptr;
		ptr = strchr( ptr, ':' );
		if( ptr == NULL ) 
			break;
		*ptr = 0;
		ptr++;
	}
	token_count = i + 1;

	// check that at least one token was found
	if( token_count == 0 ) {
		return -1;
	}

	// initialise the optional args to their defaults
	iArgs.iRendezvousCount = 1;
	iArgs.iInteractiveFlag = 0;

	// now convert the tokens into meaningful things
	iArgs.iUsecaseID = atoi(tokens[0]);
	if( token_count > 1 ) {
		iArgs.iRendezvousCount = atoi(tokens[1]);
	}
	if( token_count > 2 ) {
		iArgs.iInteractiveFlag = atoi(tokens[2]);
	}

	// done - success
	return 0;
}

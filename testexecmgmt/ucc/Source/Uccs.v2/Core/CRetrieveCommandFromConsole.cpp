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


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "CRetrieveCommandFromConsole.h"
#include "UCCS_ErrorCodes.h"


/***********************************************************************************
 *
 * Construction
 *
 **********************************************************************************/
CRetrieveCommandFromConsole::CRetrieveCommandFromConsole()
{
	iFile = NULL;
}

CRetrieveCommandFromConsole::~CRetrieveCommandFromConsole()
{
	if( iFile != NULL ) {
		fclose( iFile );
		iFile = NULL;
	}
}


/***********************************************************************************
 *
 * StartUseCase
 *
 **********************************************************************************/
int CRetrieveCommandFromConsole::StartUseCase( int aUsecaseID )
{
	fprintf( stdout, "_______________________________________________________________________\n\n");
	fprintf( stdout, "Use Case Controller -- Interactive Usecase Description\n" );
	fprintf( stdout, "_______________________________________________________________________\n\n");
	iFile = stdin;
	iUsecaseID = aUsecaseID;
	return UCCS_OK;
}


/***********************************************************************************
 *
 * EndUseCase
 *
 **********************************************************************************/
int CRetrieveCommandFromConsole::EndUseCase()
{
	fprintf( stdout, "END OF USE CASE\n" );
	return UCCS_OK;
}


/***********************************************************************************
 *
 * GetNextCommand
 *
 **********************************************************************************/
int CRetrieveCommandFromConsole::GetNextCommand( char *aBuffer, int aLength )
{
	fprintf( stdout, "> " );
	fgets( aBuffer, aLength, iFile );
	return UCCS_OK;
}



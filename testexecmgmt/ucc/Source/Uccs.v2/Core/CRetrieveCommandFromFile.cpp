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
* Switches
*
*/



/***********************************************************************************
 *
 * System Includes
 *
 **********************************************************************************/
#include <stdio.h>
#include <assert.h>

/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "CRetrieveCommandFromFile.h"
#include "UCCS_ErrorCodes.h"


/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define MAXFILENAME 256


/***********************************************************************************
 *
 * Construction
 *
 **********************************************************************************/
CRetrieveCommandFromFile::CRetrieveCommandFromFile()
{
	iFile = NULL;
}

CRetrieveCommandFromFile::~CRetrieveCommandFromFile()
{

#ifdef TESTCASE109
	char tempname[10];
	sprintf(tempname, "testing");
	iFile = new FILE;
	iFile->_ptr = tempname;
#endif

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
int CRetrieveCommandFromFile::StartUseCase( int aUsecaseID )
{
	char fname[MAXFILENAME];

	// make sure there isn't a file already open -- this is a defect -- the batchengine
	// should catch errors before they get to here
	assert( iFile == NULL );

	// try and open the file
	sprintf( fname, "script.%04d.ucd", aUsecaseID );
	iFile = fopen( fname, "r" );
	if( iFile == NULL ) {
		return UCCS_CANTOPENSCRIPTFILE;
	}
	iUsecaseID = aUsecaseID;
	return UCCS_OK;
}


/***********************************************************************************
 *
 * EndUseCase
 *
 **********************************************************************************/
int CRetrieveCommandFromFile::EndUseCase()
{
	fclose(iFile);
	iFile = NULL;
	return UCCS_OK;
}


/***********************************************************************************
 *
 * GetNextCommand
 *	
 **********************************************************************************/
int CRetrieveCommandFromFile::GetNextCommand( char *aBuffer, int aLength )
{
	char *c;
	c = fgets( aBuffer, aLength, iFile );
	if( c == NULL ) {
		return UCCS_NOMORECOMMANDS;
	}
	return UCCS_OK;
}



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
* Filename: UCCS_CDataField.cpp
* System Includes
*
*/



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_CDataField.h"

/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define MAXINTEGERSTRINGLEN 16


/***********************************************************************************
 *
 * Constructor
 *
 **********************************************************************************/
CDataField::CDataField( char* aName )
{
	// check the params
	assert( aName != NULL );
	assert( strlen(aName) < MAXNAMELENGTH );

	// initialise all state vars
	strcpy( iSelfObjectName, aName );
	iFieldValue = NULL;
	iFieldValueLength = 0;
	iType = UDF_UNDEFINED;
}


/***********************************************************************************
 *
 * Destructor
 *
 **********************************************************************************/
CDataField::~CDataField()
{
	// free any remaining value memory
	if( iFieldValue != NULL ) 
	{
		free( iFieldValue );
		iFieldValue = NULL;
	}

	iSelfObjectName[0] = 0;
	iFieldValueLength = 0;
	iType = UDF_UNDEFINED;
}

//----------------------------------------------------------------------------------
// Retrieves the item name
char* CDataField::GetName()
{
	return iSelfObjectName;
}

//----------------------------------------------------------------------------------
// Returns boolean for whether the passed string matches the name of the field
int CDataField::IsItem(char* anItemName)
{
	int difference = 0;

	// check params
	assert( anItemName != NULL );

	// compare string
	difference = strncmp( anItemName, iSelfObjectName, MAXNAMELENGTH );
	return ((difference == 0) ? 1 : 0);
}

//----------------------------------------------------------------------------------
// Set string value
void CDataField::SetValue(char* aValue)
{
	// first free previous value
	if(iFieldValue != NULL) 
	{
		free(iFieldValue);
		iFieldValueLength = 0;
	}

	// set the type
	iType = UDF_STRING;

	// if the value is null then we still want an empty string -- otherwise just copy the string
	if( aValue == NULL ) {
		iFieldValue = malloc( 1 );
		((char*)iFieldValue)[0] = 0;
		iFieldValueLength = 1;
	} else {
		iFieldValue = malloc(strlen(aValue) + 1 );
 		strcpy((char*)iFieldValue, aValue);
		iFieldValueLength = strlen((char*)iFieldValue) + 1;
	}
}

//----------------------------------------------------------------------------------
// Sets integer value 
void CDataField::SetValue(int aValue)
{
	// first free previous value
	if(iFieldValue != NULL) 
	{
		free(iFieldValue);
		iFieldValueLength = 0;
	}

	// now copy new value
	iType = UDF_STRING;
	iFieldValue = malloc( MAXINTEGERSTRINGLEN + 1 );
	sprintf((char*)iFieldValue, "%d", aValue);
	iFieldValueLength = (MAXINTEGERSTRINGLEN + 1);
}

//----------------------------------------------------------------------------------
// Sets buffer value 
void CDataField::SetValue(void *aValue, int aLen)
{
	// first free previous value
	if(iFieldValue != NULL)
	{
		free(iFieldValue);
		iFieldValueLength = 0;
	}

	// now copy new value
	iType = UDF_BUFFER;
	iFieldValue = malloc(aLen);
	memcpy(iFieldValue, aValue, aLen);
	iFieldValueLength = aLen;
}

//----------------------------------------------------------------------------------
// Returns the string value from the object (could contain anything)
char* CDataField::GetStrValue()
{
	return (char*)iFieldValue;
}

//----------------------------------------------------------------------------------
// Returns an integer value from the object 
int CDataField::GetIntValue()
{
	int intvalue = 0;

	if( iFieldValue != NULL )
	{
		intvalue = atoi((char*)iFieldValue);
	}
	return intvalue;	
}

//----------------------------------------------------------------------------------
// Returns a value from the buffer
void* CDataField::GetBufValue( int *aLen )
{
	assert( aLen != NULL );
	*aLen = iFieldValueLength;
	return iFieldValue;
}

//----------------------------------------------------------------------------------
// Sets the type of the item (i.e. command, error code etc)
void CDataField::SetType( TFieldType aType )
{
	iType = aType;
}

//----------------------------------------------------------------------------------
//returns the type
TFieldType CDataField::GetType()
{
	return iType;
}

//----------------------------------------------------------------------------------
void CDataField::PrintField( FILE *aFile )
{
	if( iType == UDF_STRING ) {
		fprintf( aFile, "%s: %s\n", iSelfObjectName, iFieldValue );
	} else if( iType == UDF_BUFFER ) {
		fprintf( aFile, "%s: %d byte buffer\n", iSelfObjectName, iFieldValueLength );
	} else {
		fprintf( aFile, "%s: unknown contents\n", iSelfObjectName );
	}
}

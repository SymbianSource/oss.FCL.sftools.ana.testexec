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
* Filename: UCCS_CDataRecord.cpp
* System Includes
*
*/



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_CDataRecord.h"
#include "UCCS_ErrorCodes.h"


/***********************************************************************************
 *
 * PUBLIC METHOD: Constructor
 *
 **********************************************************************************/
CDataRecord::CDataRecord()
{
	int i;

	// allocate a list to hold the fields
	iFieldList = (CDataField**)malloc( sizeof(CDataField*) * INITIALFIELDCOUNT );
	memset( iFieldList, 0, (sizeof(CDataField*)*INITIALFIELDCOUNT) );

	// initialise state
	iCapacity = INITIALFIELDCOUNT;
	iDataFields = 0;
	iField = -1;
	iLastFieldReturned = -1;

	// initialise all members
	for( i = 0; i < iCapacity; i++) {
		assert( iFieldList[i] == NULL );
		iFieldList[i] = NULL;
	}
}


/***********************************************************************************
 *
 * PUBLIC METHOD: Destructor
 *
 **********************************************************************************/
CDataRecord::~CDataRecord()
{
	// Delete all remaining fields
	for(int a = 0; a < iCapacity; a++) {	
		if(iFieldList[a] != NULL) {
			delete iFieldList[a];
			iFieldList[a] = NULL;
		}
	}
	
	// free the list
	free( iFieldList );
	iFieldList = NULL;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: NewField() - Creates a new record using the string value passed.
 *
 **********************************************************************************/
int CDataRecord::NewField( char* aDataFieldName, char* aValue )
{
	int pos = 0;

	// get a free slot in the list
	pos = GetFreeSlot();
	if( pos == -1 ) {
		return UCCS_RECORDFULL;
	}

	// create the field and set it's init value
	iFieldList[pos] = new CDataField( aDataFieldName );
	iFieldList[pos]->SetValue( aValue );

	// update the state
	iDataFields++;	
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: NewField() - Creates a new record using the integer value passed.
 *
 **********************************************************************************/
int CDataRecord::NewField(char* aDataFieldName, int aValue)
{
	int pos = 0;

	// get a free slot in the list
	pos = GetFreeSlot();
	if( pos == -1 ) {
		assert( pos != -1 );
		return UCCS_RECORDFULL;
	}

	// create the field and set it's init value
	iFieldList[pos] = new CDataField( aDataFieldName );
	iFieldList[pos]->SetValue( aValue );

	// update the state
	iDataFields++;
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: NewField() - Creates a new record using the length value passed.
 *
 **********************************************************************************/
int CDataRecord::NewField(char* aDataFieldName, void* aBuffer, int aLen)
{
	int pos = 0;
	
	// get a free slot in the list
	pos = GetFreeSlot();
	if( pos == -1 ) {
		assert( pos != -1 );
		return UCCS_RECORDFULL;
	}

	// create the field and set it's init value
	iFieldList[pos] = new CDataField( aDataFieldName );
	iFieldList[pos]->SetValue( aBuffer, aLen );
	
	// update the state
	iDataFields++;
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: DeleteField() - Removes data item object of the parameter name.
 *
 **********************************************************************************/
int CDataRecord::DeleteField(char* aDataFieldName)
{
	int err = 0;

	// Retrieve the item
	err = GetField( aDataFieldName );
	if( err != UCCS_OK ) {
		return err;
	}

	// Update state
	delete iFieldList[iField];
	iFieldList[iField] = NULL;
	iDataFields -= 1;
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: ChangeFieldData() - Alters the data item data.
 *
 **********************************************************************************/
int CDataRecord::ChangeFieldData( char* aDataFieldName, char* aDataFieldValue )
{
	int err = 0;

	// Retrieve the item
	err = GetField( aDataFieldName );
	if( err != UCCS_OK ) {
		return err;
	}

	// Set the new value
	(iFieldList[iField])->SetValue( aDataFieldValue );
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: ChangeFieldData() - Alters the data item data.
 *
 **********************************************************************************/
int CDataRecord::ChangeFieldData( char* aDataFieldName, int aDataFieldValue )
{
	int err = 0;

	// Retrieve the item
	err = GetField( aDataFieldName );
	if( err != UCCS_OK ) {
		return err;
	}

	// Set the new value
	(iFieldList[iField])->SetValue( aDataFieldValue );
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: ChangeFieldData() - Alters the data item data.
 *
 **********************************************************************************/
int CDataRecord::ChangeFieldData(char* aDataField, void* aDataFieldValue, int aDataFieldLen )
{
	int err = 0;
	
	// Retrieve the item
	err = GetField( aDataField );
	if( err != UCCS_OK ) {
		return err;
	}

	// Set the new value
	(iFieldList[iField])->SetValue( aDataFieldValue, aDataFieldLen );
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: GetFieldAsInt() - Returns the field as an integer.
 *
 **********************************************************************************/
int CDataRecord::GetFieldAsInt(char* aFieldName, int* aValue)
{
	int err = 0;

	// Check params
	assert( aValue != NULL );

	// Retrieve the item
	err = GetField( aFieldName );
	if( err != UCCS_OK ) {
		return err;
	}

	// Retrieve the value
	*aValue = (iFieldList[iField])->GetIntValue();
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: GetFieldAsString() - Returns the field as a string.
 *
 **********************************************************************************/
int CDataRecord::GetFieldAsString(char* aFieldName, char** aValue)
{
	int err = 0;

	// Check params
	assert( aValue != NULL );
	*aValue = NULL;

	// Retrieve the item
	err = GetField( aFieldName );
	if( err != UCCS_OK ) {
		return err;
	}

	// Retrieve the value
	*aValue = (iFieldList[iField])->GetStrValue();
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: GetFieldAsBuffer() - Returns the field as a buffer.
 *
 **********************************************************************************/
int CDataRecord::GetFieldAsBuffer(char* aFieldName, void** aValue, int* aLen)
{
	int err = 0;
	
	// Check params
	assert( aValue != NULL );
	assert( aLen != NULL );

	// Retrieve the item
	err = GetField( aFieldName );
	if( err != UCCS_OK ) {
		return err;
	}

	// Retrieve the value
	*aValue = (iFieldList[iField])->GetBufValue( aLen );
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: GetFieldCount() - Retrieve the number of active items in the list.
 *
 **********************************************************************************/
int CDataRecord::GetFieldCount()
{
	int i;
	int count = 0;

	// do a consistency check on the number of fields -- this should be taken out
	// once system test is passed.
	for( i = 0; i < iCapacity; i++ ) {
		if( iFieldList[i] != NULL ) 
			count++;
	}
	assert( iDataFields == count );

	// return the stored value
	return iDataFields;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: GetFirstField()
 *
 **********************************************************************************/
CDataField *CDataRecord::GetFirstField()
{
	int i;
	for( i = 0; i < iCapacity; i++ ) {
		if( iFieldList[i] != NULL ) {
			iLastFieldReturned = i;
			return iFieldList[i];
		}
	}
	return NULL;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: GetNextField()
 *
 **********************************************************************************/
CDataField *CDataRecord::GetNextField()
{
	int i;
	for( i = (iLastFieldReturned + 1); i < iCapacity; i++ ) {
		if( iFieldList[i] != NULL ) {
			iLastFieldReturned = i;
			return iFieldList[i];
		}
	}
	return NULL;
}


/***********************************************************************************
 *
 * PRIVATE METHODS: List Manipulation. Only these functions may modify iField
 *
 **********************************************************************************/

/***********************************************************************************
 *
 * GetFreeSlot() - returns the index of a free slot also places the 
 * index in the iField member
 *
 **********************************************************************************/
int CDataRecord::GetFreeSlot()
{
	int i;
	int oldcapacity;

	// loop through the iFieldList array and look for a free slot
	for( i = 0; i < iCapacity; i++ ) {
		if( iFieldList[i] == NULL ) {
			iField = i;
			return i;
		}
	}

	// search failed, no spaces left. Allocate more space.
	oldcapacity = iCapacity;
	iCapacity *= CAPACITYINCREASEFACTOR;
	iFieldList = (CDataField**)realloc( iFieldList, iCapacity*sizeof(CDataField*) );
	for( i = oldcapacity; i < iCapacity; i++ ) {
		iFieldList[i] = NULL;
	}
	iField = oldcapacity;
	return oldcapacity;
}


/***********************************************************************************
 *
 * GetField() - returns the index of a field with the matching name
 *
 **********************************************************************************/
int CDataRecord::GetField( char* aDataFieldName )
{
	int i, match;

	// loop through the iFieldList array and check for a name match
	for( i = 0; i < iCapacity; i++ ) {
		if( iFieldList[i] != NULL ) {
			match = iFieldList[i]->IsItem( aDataFieldName );
			if( match ) {
				iField = i;
				return UCCS_OK;
			}
		}
	}

	// otherwise 
	return UCCS_FIELDNOTFOUND;
}


/***********************************************************************************
 *
 * DEBUG METHODS: Print the record
 *
 **********************************************************************************/
void CDataRecord::PrintRecord( FILE *aFile )
{
	int i;
	for( i = 0; i < iCapacity; i++ ) {
		if( iFieldList[i] != NULL ) {
			fprintf( aFile, "    " );
			iFieldList[i]->PrintField( aFile );
		}
	}
}

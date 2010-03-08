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
* Filename: UCCS_CDataRecord.h
*
*/



#ifndef __UCCSCDATARECORD_H__
#define __UCCSCDATARECORD_H__

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
#define INITIALFIELDCOUNT			16
#define CAPACITYINCREASEFACTOR		2

/***********************************************************************************
 *
 * CDataRecord Class Definition -- this class will act as the 'library' of data 
 * items it has under it's control. It instantiates the items itself and retains
 * responsibility for the memory.
 *
 **********************************************************************************/
class CDataRecord
{
	public:

		//methods
		CDataRecord();
		~CDataRecord();

		// Create new field in the record
		int NewField(char* aDataFieldName, char* aDataFieldValue);				// for string values
		int NewField(char* aDataFieldName, int aDataFieldValue);				// for integer values
		int NewField(char* aDataFieldName, void* aBuffer, int aDataFieldLen);	// for opaque buffers

		// Delete a field from the record
		int DeleteField(char* aDataFieldName);

		// Change the value 
		int ChangeFieldData(char* aDataField, char* aDataFieldValue);		
		int ChangeFieldData(char* aDataField, int aDataFieldValue);		
		int ChangeFieldData(char* aDataField, void* aDataFieldValue, int aDataFieldLen );		
		
		// Get the value -- used by the service manager (and others) to extract info.
		int GetFieldAsInt(char* aFieldName, int* aValue);
		int GetFieldAsString(char* aFieldName, char** aValue);
		int GetFieldAsBuffer(char* aFieldName, void** aBuf, int* aLen);

		// Allow people to traverse records
		CDataField *GetFirstField();
		CDataField *GetNextField();

		// Get the number of fields in the record
		int GetFieldCount();
		
		// debug
		void PrintRecord( FILE *aFile );

	private:

		// Methods -- internal list manipulation functions
		int GetFreeSlot();
		int GetField(char* aDataFieldName);

		// Variables
		int iCapacity;
		int iDataFields;							// number of fields in the record
		CDataField **iFieldList;					// the list of item 
		int iField;									//acts as an index for the array (above)
		int iLastFieldReturned;
};

//----------------------------------------------------------------------------------

#endif __UCCSCDATARECORD_H__

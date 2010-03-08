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
* Filename: UCCS_CDataField.h
*
*/



#ifndef __UCCSCDATAFIELD_H__
#define __UCCSCDATAFIELD_H__


/***********************************************************************************
 *
 * System Includes
 *
 **********************************************************************************/
#include <stdio.h>


/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define MAXNAMELENGTH	128


/***********************************************************************************
 *
 * Types
 *
 **********************************************************************************/
typedef enum {
	UDF_UNDEFINED,
	UDF_STRING,
	UDF_INTEGER,
	UDF_BUFFER
} TFieldType;


/***********************************************************************************
 *
 * CDataField
 *
 **********************************************************************************/
class CDataField
{
	public:

		// Methods
		CDataField( char* aName );
		~CDataField();

		char* GetName();				// Get the name of the field
		int IsItem(char* anItemName);	// Return boolean whether this item has the given name

		// set field value
		void SetValue(char* aValue);
		void SetValue(int aValue);
		void SetValue(void *aValue, int aLen);

		// retrieve field value
		char* GetStrValue();	
		int GetIntValue();
		void* GetBufValue( int *aLen );

		// set and get type
		void SetType(TFieldType aType);
		TFieldType GetType();

		// debug
		void PrintField( FILE *aFile );

	private:
	
		//variables
		void* iFieldValue;
		int iFieldValueLength;
		char iSelfObjectName[MAXNAMELENGTH];
		TFieldType iType;
};

#endif //__UCCSCDATAFIELD_H__

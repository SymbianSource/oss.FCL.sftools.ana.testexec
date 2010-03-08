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
* Filename: UCCS_CCommandDecoder.h
*
*/



#ifndef __UCCSCOMMANDLINE_H__
#define __UCCSCOMMANDLINE_H__


/***********************************************************************************
 *
 * System Includes
 *
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_CServMgr.h"
#include "UCCS_ErrorCodes.h"
#include "UCCS_CDataRecord.h"

/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define MAXFIELDCOUNT 1024

/***********************************************************************************
 *
 * Types
 *
 **********************************************************************************/
typedef struct {
	char *iName;
	char *iValue;
	int iLength;
} TFieldDesc;

/***********************************************************************************
 *
 * CCommandDecoder definition
 *
 **********************************************************************************/
class CCommandDecoder
{
public:
		
	//methods
	CCommandDecoder( CDataRecord *aEnvironment );
	~CCommandDecoder();
	int ParseCommandToRecord( char *aCommandstring, CDataRecord *aRecord );
	void SetLastReply( CDataRecord *aLastReply );

private:
	int ParseCommand( char* aCommandstring, TFieldDesc *aFieldList, int aListLength );
	int StoreNextValue( char *aName, char *aValue, int aFileFlag, TFieldDesc *aFieldList, int aIndex );
	char *FindEndOfDoubleQuotes( char *aString );
	int GetFileSize( char *aFilename );
	int ReadFileIntoBuffer( char *aFilename, char *aBuff, int aBuffSize );
	void Cleanup( TFieldDesc *aFieldList, int aLen );
	int DecodeEscapeSequences(char *start_value);

	CDataRecord *iLastReply;
	CDataRecord *iEnvironment;
};													
//----------------------------------------------------------------------------------

#endif __UCCSCOMMANDLINE_H__

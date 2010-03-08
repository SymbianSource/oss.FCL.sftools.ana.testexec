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
* Filename: UCCS_CCommandDecoder.cpp
* System Includes
*
*/



#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <STDDEF.H>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_CCommandDecoder.h"


/***********************************************************************************
 *
 * Macro Functions
 *
 **********************************************************************************/
#define IS_WHITESPACE(c)				((c == '\t')||(c == ' ')||(c == '\n'))
#define NOT_IS_WHITESPACE_OR_TERM(c)	((IS_WHITESPACE(c) == 0)&&(c != 0))
#define IS_NAME_CHAR(c)					(isdigit(c)||isalpha(c)||(c == '_'))

/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define TA_FILENAME			1
#define TA_ASSIGNEDVALUE	2
#define TA_INTEGERLITERAL	4
#define TA_STRINGLITERAL	8

//--------------------------------------------------------------------------------------------------
//
// Construction
//
//--------------------------------------------------------------------------------------------------
CCommandDecoder::CCommandDecoder( CDataRecord *aEnvironment )
{
	iEnvironment = aEnvironment;
	iLastReply = NULL;
}

CCommandDecoder::~CCommandDecoder()
{
}


//--------------------------------------------------------------------------------------------------
//
// Parse commands here and store as a list of field descriptions 
//
//--------------------------------------------------------------------------------------------------
int CCommandDecoder::ParseCommand( char* aCommandstring, TFieldDesc *aFieldList, int aListLength )
{
	char *start_name;
	char *start_value;
	int element_attributes;
	char *end_value;
	int i;
	int err;

	// if the command string is null then there is nothing to do
	if( aCommandstring == NULL ) 
		return UCCS_OK;

	// process the entire string
	start_name = aCommandstring;
	for( i = 0; (i < aListLength) && (*start_name != 0); i++ ) {

		// init the vars
		element_attributes = 0;

		// skip over any whitespace to the next start
		for( start_name; IS_WHITESPACE(*start_name); start_name++ )
			;

		// if we are at NULL then exit
		if( *start_name == 0 ) {
			continue;
		}

		// if the character is not alpha then exit
		if( isalpha(*start_name) == 0 ) {
			return UCCS_PARSERFAILEDTOSTOREVALUE;
		}

		// otherwise this is the start of the token, look for the '=', only alpha, digit, and '_'
		// characters are allowed in names
		for( start_value = start_name; (start_value != NULL) && IS_NAME_CHAR(*start_value); start_value++ )
			;
		if( start_value == NULL ) {
			break;
		}
		if( *start_value != '=' ) {
			return UCCS_PARSERFAILEDTOSTOREVALUE;
		}
		*start_value = 0;
		start_value++;

		// if the character at start_value is alpha then this is an assigned value
		if( isalpha(*start_value) ) {
			element_attributes |= TA_ASSIGNEDVALUE;
		}

		// if the character at the start is numeric then this is an integer literal
		if( isdigit(*start_value) ) {
			element_attributes |= TA_INTEGERLITERAL;
		}

		// if the character at the start is '"' then this is a string literal
		if( *start_value == '"' ) {
			element_attributes |= TA_STRINGLITERAL;
		}

		// if the character is whitespace then this is an error
		if( IS_WHITESPACE(*start_value) ) {
			return UCCS_PARSERFAILEDTOSTOREVALUE;
		}

		// if the character at start_value is '"' then this is a string literal that may include
		// whitespace and escaped '\' and '"' characters. At the end of this the start_value should
		// point to a null terminated string that is what we want
		if( *start_value == '"' ) {
			end_value = FindEndOfDoubleQuotes( start_value );
			if( end_value == NULL ) {
				continue;
			}
			start_value++;
		} else {
			end_value = start_value;
			for( end_value; NOT_IS_WHITESPACE_OR_TERM(*end_value); end_value++ )
				;
		}

		// terminate the value
		*end_value = 0;

		// remove the escape sequences - eg. \" to "
		if(DecodeEscapeSequences(start_value)) {
            Cleanup( aFieldList, aListLength );
            return UCCS_PARSERFAILEDTOSTOREVALUE;
        }

		// save the new value
		err = StoreNextValue( start_name, start_value, element_attributes, aFieldList, i );
		if( err != 0 ) {
			Cleanup( aFieldList, aListLength );
			return UCCS_PARSERFAILEDTOSTOREVALUE;
		}

		// now move to the next potential token
		start_name = end_value + 1;
	}

	// done
	return UCCS_OK;
}


//--------------------------------------------------------------------------------------------------
//
// Parse commands here and store as a data record
//
//--------------------------------------------------------------------------------------------------
int CCommandDecoder::ParseCommandToRecord( char *aCommandstring, CDataRecord *aRecord )
{
	int err;
	int i;
	TFieldDesc iFieldList[MAXFIELDCOUNT];

	// Check params 
	assert( aRecord != NULL );

	// clear the field descriptions
	memset( iFieldList, 0, sizeof(TFieldDesc)*MAXFIELDCOUNT );

	// Parse the command into buffers
	err = ParseCommand( aCommandstring, iFieldList, MAXFIELDCOUNT );
	if( err != UCCS_OK ) {
		return err;
	}

	// Now add all the fields to the record	
	for( i = 0; (iFieldList[i].iName != NULL) && (i < MAXFIELDCOUNT); i++ ) {
		if( iFieldList[i].iLength == 0 ) {
			err = aRecord->NewField( iFieldList[i].iName, iFieldList[i].iValue );
			assert( err == UCCS_OK );
		} else {
			err = aRecord->NewField( iFieldList[i].iName, iFieldList[i].iValue, iFieldList[i].iLength );
			assert( err == UCCS_OK );
		}
		free( iFieldList[i].iName );
		free( iFieldList[i].iValue );
		iFieldList[i].iName = 0;
		iFieldList[i].iValue = 0;
		iFieldList[i].iLength = 0;
	}

	//done
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: SetLastReply - set the last reply to be used in parsing
 *
 **********************************************************************************/
void CCommandDecoder::SetLastReply( CDataRecord *aLastReply )
{
	iLastReply = aLastReply;
}


//--------------------------------------------------------------------------------------------------
//
// PRIVATE METHODS
//
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// Store the value
//
//--------------------------------------------------------------------------------------------------
int CCommandDecoder::StoreNextValue( char *aName, char *aValue, int aAttributes, TFieldDesc *aFieldList, int aIndex )
{
	int namelength;
	int valuelength;
//	int filesize;
	int err;
	int variable_found = 0;
	char *aAssignedValue;

	// check params
	assert( aName != NULL );
	assert( aValue != NULL );
	assert( aFieldList != NULL );
	assert( aFieldList[aIndex].iName == NULL );
	assert( aFieldList[aIndex].iValue == NULL );

	// store the name
	_strupr( aName );
	namelength = strlen( aName );
	(aFieldList[aIndex].iName) = (char*)malloc( namelength + 1 );
	assert( (aFieldList[aIndex].iName) != NULL );
	strcpy( (aFieldList[aIndex].iName), aName );

	// if the assigned values attribute is set then we: value = environment[value'] -- value = lastreply[value']
	if( aAttributes & TA_ASSIGNEDVALUE ) {

		// change the variable name to upper case to remove all case sensitivity
		_strupr( aValue );

		// look in the environment first
		if( iEnvironment != NULL ) {
			err = iEnvironment->GetFieldAsString( aValue, &aAssignedValue );
			if( err == UCCS_OK ) {
				aValue = aAssignedValue;
				variable_found = 1;
			}
		}

		// look in the last reply
		if( (iLastReply != NULL) && (variable_found == 0) ) {
			err = iLastReply->GetFieldAsString( aValue, &aAssignedValue );
			if( err == UCCS_OK ) {
				aValue = aAssignedValue;
				variable_found = 1;
			}
		}

		// if variable wasn't found then return error
		if( variable_found == 0 ) {
			return -1;
		}
	}

	// store non-file values 
	if( (aAttributes & TA_FILENAME) == 0 ) {
		valuelength = strlen( aValue );
		(aFieldList[aIndex].iValue) = (char*)malloc( valuelength + 1 );
		assert( (aFieldList[aIndex].iValue) != NULL );
		strcpy( (aFieldList[aIndex].iValue), aValue );
	}

	// store file value
#ifdef OLDSTYLE
	if( aAttributes & TA_FILENAME ) {

		// get the size of the file
		filesize = GetFileSize( aValue );
		if( filesize == -1 ) {
			free( (aFieldList[aIndex].iName) );
			(aFieldList[aIndex].iName) = NULL;
			return -1;
		}

		// allocate the memory to hold the file
		(aFieldList[aIndex].iValue) = (char*)malloc( filesize );
		assert( (aFieldList[aIndex].iValue) != NULL );

		// set the size
		aFieldList[aIndex].iLength = filesize;

		// read the file into memory
		err = ReadFileIntoBuffer( aValue, (aFieldList[aIndex].iValue), filesize );
		if( err != 0 ) {
			free( (aFieldList[aIndex].iName) );
			(aFieldList[aIndex].iName) = NULL;
			free( (aFieldList[aIndex].iValue) );
			(aFieldList[aIndex].iValue) = NULL;
			return -1;
		}
	}
#endif

	// done
	return 0;
}


//--------------------------------------------------------------------------------------------------
//
// FindEndOfDoubleQuotes -- given a string starting with '"' it returns a pointer to the character
// matching '"'. It deals with escaped chars.
//
//--------------------------------------------------------------------------------------------------
char *CCommandDecoder::FindEndOfDoubleQuotes( char *aString )
{
	char *str;

	// the first char MUST be '"'
	if( aString[0] != '"' ) {
		return NULL;
	}

	// now look for a non-escaped '"'
	str = aString + 1;
	while( 1 ) {
		str = strchr( str, '"' );
		if( str == NULL ) {
			return NULL;
		}
		if( str[-1] != '\\' ) {
			return str;
		}
		str++;
	}

	// done
	assert( !"INVALID CODE PATH" );
	return NULL;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//
// GetFileSize -- return the size of a file
//
//--------------------------------------------------------------------------------------------------
int CCommandDecoder::GetFileSize( char *aFilename )
{
	struct _stat sinfo;
	int err;
	err = _stat( aFilename, &sinfo );
	if( err != 0 ) {
		return -1;
	}
	return sinfo.st_size;
}


//--------------------------------------------------------------------------------------------------
//
// GetFileSize -- return the size of a file
//
//--------------------------------------------------------------------------------------------------
int CCommandDecoder::ReadFileIntoBuffer( char *aFilename, char *aBuff, int aBuffSize )
{
	FILE *fp;
	int err;
	int i;

	// open the file
	fp = fopen( aFilename, "rb" );
	if( fp == NULL ) {
		return -1;
	}

	// read in the bytes
	for( i = 0; i < aBuffSize; i++ ) {
		err = fread( &(aBuff[i]), 1, 1, fp );
		if( err != 1 ) {
			fclose( fp );
			return -1;
		}
	}

	// done
	fclose( fp );
	return 0;
}


//--------------------------------------------------------------------------------------------------
//
// Cleanup
//
//--------------------------------------------------------------------------------------------------
void CCommandDecoder::Cleanup( TFieldDesc *aFieldList, int aLen )
{
	int i;
	for( i = 0; i < aLen; i++ ) {
		if( (aFieldList[i].iName) != NULL ) {
			free( (aFieldList[i].iName) );
			(aFieldList[i].iName) = NULL;
		}
		if( (aFieldList[i].iValue) != NULL ) {
			free( (aFieldList[i].iName) );
			(aFieldList[i].iName) = NULL;
		}
	}
}


//--------------------------------------------------------------------------------------------------
//
// DecodeEscapeSequences - change \" to " and \\ to \
//
//--------------------------------------------------------------------------------------------------
// decode escape sequence
// - deliberately chose not to use sscanf's parser; more complicated, performance overhead, & requirement for a seperate string
int CCommandDecoder::DecodeEscapeSequences(char *start_value)
{
	int result = 0, read_pos = 0, write_pos = 0;

	// remove the escape char
	while (start_value[read_pos] != 0)
    {
		if (start_value[read_pos++] == '\\') 
        {
            char decodedChar = 0;
            switch (start_value[read_pos++]) 
            {
            case 'a':  decodedChar = '\a'; break;
            case 'b':  decodedChar = '\b'; break;
            case 'f':  decodedChar = '\f'; break;
            case 'n':  decodedChar = '\n'; break;
            case 'r':  decodedChar = '\r'; break;
            case 't':  decodedChar = '\t'; break;
            case 'v':  decodedChar = '\v'; break;
            case '\\': decodedChar = '\\'; break;
            case '?':  decodedChar = '\?'; break;
            case '\'': decodedChar = '\''; break;
            case '\"': decodedChar = '\"'; break;
            // case '\x"': decodedChar = '\"'; break; - not supported, since this could overrun the char array
            // case '\o"': decodedChar = '\"'; break; - not supported, functionality not needed
            default:
                // unknown
                result = -1;
                return result;
            }
            start_value[write_pos++] = decodedChar;
        }
        else
			start_value[write_pos++] = start_value[read_pos - 1];
	}

	// re-insert the null char
	start_value[write_pos] = 0;

    return result;
}

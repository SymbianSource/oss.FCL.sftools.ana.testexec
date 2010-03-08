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
#include <assert.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CLog.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define  MIN(a,b)					(((a) < (b)) ? (a) : (b))
#define  PRINT_ENTRY_PREFIX_LEN		16
#define  PRINT_ENTRY_SEPARATOR		(" - ")
#define  PRINT_ENTRY_SEPARATOR_LEN	3
#define  MAXLINESIZE				(PRINT_ENTRY_PREFIX_LEN + PRINT_ENTRY_SEPARATOR_LEN + MAXWHOSIZE + MAXMSGSIZE)
#ifndef WIN32
#define _snprintf snprintf
#endif

/*******************************************************************************
 *
 * Statics
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * Implementation
 *
 ******************************************************************************/
CLog::CLog( int aFileBacked )
{
	assert( aFileBacked == 0 );
	iLogVector = 0xFFFFFFFF;
}


void CLog::WriteLogEntry( TSeverity aSeverity, char *aWho, char *aMsg )				 
{
	TLogEntry entry;
	int slen;

	// copy message into struct
	memset( &entry, 0, sizeof(entry) );
	entry.iSeverity = aSeverity;
	slen = strlen(aWho);
	slen = MIN( slen, (MAXWHOSIZE-1) );
	memcpy( entry.iWho, aWho, slen );
	slen = strlen(aMsg);
	slen = MIN( slen, (MAXMSGSIZE-1) );
	memcpy( entry.iMsg, aMsg, slen );

	// now add it to the vector
	iLogEntries.push_back( entry );
}


void CLog::WriteLogEntry( TSeverity aSeverity, char *aWho, char *aErrorLocation, int aErrorA, int aErrorB )
{
	char buff[MAXMSGSIZE];
	_snprintf( buff, MAXMSGSIZE, "%s (%d, %d)", aErrorLocation, aErrorA, aErrorB );
	WriteLogEntry( aSeverity, aWho, buff );
}


void CLog::SetLogLevel( int aLogVector )
{
	iLogVector = aLogVector;
}


char *CLog::GetSeverityString( TSeverity aSeverity )
{
	switch( aSeverity ) {

	case SV_INFO:
		return "Info: ";

	case SV_WARNING:
		return "Warning: ";

	case SV_ERROR:
		return "Error: ";

	case SV_RESOURCE:
		return "Resource: ";

	case SV_STATE:
		return "State: ";

	default:
		return "Unknown: ";
	}

	// should never get here
	assert( !"INVALID CODE PATH" );
	return NULL;
}


int CLog::CalculateLogSize()
{
	char *severity_string;
	char cline[MAXLINESIZE];
	int log_size = 0;

	vector<TLogEntry>::iterator iter;
	for( iter = iLogEntries.begin(); iter != iLogEntries.end(); iter++ ) {
		severity_string = GetSeverityString( iter->iSeverity );
		sprintf( cline, "%s%s%s%s\n", severity_string, iter->iWho, PRINT_ENTRY_SEPARATOR, iter->iMsg );
		log_size += strlen(cline);
	}
	log_size++;
	return log_size;
}


void CLog::PrintLogToBuffer( int aBufferSize, char *aBuffer )
{
	char cline[MAXLINESIZE];
	vector<TLogEntry>::iterator iter;
	int slen, remaining_space;
	char *severity_string;

	// init params
	assert( aBufferSize > 0 );
	assert( aBuffer != NULL );

	// init
	aBuffer[0] = 0;
	remaining_space = aBufferSize;

	// process each entry 
	for( iter = iLogEntries.begin(); iter != iLogEntries.end(); iter++ ) {

		// print the line to the temp buffer
		severity_string = GetSeverityString( iter->iSeverity );
		sprintf( cline, "%s%s%s%s\n", severity_string, iter->iWho, PRINT_ENTRY_SEPARATOR, iter->iMsg );
	
		// check there is room left in the passed buffer
		slen = strlen(cline);
		if( remaining_space <= slen ) {
			break;
		}

		// otherwise 
		strcat( aBuffer, cline );
		remaining_space -= slen;
	}

	// done
	return;
}


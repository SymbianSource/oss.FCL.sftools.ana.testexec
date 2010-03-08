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
* CLog
* Log class used by the MT. All objects use the same instance.
*
*/



#ifndef __CLOG_H__
#define __CLOG_H__

/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <stdio.h>
#include <vector>
using namespace std;


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAXWHOSIZE	(128)
#define MAXMSGSIZE	(256)


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum
{
	SV_INFO			= 1,
	SV_WARNING		= 2,
	SV_ERROR		= 4,
	SV_RESOURCE		= 8,
	SV_STATE		= 16,
} TSeverity;

typedef struct {
	TSeverity iSeverity;
	char iWho[MAXWHOSIZE];
	char iMsg[MAXMSGSIZE];
} TLogEntry;


/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CLog
{
public:
	CLog( int aFileBacked = 0 );
	void WriteLogEntry( TSeverity aSeverity, char *aWho, char *aMsg );
	void WriteLogEntry( TSeverity aSeverity, char *aWho, char *aErrorLocation, int aErrorA, int aErrorB );
	void SetLogLevel( int aLogVector );
	char *GetSeverityString( TSeverity aSeverity );
	int CalculateLogSize();
	void PrintLogToBuffer( int aBufferSize, char *aBuffer );

private:
	FILE *iLogFile;
	int iLogVector;
	vector<TLogEntry> iLogEntries;
};

#endif //__CLOG_H__

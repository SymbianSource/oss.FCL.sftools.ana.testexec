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
* CHTMLOutput
*
*/



#ifndef __CHTMLOutput_H__
#define __CHTMLOutput_H__

/********************************************************************************
 *
 * System Includes
 *
 ********************************************************************************/
#include <stdio.h>

/********************************************************************************
 *
 * Local Includes
 *
 ********************************************************************************/
#include "UCCS_Interfaces.h"


/********************************************************************************
 *
 * Defines
 *
 ********************************************************************************/
#define MAXLOGFILENAMELEN	256
#define MAXLINELENGTH		1024

typedef enum {
	RT_INVALID,
	RT_COMMAND,
	RT_COMMAND_REPLY,
	RT_ENVIRONMENT,
	RT_COUNT
} TRecordType;


/********************************************************************************
 *
 * CHTMLOutput
 *
 ********************************************************************************/
class CHTMLOutput : public IOutput
{
public:
	CHTMLOutput();
	~CHTMLOutput();

	int SetFilename( char *aFilename );

	// from IOutput
	void StartUsecase( int aID );
	void EndUsecase( int aID, int aResult );
	void Error( int aErrorCode, char *aErrorString );
	void CompletedScript();

	void ExecuteCommand( TUccsCommand aCommand, CDataRecord *aArgs );
	void ExecuteCommandResult( int aUccsErrorcode );
	void ExecuteCommandReply( CDataRecord *aReply );

	void DisplayHelp();
	void DisplayLastCommand( CDataRecord *aCommand );
	void DisplayLastReply( CDataRecord *aResult );
	void DisplayEnvironment( CDataRecord *aEnvironment );

	void ExecuteString( char *aCommandLine );
	void ExecuteStringResult( int aUccsErrorcode );

	void StartServiceResult( int aServiceID, char *aServiceHost, int aStubReturnCode, int aErrorCode, int aUnused );
	void StopServiceResult( int aServiceID, char *aServiceHost, int aStubReturnCode, int aErrorCode, int aUnused );

private:
	// helper methods
	void OutputLine( FILE *aFile, char *aLine );
	void OutputBeginTags();
	void OutputDataRecord( FILE* aFile, CDataRecord *aArgs, TRecordType aRecordType, TUccsCommand aCommand );
	void OutputLineBreak( FILE* aFile );
	int IsFailedResult( CDataField *aField );

	// log files
	FILE *iFile;
	FILE *iFileDebug;
	FILE *iFileNoRefresh;
	char iLogName[MAXLOGFILENAMELEN];
	char iDebugLogName[MAXLOGFILENAMELEN];
	char iLogNameNoRefresh[MAXLOGFILENAMELEN];
	
	// standard line buffer
	char iLineBuffer[MAXLINELENGTH];

	// state
	bool iCompletedScriptCalled;
	int iCounter;

	// configurations - initialised in the constructor
	char *iNonZeroResultColour;
	char *iRecordTableTitle[RT_COUNT];
	char *iRecordTableTitleBackgroundColour[RT_COUNT];
	char *iRecordTableBorderColour[RT_COUNT];
	char *iStandardRowColour[RT_COUNT];
	int iStandardRowCount[RT_COUNT];
};

#endif //__CHTMLOutput_H__

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
#include <string.h>
#include <time.h>
#include <assert.h>

/********************************************************************************
 *
 * Local Includes
 *
 ********************************************************************************/
#include "penstd.h"
#include "strncpynt.h"
#include "HTMLOutput.h"
#include "UCCS_ErrorCodes.h"
#include "../ServiceStubs/CommonServiceStub/CServiceAgentBase.h"
#include "../Core/UCCS_CExecuteCommand.h"


/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define MAXFILENAME 1024
#define DEFAULT_ERROR_STRING ""
#define STRINGIFY_NULL(x)	(((x) == NULL) ? ("<null>") : (x)) 


/***********************************************************************************
 *
 * SECTION: CHTMLOutput
 *
 **********************************************************************************/
/********************************************************************************
 *
 * CHTMLOutput
 *
 ********************************************************************************/
CHTMLOutput::CHTMLOutput()
{
	iFile = NULL;
	iFileDebug = NULL;
	iFileNoRefresh = NULL;
	iCompletedScriptCalled = false;
	iCounter = 0;

	// setup the constant arrays
	iRecordTableTitle[RT_COMMAND]			= "<invalid>";
	iRecordTableTitle[RT_COMMAND_REPLY]		= "Command Reply";
	iRecordTableTitle[RT_ENVIRONMENT]		= "Environment";
	iRecordTableTitleBackgroundColour[RT_COMMAND]			= "#c0e0c0";
	iRecordTableTitleBackgroundColour[RT_COMMAND_REPLY]		= "#b0b0d0";
	iRecordTableTitleBackgroundColour[RT_ENVIRONMENT]		= "#e0c0c0";
	iRecordTableBorderColour[RT_COMMAND]		= "#008000";
	iRecordTableBorderColour[RT_COMMAND_REPLY]	= "#0000C0";
	iRecordTableBorderColour[RT_ENVIRONMENT]	= "#800000";
	iStandardRowCount[RT_COMMAND]		 = 3;
	iStandardRowCount[RT_COMMAND_REPLY]	 = 5;
	iStandardRowCount[RT_ENVIRONMENT]	 = 0;
	iStandardRowColour[RT_COMMAND]			= " bgcolor=\"#E0E0E0\"";
	iStandardRowColour[RT_COMMAND_REPLY]	= " bgcolor=\"#E0E0E0\"";
	iStandardRowColour[RT_ENVIRONMENT]		= " bgcolor=\"#E0E0E0\"";
	iNonZeroResultColour = " bgcolor=\"#e06060\"";
}


/********************************************************************************
 *
 * ~CHTMLOutput
 *
 ********************************************************************************/
CHTMLOutput::~CHTMLOutput()
{
	// output the end of the HTML
	OutputLine( iFile, "</body></html>\n" );
	OutputLine( iFileNoRefresh, "</body></html>\n" );
	OutputLine( iFileDebug, "</body></html>\n" );

	// close the files
	if( iFile != NULL ) {
		fclose( iFile );
	}
	if( iFileNoRefresh != NULL ) {
		fclose( iFileNoRefresh );
	}
	if( iFileDebug != NULL ) {
		fclose( iFileDebug );
	}
}


/********************************************************************************
 *
 * SetFilename
 *
 ********************************************************************************/
int CHTMLOutput::SetFilename( char *aFilename )
{	
	char fname[MAXFILENAME];
	int len;

	// check the filename
	if( aFilename == NULL ) {
		return UCCS_CANTOPENOUTPUTFILE;
	}

	// create the normal log file
	_snprintf( fname, MAXFILENAME, "%s.log.html", aFilename );
	len = strlen(fname)+1;
	if( len > MAXLOGFILENAMELEN)
		return UCCS_CANTOPENOUTPUTFILE;
	STRNCPY_NULL_TERMINATE( iLogName, fname, len );
	iFile = fopen( fname, "w+" );
	if( iFile == NULL ) {
		return UCCS_CANTOPENOUTPUTFILE;
	}

	// create the debug log file
	_snprintf( fname, MAXFILENAME, "%s.debug.html", aFilename );
	len = strlen(fname)+1;
	if( len > MAXLOGFILENAMELEN)
		return UCCS_CANTOPENOUTPUTFILE;
	STRNCPY_NULL_TERMINATE(iDebugLogName, fname, len);
	iFileDebug = fopen( fname, "w+" );
	if( iFileDebug == NULL ) {
		fclose( iFile );
		return UCCS_CANTOPENOUTPUTFILE;
	}

	// create the no refresh log file
	_snprintf( fname, MAXFILENAME, "%s.noref.html", aFilename );
	len = strlen(fname)+1;
	if( len > MAXLOGFILENAMELEN)
		return UCCS_CANTOPENOUTPUTFILE;
	STRNCPY_NULL_TERMINATE(iLogNameNoRefresh, fname, len);
	iFileNoRefresh = fopen( fname, "w+" );
	if( iFileNoRefresh == NULL ) {
		fclose( iFile );
		fclose( iFileDebug );
		return UCCS_CANTOPENOUTPUTFILE;
	}

	// add the header HTML tags to the all the log files
	OutputBeginTags();
	return UCCS_OK;
}


/***********************************************************************************
 *
 * SECTION: IOutput
 *
 **********************************************************************************/
/********************************************************************************
 *
 * PUBLIC METHOD: StartUsecase
 *
 ********************************************************************************/
void CHTMLOutput::StartUsecase( int aID )
{
	int err;
	char fname[MAXFILENAME];

	// Reset the operation counter
	iCounter = 0;

	// Set the filename (and open the files)
	_snprintf( fname, MAXFILENAME, "output%04d", aID );
	err = SetFilename( fname );
	if( err != UCCS_OK ) {
		fprintf( stderr, "ERROR: can't open output files.\n" );
	}

	// Output the startusecase heading
	_snprintf( iLineBuffer, MAXLINELENGTH, "<hr><h3>StartUseCase(%d)</h3><hr> \n", aID );
	OutputLine( iFile, iLineBuffer );
	OutputLine( iFileNoRefresh, iLineBuffer );
	OutputLine( iFileDebug, iLineBuffer );
}


/********************************************************************************
 *
 * PUBLIC METHOD: EndUsecase
 *
 ********************************************************************************/
void CHTMLOutput::EndUsecase( int aID, int aResult )
{
	// make the line
	_snprintf( iLineBuffer, MAXLINELENGTH, "<h3>EndUsecase(%d, %d)</h3><hr> \n", aID, aResult );

	// output the line
	OutputLine( iFile, iLineBuffer );
	OutputLine( iFileNoRefresh, iLineBuffer );
	OutputLine( iFileDebug, iLineBuffer );
}


/********************************************************************************
 *
 * PUBLIC METHOD: CompletedScript
 *
 ********************************************************************************/
void CHTMLOutput::CompletedScript()
{
	// create the line
	_snprintf( iLineBuffer, MAXLINELENGTH, "<hr><h3>All script commands have been executed</h3>\n" );
	
	// output the line
	OutputLine( iFile, iLineBuffer );
	OutputLine( iFileNoRefresh, iLineBuffer );
	OutputLine( iFileDebug, iLineBuffer );

	// set the flag (which at the moment has no effect?)
	iCompletedScriptCalled = true;
}


/********************************************************************************************
 *
 * PUBLIC METHOD: StartServiceResult 
 *
 *******************************************************************************************/
void CHTMLOutput::StartServiceResult( int aServiceID, char *aServiceHost, int aStubReturnValue, int aErrorCode, int aUnused )
{
	char error_tag[MAXLINELENGTH];

	// make the appropriate error tag
	if( aStubReturnValue == ERR_NONE ) {
		error_tag[0] = 0;
	} else {
		_snprintf( error_tag, MAXLINELENGTH, "<font color=\"red\" size=\"+1\">ERROR: </b></font>" );
	}

	// create the log entry
	_snprintf( iLineBuffer, MAXLINELENGTH, "<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%sStartServiceResult</b>(%d, %s, %s)\n", error_tag, aServiceID, aServiceHost, GetPenstdErrorString(aStubReturnValue) );

	// print the entry in the debug log
	OutputLine( iFileDebug, iLineBuffer );

	// print the entry in the other logs if there is an error
	if( aStubReturnValue != ERR_NONE ) {
		OutputLine( iFile, iLineBuffer );
		OutputLine( iFileNoRefresh, iLineBuffer );
	}
}


/********************************************************************************************
 *
 * PUBLIC METHOD: StopServiceResult 
 *
 *******************************************************************************************/
void CHTMLOutput::StopServiceResult( int aServiceID, char *aServiceHost, int aStubReturnValue, int aErrorCode, int aUnused )
{
	char error_tag[MAXLINELENGTH];

	// make the appropriate error tag
	if( aStubReturnValue == ERR_NONE ) {
		error_tag[0] = 0;
	} else {
		_snprintf( error_tag, MAXLINELENGTH, "<font color=\"red\" size=\"+1\">ERROR: </b></font>" );
	}

	// create the log entry
	_snprintf( iLineBuffer, MAXLINELENGTH, "<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%sStopServiceResult</b>(%d, %s, %s)\n", error_tag, aServiceID, aServiceHost, GetPenstdErrorString(aStubReturnValue) );

	// print the entry in the debug log
	OutputLine( iFileDebug, iLineBuffer );

	// print the entry in the other logs if there is an error
	if( aStubReturnValue != ERR_NONE ) {
		OutputLine( iFile, iLineBuffer );
		OutputLine( iFileNoRefresh, iLineBuffer );
	}
}


/********************************************************************************************
 *
 * PUBLIC METHOD: ExecuteString
 *
 *******************************************************************************************/
void CHTMLOutput::ExecuteString( char *aCommandLine )
{
	char *nptr;

	// remove the newline from the command line
	nptr = strchr( aCommandLine, '\n' );
	if( nptr != NULL ) {
		*nptr = 0;
	}

	// before the debug entry we put a line 
	OutputLine( iFileDebug, "<hr>\n" );

	// create the entry line
	_snprintf( iLineBuffer, MAXLINELENGTH, "&nbsp;&nbsp;&nbsp;<img src=\"bullet.gif\" width=\"8\" height=\"8\" ><b>&nbsp; %d - ExecuteString</b>( %s )\n", iCounter, aCommandLine );
	iCounter++;

	// print the entry into all logs
	OutputLine( iFile, iLineBuffer );
	OutputLine( iFileDebug, iLineBuffer );
	OutputLine( iFileNoRefresh, iLineBuffer );
}


/********************************************************************************
 *
 * PUBLIC METHOD: ExecuteCommand
 *
 ********************************************************************************/
void CHTMLOutput::ExecuteCommand( TUccsCommand aCommand, CDataRecord *aArgs )
{
	// make sure the passed record is valid
	if ( aArgs == NULL )
		return;
	
	// print the record
	fprintf( iFileDebug, "<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>\n");
	OutputDataRecord( iFileDebug, aArgs, RT_COMMAND, aCommand );

	// print an empty line to make things pretty
	OutputLineBreak( iFileDebug );
}


/********************************************************************************************
 *
 * PUBLIC METHOD: ExecuteCommandReply
 *
 *******************************************************************************************/
void CHTMLOutput::ExecuteCommandReply( CDataRecord *aReply )
{	
	int completion_code, err;

	// if there is no valid reply record then just return
	if( aReply == NULL ) {
		return;
	}

	// print the record
	OutputLineBreak( iFileDebug );
	OutputDataRecord( iFileDebug, aReply, RT_COMMAND_REPLY, UC_INVALID );
	OutputLineBreak( iFileDebug );

	// raise an error if the completion code isn't success then this is an error
	err = aReply->GetFieldAsInt( STD_REPLY_FIELD_REQUESTCOMPLETIONCODE, &completion_code );
	assert( err == UCCS_OK );
	if( completion_code != ERR_NONE ) {
		Error( UCCS_CLIENT_ERROR, GetPenstdErrorString(completion_code) );
	}
}


/********************************************************************************
 *
 * PUBLIC METHOD: Error
 *
 ********************************************************************************/
void CHTMLOutput::Error( int aErrorCode, char *aErrorString )
{
	char custom_error_string[MAXLINELENGTH];

	// set a default error string
	if( aErrorString == NULL ) {
		aErrorString = DEFAULT_ERROR_STRING;
	}

	// if the error is UCCS_CLIENT_ERROR then customise the error string
	if( aErrorCode == UCCS_CLIENT_ERROR ) {
		_snprintf( custom_error_string, MAXLINELENGTH, "%s.", aErrorString );
		custom_error_string[0] = toupper( custom_error_string[0] );
		aErrorString = custom_error_string;
	}

	// create the entry line
	_snprintf( iLineBuffer, MAXLINELENGTH, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\" size=\"+1\"><b>ERROR:</b></font> %s (%d). %s\n", GetUccsErrorStringI(aErrorCode), aErrorCode, aErrorString );

	// add the line to all logs
	OutputLine( iFile, iLineBuffer );
	OutputLine( iFileNoRefresh, iLineBuffer );
	OutputLine( iFileDebug, iLineBuffer );
}


/********************************************************************************************
 *
 * PUBLIC METHOD: DisplayEnvironment
 *
 *******************************************************************************************/
void CHTMLOutput::DisplayEnvironment( CDataRecord *aEnvironment )
{
	// if there is no environment then return
	if( aEnvironment == NULL ) {
		return;
	}

	// print an empty line to make things pretty
	OutputLineBreak( iFile );
	OutputLineBreak( iFileDebug );
	OutputLineBreak( iFileNoRefresh );

	// print the record
	OutputDataRecord( iFile, aEnvironment, RT_ENVIRONMENT, UC_INVALID );
	OutputDataRecord( iFileDebug, aEnvironment, RT_ENVIRONMENT, UC_INVALID );
	OutputDataRecord( iFileNoRefresh, aEnvironment, RT_ENVIRONMENT, UC_INVALID );
}


/********************************************************************************************
 *
 * SECTION: Unused methods
 *
 *******************************************************************************************/
/********************************************************************************************
 *
 * PUBLIC METHOD: ExecuteStringResult 
 *
 *******************************************************************************************/
void CHTMLOutput::ExecuteStringResult( int aUccsErrorcode )
{
}
void CHTMLOutput::ExecuteCommandResult( int aUccsErrorcode )
{
}


/********************************************************************************************
 *
 * SECTION: Removed API Calls
 *
 *******************************************************************************************/
void CHTMLOutput::DisplayLastCommand( CDataRecord *aCommand )
{
	assert( !"Removed API" );
}
void CHTMLOutput::DisplayLastReply( CDataRecord *aResult )
{
	assert( !"Removed API" );
}

void CHTMLOutput::DisplayHelp()
{
	assert( !"Removed API" );
}


/********************************************************************************************
 *
 * SECTION: Helper Methods
 *
 *******************************************************************************************/
/********************************************************************************************
 *
 * PRIVATE METHOD: OutputLine 
 *
 *******************************************************************************************/
void CHTMLOutput::OutputLine( FILE *aFile, char *aLine )
{
	// check the file pointer is valid
	if( aFile == NULL ) {
		return;
	}

	// print the line to the file
	fprintf( aFile, "%s", aLine );

	// fflush to file
	fflush( aFile );
}


/**************************************************************************************
 *
 * PRIVATE METHOD: OutputBeginTags
 *
 *************************************************************************************/
void CHTMLOutput::OutputBeginTags()
{
	// get the current system time to put in the header
	time_t ltime;
	time( &ltime );
	
	// check that the files exist
	if( (iFile == NULL) || (iFileDebug == NULL) || (iFileNoRefresh == NULL) ) {
		return;
	}

	// output the standard header for the files
	OutputLine(iFile,"<html> \n");
	OutputLine(iFile,"<head> \n");
	OutputLine(iFile, "<meta HTTP-EQUIV=\"Refresh\" CONTENT=\"5\">\n" );
	fprintf(iFile,"<title>%s</title> \n", iLogName);
	OutputLine(iFile,"</head> \n");
	OutputLine(iFile,"<body> \n");
	OutputLine(iFile,"<pre> \n");
	fprintf(iFile,"<hr>\n<h1>%s</h1> \n", iLogName);
	fprintf(iFile, "<h4>%s</h4>\n", ctime( &ltime ));
	fflush( iFile );

	OutputLine(iFileDebug,"<html> \n");
	OutputLine(iFileDebug,"<head> \n");
	fprintf(iFileDebug,"<title>%s</title> \n", iDebugLogName);
	OutputLine(iFileDebug,"</head> \n");
	OutputLine(iFileDebug,"<body> \n");
	OutputLine(iFileDebug,"<pre> \n");
	fprintf(iFileDebug,"<hr>\n<h1>%s</h1> \n", iDebugLogName);
	fprintf(iFileDebug, "<h4>%s</h4>\n", ctime( &ltime ));
	fflush( iFileDebug );

	OutputLine(iFileNoRefresh,"<html> \n");
	OutputLine(iFileNoRefresh,"<head> \n");
	fprintf(iFileNoRefresh,"<title>%s</title> \n", iLogNameNoRefresh);
	OutputLine(iFileNoRefresh,"</head> \n");
	OutputLine(iFileNoRefresh,"<body> \n");
	OutputLine(iFileNoRefresh,"<pre> \n");
	fprintf(iFileNoRefresh,"<hr>\n<h1>%s</h1> \n", iLogNameNoRefresh);
	fprintf(iFileNoRefresh, "<h4>%s</h4>\n", ctime( &ltime ));
	fflush( iFileNoRefresh );
}


/********************************************************************************
 *
 * PRIVATE METHOD: OutputDataRecord -- outputs a data CDataRecord to the passed
 * log file. Records are output as tables
 *
 ********************************************************************************/
void CHTMLOutput::OutputDataRecord( FILE* aFile, CDataRecord *aRecord, TRecordType aRecordType, TUccsCommand aCommandType )
{
	int standard_row_count, i;
	char *multiline_value;
	char *value;
	char *background_colour;
	char *fontsize;
	CDataField *field;

	// check that a real record is passed
	if ( aRecord == NULL ) {
		return;
	}

	// check the record type -- this is an internal thing so can just assert
	assert( (aRecordType > RT_INVALID) && (aRecordType < RT_COUNT) );

	// print the table header
	_snprintf( iLineBuffer, MAXLINELENGTH, "<br>" );
	OutputLine( aFile, iLineBuffer );
	_snprintf( iLineBuffer, MAXLINELENGTH, "<table border=\"2\" cellpadding=3 cellspacing=\"0\" bordercolor=\"%s\">", iRecordTableBorderColour[aRecordType] );
	OutputLine( aFile, iLineBuffer );

	// print the table title 
	iRecordTableTitle[RT_COMMAND] = (char*)GetScriptCommandString( aCommandType );
	STRINGIFY_NULL( iRecordTableTitle[RT_COMMAND] );
	_snprintf( iLineBuffer, MAXLINELENGTH, "<tr bgcolor=\"%s\"><td colspan=2 align=\"center\"><font size=\"3\"><b>%s</b></font></tr>", iRecordTableTitleBackgroundColour[aRecordType], iRecordTableTitle[aRecordType] );
	OutputLine( aFile, iLineBuffer );

	// find out how many fields in this record are standard rows - AE: unsure why the 'if' is needed
	standard_row_count = iStandardRowCount[aRecordType];
	if( (aRecordType == RT_COMMAND) && (aCommandType != UC_RUNCMD) ) {
		standard_row_count = 0;
	}

	// print all rows -- first X rows are standard and have a light grey background to distinguish them
	for( field = aRecord->GetFirstField(), i = 0; field != NULL; i++ ) {

		// get the background colour for the row - standard - warnings
		background_colour = "";
		if( i < standard_row_count ) {			
			background_colour = iStandardRowColour[aRecordType];
		}
		if( (aRecordType == RT_COMMAND_REPLY) && (IsFailedResult(field)) ) {
			background_colour = iNonZeroResultColour;
		}

		// get the string value for the field and check for multi-line entries
		value = field->GetStrValue();
		multiline_value = strchr( value, '\n' );
		
		// set the font size
		fontsize = "1";
		if( multiline_value != NULL ) {
			fontsize = "3";
		} 

		// print the row - datarecord entries (especially log entries) can be longer than 1k so we break things up
		_snprintf( iLineBuffer, MAXLINELENGTH, "<tr%s><td><font size=\"1\">%s</font></td><td><font size=\"%s\">", background_colour, field->GetName(), fontsize );
		OutputLine( aFile, iLineBuffer ); 

		// if multiline then put in the <pre> tags to make newlines come out
		if( multiline_value != NULL ) {
			OutputLine( aFile, "<pre>" );
		}

		// print actual data
		OutputLine( aFile, value );
		
		// if multiline then put in the </pre> tags to make newlines come out
		if( multiline_value != NULL ) {
			OutputLine( aFile, "</pre>" );
		}

		// get the next field
		field = aRecord->GetNextField();
	}

	// end the table
	_snprintf( iLineBuffer, MAXLINELENGTH, "</table>" );
	OutputLine( aFile, iLineBuffer );
} 


/********************************************************************************
 *
 * PRIVATE METHOD: IsResult
 *
 ********************************************************************************/
int CHTMLOutput::IsFailedResult( CDataField *aField )
{
	int match, value;
	char *field_name;

	// check parameters
	if( aField == NULL ) {
		return 0;
	}

	// get the name of the field and see if this is a result
	field_name = aField->GetName();
	match = strcmp( field_name, STD_REPLY_FIELD_RESULT );
	if( match != 0 ) {
		return 0;
	}

	// this is a RESULT field, see if it is none-zero
	value = aField->GetIntValue();
	if( value == ERR_NONE ) {
		return 0;
	}

	// this is a failed result
	return 1;
}


/********************************************************************************
 *
 * PRIVATE METHOD: OutputLineBreak
 *
 ********************************************************************************/
void CHTMLOutput::OutputLineBreak( FILE* aFile )
{
	_snprintf( iLineBuffer, MAXLINELENGTH, "<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>\n" );
	OutputLine( aFile, iLineBuffer );
}

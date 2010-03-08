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
* CConsolePort
* System Includes
*
*/



#include <stdio.h>
#include <assert.h>

/*********************************************************************
 *
 * Local Includes
 *
 ********************************************************************/
#include "CConsolePort.h"
#include "../Core/UCCS_ErrorCodes.h"
#include "CProtocolTypes.h"

/*********************************************************************
 *
 * File-scope statics
 *
 ********************************************************************/
char *gKnownCommands[] = { "RUBBISH", "startusecase", "signal", "rendezvous", "endusecase", "quit", "waitforsignal", "runcmd", NULL };


/*********************************************************************
 *
 * Construction
 *
 ********************************************************************/
CConsolePort::CConsolePort()
	: iReadingData(0), iUcid(0), iCommand(0)
{
	memset( iCommandLine, 0x00, sizeof(iCommandLine) );
}


CConsolePort::~CConsolePort()
{
}


/*********************************************************************
 *
 * OpenPort() -- 
 *
 ********************************************************************/
int CConsolePort::OpenPort( char *port )
{
	return 0;
}


/*********************************************************************
 *
 * ClosePort() --
 *
 ********************************************************************/
void CConsolePort::ClosePort()
{
}

/*********************************************************************
 *
 *  ReadData() 
 *
 ********************************************************************/
int CConsolePort::ReadData( char* aBuff, int *aSize)
{
	int rand_uid = 1;
	int eng_result = 0;

	TEndUsecaseRequest* endusecase;
	TRunCommandRequest* runcommand;
	// Note we are using the startusecase structure for all the commands, as they 
	// are identical in the data they contain.
	TStartUsecaseRequest* othercases;

	// If the command we are reading the header for is Endusecase, then we 
	// have 8 bytes of data as opposed to 4, like the rest of the commands.
	// Therefore, need to distinguish between the commands.

	// If the Endusecase case
	if (iCommand == 4)
	{
		endusecase = (TEndUsecaseRequest*)aBuff;
		endusecase->iUsecaseID = iUcid;
		endusecase->iResult = eng_result;
		*aSize = sizeof(endusecase);
		iReadingData = 0;
		return 0;
	}
	else if( iCommand == 7 )
	{
		runcommand = (TRunCommandRequest*)aBuff;
		if( strlen(iCommandLine) > 0 )
		{
			memset( runcommand->iCommandLine, 0x00, sizeof(MAXCOMMANDLINELEN) );
			strcpy( runcommand->iCommandLine, iCommandLine );
			memset( iCommandLine, 0x00, sizeof(iCommandLine) );
		}
		runcommand->iResult = eng_result;
		*aSize = sizeof(runcommand);
		iReadingData = 0;
		return 0;
	}
	else 
	{	
		othercases = (TStartUsecaseRequest*)aBuff;
		othercases->iUsecaseID = iUcid; 
		*aSize = sizeof(othercases);
		iReadingData = 0;
		return 0;
	}
}

/*********************************************************************
 *
 *  ReadHeader() 
 *
 ********************************************************************/
int CConsolePort::ReadHeader( char* aBuff, int *aSize)
{
	int uid;
	int rand_uid = 1;
	int eng_result = 0;
	int ret;

	TPHeader *header = (TPHeader*)aBuff;

	do { 
	
		// Get the next command
		iCommand  = ParseNextLine( &uid );
		
		// If we have encountered an invalid command return 
		if( (iCommand == -2) ) 
		{
			return -1; //EInvalidCommand
		}
		
		header->iUid = rand_uid;
		ret = GetTPCommand(iCommand, &(header->iCmdID));
		if ( ret != 0 )
		{
			return -1; //EInvalidCommand
		}	

		// If the command is startusecase, for every other command we just ignore this
		if( iCommand == 1 ) 
		{
			iUcid = uid;
		}

		// If the command is endusecase - iDataLen is 8 - as currently we have 4 bytes 
		// for the uid and 4 for the result.
		if(iCommand == 4 )
		{
			header->iDataLen = E8DataLen;
		}
		else 
		{
			header->iDataLen = E4DataLen;
		}	
		
		// 4 bytes each for each field in the header.
		*aSize = sizeof(*header);
	} while( iCommand == -1 );

	return 0;
}

 
/*********************************************************************
 *
 *  ReceiveBytes() 
 *
 ********************************************************************/
int CConsolePort::ReceiveBytes( char* aBuff, int *aSize)
{
	int ret = 0;

	// If we are just the data part of the frame iReadingFlag is true
	if( iReadingData == 1 ) 
	{
		ret = ReadData( aBuff, aSize );
		if ( ret != 0 )
			return -1;
	
	}
	else
	{
		// Else we are reading the header - so get the next command
		ret = ReadHeader( aBuff, aSize );
		if ( ret != 0 )
			return -1;

		// Set the flag to true now that we have read the header,
		// so that the data part is read in the next iteration.
		iReadingData = 1;
	}
	
	// Return 
	return ret;
}


/*********************************************************************
 *
 *  SendBytes() 
 *
 ********************************************************************/
int CConsolePort::SendBytes( char *aBuff, int *aSize )
{
	int data_base_index;
	TPHeader* header;
	int *engine_result, *script_result;

	// extract the info from the buffer
	header = (TPHeader*)aBuff;
	data_base_index = sizeof(TPHeader);
	engine_result = ((int*)(&aBuff[data_base_index]));
	script_result = ((int*)(&aBuff[data_base_index+4]));

	// print out the information for the endusecase command
	if( header->iCmdID == CMD_REP_ENDUSECASEID ) {
		assert( header->iDataLen == E8DataLen );
		if( (*script_result) == 0 ) {
			fprintf( stdout, "  %s - No script errors occured.\n\n", GetUccsErrorStringI(*engine_result) );
		} else if( ((*script_result) > UCCS_OK) && ((*script_result) < UCCS_LAST) ) {
			fprintf( stdout, "  %s - %s\n\n", GetUccsErrorStringI(*engine_result), GetUccsErrorStringI(*script_result) );
		} else {
			fprintf( stdout, "  %s - %d\n\n", GetUccsErrorStringI(*engine_result), *script_result );
		}
		*aSize = 8;
	}

	// print out the information for all other commands 
	if( header->iCmdID != CMD_REP_ENDUSECASEID ) 
	{
		assert ( header->iDataLen == E4DataLen );
		if (*engine_result < 0)
			fprintf(stdout, "Error! %s (%d)\n\n", GetUccsErrorStringI(*engine_result), *engine_result);
		else
			fprintf(stdout, "Success! (0x%08x)\n\n", *engine_result);
		*aSize = 4;
	}

	// done
	return 0;
}


/*********************************************************************
 *
 * ParseNextLine() 
 *
 ********************************************************************/
int CConsolePort::ParseNextLine( int *uid )
{
	char *cp;
	char cline[MAXLINESIZE];
	char command[MAXLINESIZE];
	int token_count;
	int i;
	int match;

	// check param
	assert( uid != NULL );

	// write a litte prompt
	fprintf( stdout, "> " );

	// read the line 
	cp = fgets( cline, MAXLINESIZE, stdin );
	if( cp == NULL ) {
		return -2;
	}

	// tokenise the line
	token_count = sscanf( cline, "%s %d\n", command, uid );

	// if there are no tokens then leave
	if( token_count <= 0 ) {
		return -1;
	}

	// otherwise try and match the token
	for( i = 0; gKnownCommands[i] != NULL; i++ ) {
		match = strcmp( gKnownCommands[i], command );
		if( match == 0 ) {
			break;
		}
	}

	// If runcmd then set the command line data
	if( i == 7 )
	{
		if( strlen(iCommandLine) > 0 )
		{
			memset( iCommandLine, 0x00, sizeof(iCommandLine) );
		}
		strcpy( iCommandLine, cline );
	}

	// check that a command was found
	if( gKnownCommands[i] == NULL ) {
		fprintf( stderr, "WARNING: unknown command '%s' (ignoring)\n\n", command );
		return -1;
	}

	// otherwise we have a command -- if startusecase then make sure that it has a uid
	if( (i == 1) && (token_count == 1) ) {
		fprintf( stderr, "WARNING: command '%s' requires a use case ID argument\n\n", gKnownCommands[i] );
		return -1;
	}

	// OK -- return the command
	return i;
}


/*********************************************************************
 *
 * GetTPCommand() 
 *
 ********************************************************************/
int CConsolePort::GetTPCommand( int aCommandID, TPCommand* aTPCommand )
{

	switch (aCommandID)
	{
		case 1:		// startusecase
			*aTPCommand = CMD_REQ_STARTUSECASEID;
			break;
		
		case 2:		// signal
			*aTPCommand = CMD_REQ_SIGNALID;
			break;
		
		case 3:		// rendezvous
			*aTPCommand = CMD_REQ_RENDEZVOUSID;
			break;

		case 4:		// endusecase
			*aTPCommand = CMD_REQ_ENDUSECASEID;
			break;

		case 5:		// quit	
			*aTPCommand = CMD_QUITID;
			break;

		case 6:		// waitforsignal
			*aTPCommand = CMD_REQ_WAITID;
			break;

		case 7:		// runcmd
			*aTPCommand = CMD_REQ_RUNCOMMAND;
			break;

		default:
			return -1;
	}

	return 0;
}

	

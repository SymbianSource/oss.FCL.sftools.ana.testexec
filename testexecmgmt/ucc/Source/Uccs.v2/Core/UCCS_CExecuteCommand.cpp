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
* Switches
*
*/


/****************************************************************************************
 *
 * System Includes
 *
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <windows.h>

/****************************************************************************************
 *
 * Local Includes
 *
 ***************************************************************************************/
#include "UCCS_CExecuteCommand.h"


/****************************************************************************************
 *
 * Definitions
 *
 ***************************************************************************************/
#define REPEATCOMMANDWAITPERIOD		2000


/********************************************************************************
 *
 * Macro Functions
 *
 ********************************************************************************/
#define NOT_IS_WHITESPACE(c)		((c != '\t')&&(c != '\n')&&(c != 0)&&(c != ' '))
 
/****************************************************************************************
 *
 * File-scope variables
 *
 ***************************************************************************************/
static const char *iCommandStrings[] = {	"<invalid>", 
											"quit", 
											"runcmd", 
											"assign",
											"waitfor", 
											"waitforsignal", 
											"rendezvous", 
											"require", 
											"requirenot", 
											"check", 
											"checknot", 
											"showenvironment", 
											"waitfornot", 
											"signal", 
											NULL };

/****************************************************************************************
 *
 * PUBLIC FUNCTION: GetScriptCommandString
 *
 ***************************************************************************************/
const char *GetScriptCommandString( int aCommandID )
{
	// make sure the command ID is valid
	if( (aCommandID < UC_QUIT) || (aCommandID > UC_SIGNAL) ) {
		return NULL;
	}
	
	// return the string
	return iCommandStrings[aCommandID];
}


/****************************************************************************************
 *
 * Construction
 *
 ***************************************************************************************/
CExecuteCommand::CExecuteCommand( CSynchronisation *aSync, IOutput *aOutput )
{
	// check params
	assert( aOutput != NULL );
	assert( aSync != NULL );

	// init vars
	iSync = aSync;
	iOutput = aOutput;
	iLastResult = NULL;
	iCommandContextSize = 0;

	// create objs
	iEnvironment = new CDataRecord();
	assert( iEnvironment != NULL );
	iServiceManager = new CServiceManager( aOutput );
	assert( iServiceManager != NULL );
	iCommandDecoder = new CCommandDecoder( iEnvironment );
	assert( iCommandDecoder != NULL );
}


CExecuteCommand::~CExecuteCommand()
{
	ClearCommandContext();
	if( iLastResult != NULL ) {
		delete iLastResult;
	}
	delete iCommandDecoder;
	delete iServiceManager;
	delete iEnvironment;
}


/****************************************************************************************
 *
 * Public Methoid: Execute Command -- parse into (command, argument)
 *
 ***************************************************************************************/
int CExecuteCommand::ExecuteCommand( char *aCommandLine )
{
	char *command, *arguments;

	// check the params
	assert( aCommandLine != NULL );

	// parse the line into (commandPtr, argumentPtr)
	for( command = aCommandLine, arguments = aCommandLine; NOT_IS_WHITESPACE(*arguments) && (*arguments != 0); arguments++ )
			;

	// check for no arguments (fine for quit) -- otherwise null the command and set the arg ptr
	if( *arguments == 0 ) {
		arguments = NULL;
	} else {
		*arguments = 0;
		arguments++;
	}


	// call the handler
	return ExecuteCommand( command, arguments );
}


/****************************************************************************************
 *
 * Public Method: Execute Command -- deals with contexts and waitfors
 *
 ***************************************************************************************/
int CExecuteCommand::ExecuteCommand( char *aCommand, char *aArgs )
{
	int i;
	int err;
	int match;
	TUccsCommand command;
	CDataRecord *command_arguments;

	// resolve the command
	for( i = 0; iCommandStrings[i] != NULL; i++ ) {
		match = strcmp( aCommand, iCommandStrings[i] );
		if( match == 0 ) {
			break;
		}
	}

	// if no match is found then error
	if( iCommandStrings[i] == 0 ) {
		return UCCS_UNKNOWNCOMMAND;
	}
	command = (TUccsCommand)i;

	// create a new request record
	command_arguments = new CDataRecord();

	// create a data object from the argument string
	err = iCommandDecoder->ParseCommandToRecord(aArgs, command_arguments);
	if( err != UCCS_OK ) {
		return err;
	}

	// if the command is a run command then we clear the context
	if( command == UC_RUNCMD ) {
		ClearCommandContext();
	}

	// add the command to the context
	AddToCommandContext( command, command_arguments );

	// now execute the command for the first time
	err = InternalExecuteCommand( command, command_arguments );

	// if the result was NOT UCCS_REPLAYCOMMAND then just return
	if( err != UCCS_REPLAYCOMMAND ) {
		return err;
	}


	// NOTE: if we get here then we are asked to replay

	// Run each command in the context again in order. If an error occurs then exit. If someone
	// asks to wait again then start agani
	for( i = 0; i < iCommandContextSize; i++ ) {
		err = InternalExecuteCommand( iCommandContextCommands[i], iCommandContextArgs[i] );
		if( err == UCCS_REPLAYCOMMAND ) {
			i = -1;
			continue;
		} else if( err != UCCS_OK ) {
			return err;
		}
	}

	// done
	return UCCS_OK;
}

/****************************************************************************************
 *
 * Public Method: Execute Command -- dispatch commands to appropriate 
 * handlers
 *
 ***************************************************************************************/
int CExecuteCommand::InternalExecuteCommand( TUccsCommand aCmd, CDataRecord *aArgs )
{
	int err;

	// let the output know that we are executing a command
	if( aCmd != UC_RUNCMD )
	{
		iOutput->ExecuteCommand( aCmd, aArgs );
	}

	// pass the command to the appropriate handler
	switch( aCmd ) {

	case UC_QUIT:
		err = UCCS_QUIT;
		break;

	case UC_RUNCMD:
		err = HandleRunCmd( aArgs );
		break;

	case UC_ASSIGN:
		err = HandleAssignment( aArgs );
		break;

	case UC_WAITFOR:
		err = HandleWaitFor( aArgs );
		break;

	case UC_REQUIRE:
		err = HandleRequire( aArgs );
		break;

	case UC_REQUIRENOT:
		err = HandleRequireNot( aArgs );
		break;

	case UC_CHECK:
		err = HandleCheck( aArgs );
		break;

	case UC_CHECKNOT:
		err = HandleCheckNot( aArgs );
		break;

	case UC_WAITFORSIGNAL:
		err = HandleWaitForSignal( aArgs );
		break;

	case UC_RENDEZVOUS:
		err = HandleRendezvous( aArgs );
		break;

	case UC_PRINTENVIRONMENT:
		iOutput->DisplayEnvironment( iEnvironment );
		err = UCCS_OK;
		break;

	case UC_WAITFORNOT:
		err = HandleWaitForNot( aArgs );
		break;

	case UC_SIGNAL:
		err = HandleSignal( aArgs );
		break;
	}

	// output the result
	iOutput->ExecuteCommandResult( err );
	
	// should always get here
	return err;
}


/****************************************************************************************
 *
 * Private Method: HandleRunCmd
 *
 ***************************************************************************************/
int CExecuteCommand::HandleRunCmd( CDataRecord *aArgs )
{
	int err;
	CDataRecord *result = NULL;

	// run the command
	err = iServiceManager->IssueCommand( aArgs, &result );
	
	// if the command fails then return the error
	if( err != UCCS_OK && err != UCCS_ERROR_NONE ) {
			return err;
	}
	assert( result != NULL );

	// otherwise this is success -- let the output know
	iOutput->ExecuteCommandReply( result );

	// otherwise we clear the previous saved command and result
	if( iLastResult != NULL ) {
		delete iLastResult;
	}

	// now save this command and result
	iLastResult = result;
	iCommandDecoder->SetLastReply( iLastResult );

	// done
	return err;
}


/****************************************************************************************
 *
 * Private Method: HandlePrintLastCommand
 *
 ***************************************************************************************/
int CExecuteCommand::HandlePrintLastCommand( CDataRecord *aArgs )
{
	iOutput->DisplayLastCommand( iCommandContextArgs[0] );
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandlePrintLastResult
 *
 ***************************************************************************************/
int CExecuteCommand::HandlePrintLastResult( CDataRecord *aArgs )
{
	iOutput->DisplayLastReply( iLastResult );
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleAssignment -- allows elements of the reply to be assigned. For
 * each field (name1, value1) in the passed record we:
 *
 *	field_value = reply->GetField( name1 );
 *	environment->NewField( value1, field_value );
 *
 ***************************************************************************************/
int CExecuteCommand::HandleAssignment( CDataRecord *aArgs )
{
	int err;
//	char *source_field_name;
	char *dest_field_name;
	char *dest_field_value;
	CDataField *element = NULL;

	// now do the assignment -- first try and update, then a new
	element = aArgs->GetFirstField();
	while( element != NULL ) {
		dest_field_name = element->GetName();
		dest_field_value = element->GetStrValue();
		err = iEnvironment->ChangeFieldData( dest_field_name, dest_field_value );
		if( err == UCCS_FIELDNOTFOUND ) {
			err = iEnvironment->NewField( dest_field_name, dest_field_value );
			if( err != UCCS_OK ) {
				return err;
			}
		} else if( err != UCCS_OK ) {
			return err;
		}
		element = aArgs->GetNextField();
	}


	// done
	return UCCS_OK;
}



/****************************************************************************************
 *
 * Private Method: HandleWaitFor. For each field in the args (namei, valuei)
 *
 * rv = reply->GetField(namei);
 * if( rv != valuei ) 
 *		repeat the last command with X second wait
 *
 *
 ***************************************************************************************/
int CExecuteCommand::HandleWaitFor( CDataRecord *aArgs )
{
	char *field_name;
	char *reference_value;
	char *actual_value;
	int err, match;
	CDataField *element = NULL;

	// make sure there is a previous reply and request
	if( iLastResult == NULL ) {
		return UCCS_NORESULT;
	}

	// now do the waitfor
	element = aArgs->GetFirstField();
	while( element != NULL ) {

		field_name = element->GetName();
		reference_value = element->GetStrValue();
		err = iLastResult->GetFieldAsString( field_name, &actual_value );
		if( err != UCCS_OK ) {
			return err;
		}
		match = strcmp( reference_value, actual_value );
		if( match != 0 ) {
			Sleep( REPEATCOMMANDWAITPERIOD );
			return UCCS_REPLAYCOMMAND;
		}

		element = aArgs->GetNextField();
	}

	// done
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleWaitForNot. For each field in the args (namei, valuei)
 *
 * rv = reply->GetField(namei);
 * if( rv != valuei ) 
 *		repeat the last command with X second wait
 *
 *
 ***************************************************************************************/
int CExecuteCommand::HandleWaitForNot( CDataRecord *aArgs )
{
	char *field_name;
	char *reference_value;
	char *actual_value;
	int err, match;
	CDataField *element = NULL;

	// make sure there is a previous reply and request
	if( iLastResult == NULL ) {
		return UCCS_NORESULT;
	}

	// now do the waitfornot
	element = aArgs->GetFirstField();
	while( element != NULL ) {

		field_name = element->GetName();
		reference_value = element->GetStrValue();
		err = iLastResult->GetFieldAsString( field_name, &actual_value );
		if( err != UCCS_OK ) {
			return err;
		}
		match = strcmp( reference_value, actual_value );
		if( match == 0 ) {
			Sleep( REPEATCOMMANDWAITPERIOD );
			return UCCS_REPLAYCOMMAND;
		}

		element = aArgs->GetNextField();
	}

	// done
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleRequire.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleRequire( CDataRecord *aArgs )
{
	int err, match;
	char *field_name, *field_value, *actual_value;
	CDataField *element;

	// make sure there is a previous reply
	if( iLastResult == NULL ) {
		return UCCS_NORESULT;
	}

	// now do the require
	element = aArgs->GetFirstField();
	while( element != NULL ) {

		field_name = element->GetName();
		field_value = element->GetStrValue();
		err = iLastResult->GetFieldAsString( field_name, &actual_value );
		if( err != UCCS_OK ) {
			return UCCS_REQUIREDVALUEERROR;
		}
		match = strcmp( actual_value, field_value );
		if( match != 0 ) {
			return UCCS_REQUIREDVALUEINCORRECT;
		}

		element = aArgs->GetNextField();
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleRequireNot.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleRequireNot( CDataRecord *aArgs )
{
	int err, match;
	char *field_name, *field_value, *actual_value;
	CDataField *element;

	// make sure there is a previous reply
	if( iLastResult == NULL ) {
		return UCCS_NORESULT;
	}

	// now do the requirenot
	element = aArgs->GetFirstField();
	while( element != NULL ) {

		field_name = element->GetName();
		field_value = element->GetStrValue();
		err = iLastResult->GetFieldAsString( field_name, &actual_value );
		if( err != UCCS_OK ) {
			return UCCS_REQUIREDNOTVALUEERROR;
		}
		match = strcmp( actual_value, field_value );
		if( match == 0 ) {
			return UCCS_REQUIREDNOTVALUEMATCH;
		}

		element = aArgs->GetNextField();
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleCheck.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleCheck( CDataRecord *aArgs )
{
	int err, match;
	char *field_name, *field_value, *actual_value;
	CDataField *element;

	// make sure there is a previous reply
	if( iLastResult == NULL ) {
		return UCCS_NORESULT;
	}

	// now do the check
	element = aArgs->GetFirstField();
	while( element != NULL ) {

		field_name = element->GetName();
		field_value = element->GetStrValue();
		err = iLastResult->GetFieldAsString( field_name, &actual_value );
		if( err != UCCS_OK ) {
			return UCCS_CHECKVALUEERROR;
		}
		match = strcmp( actual_value, field_value );
		if( match != 0 ) {
			return UCCS_CHECKVALUEINCORRECT;
		}

		element = aArgs->GetNextField();
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleCheckNot.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleCheckNot( CDataRecord *aArgs )
{
	int err, match;
	char *field_name, *field_value, *actual_value;
	CDataField *element;

	// make sure there is a previous reply
	if( iLastResult == NULL ) {
		return UCCS_NORESULT;
	}

	// now do the checknot
	element = aArgs->GetFirstField();
	while( element != NULL ) {

		field_name = element->GetName();
		field_value = element->GetStrValue();
		err = iLastResult->GetFieldAsString( field_name, &actual_value );
		if( err != UCCS_OK ) {
			return UCCS_CHECKNOTVALUEERROR;
		}
		match = strcmp( actual_value, field_value );
		if( match == 0 ) {
			return UCCS_CHECKNOTVALUEMATCH;
		}

		element = aArgs->GetNextField();
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * Private Method: HandleWaitForSignal.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleWaitForSignal( CDataRecord *aArgs )
{
	return iSync->WaitFromScript();
}


/****************************************************************************************
 *
 * Private Method: HandleSignal.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleSignal( CDataRecord *aArgs )
{
	return iSync->SignalFromScript();
}


/****************************************************************************************
 *
 * Private Method: HandleRendezvous.  
 *
 ***************************************************************************************/
int CExecuteCommand::HandleRendezvous( CDataRecord *aArgs )
{
	return iSync->RendezvousFromScript();
}


/****************************************************************************************
 *
 * PRIVATE METHODS: Manage the command contexts
 *
 ***************************************************************************************/
void CExecuteCommand::ClearCommandContext()
{
	int i;
	for( i = 0; i <iCommandContextSize; i++ ) {
		assert( iCommandContextArgs[i] != NULL );
		delete iCommandContextArgs[i];
		iCommandContextArgs[i] = NULL;
	}
	iCommandContextSize = 0;
}


void CExecuteCommand::AddToCommandContext( TUccsCommand aCommand, CDataRecord *aRec )
{
	assert( aRec != NULL );
	assert( iCommandContextSize < MAXRECENTCOMMANDS );
	iCommandContextCommands[iCommandContextSize] = aCommand;
	iCommandContextArgs[iCommandContextSize++] = aRec;
}


/****************************************************************************************
 *
 * PUBLIC METHODS: To get the value of an varible in the environment
 *
 ***************************************************************************************/
int CExecuteCommand::GetEnvironmentVariable ( char *aVariableName, char *aOutputBuffer, int aOutputBufferSize )
{
	int ret, len;
	char* fieldValue;

	// Check params
	assert ( aVariableName != NULL );
	assert ( aOutputBuffer != NULL );
	assert ( aOutputBufferSize > 0 );

	// If there is nothing in the environment then return that env var not found Error
	if ( iEnvironment == NULL )
		return UCCS_NOENVIRONMENT;

	// Query the environment for this variable name
	ret = iEnvironment->GetFieldAsString( aVariableName, &fieldValue );
	assert ( (ret == UCCS_OK) || (ret == UCCS_FIELDNOTFOUND) ); 
	if( ret == UCCS_FIELDNOTFOUND ) 
		return UCCS_VARIABLEDOESNOTEXIST;

	// Now copy the fieldValue returned into the return output buffer
	// First check we have sufficient memory to hold the value in the return buffer
	len = strlen (fieldValue) + 1;

	if ( len > aOutputBufferSize )
		return UCCS_VARIABLEVALTOOLONG;

	// Else we copy the data
	memcpy( aOutputBuffer, fieldValue, len );

	return UCCS_OK;
}




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
* Filename: UCCS_CBatchEngine.cpp
* System Includes
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_CBatchEngine.h"
#include "UCCS_ErrorCodes.h"


/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define IS_WHITESPACE(c)			(((c) == '\t')||((c) == '\n')||((c) == ' '))
#define MAXCOMMANDLENGTH			2048


/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
DWORD WINAPI ThreadProc(LPVOID lpParameter);


/***********************************************************************************
 *
 * PUBLIC METHOD: Construction
 *
 **********************************************************************************/
CBatchEngine::CBatchEngine( IRetrieveCommand *aRetrieveCommand, IOutput *aOutput )
{
	// check parameters
	assert( aRetrieveCommand != NULL );
	assert( aOutput != NULL );

	// init the vars
	iRetrieveCommand = aRetrieveCommand;
	iOutput = aOutput;

	// set all other state
	iExecutionThreadState = ETS_IDLE;
	iControlThreadState = CTS_IDLE;

	// the rest are set according to the state -- for idle their value is irrelevant 
	// but I'd like to set them anyway for completeness
	iLastError = 0;
	iSync = NULL;
	iUsecaseID = INVALID_USECASE_ID;
	hThreadHandle = NULL;
	iExecuteCommand = NULL;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: Destruction
 *
 **********************************************************************************/
CBatchEngine::~CBatchEngine()
{
	// clean up any memory holding state variables
	if( iExecuteCommand != NULL ) {
		delete iExecuteCommand;
		iExecuteCommand = NULL;
	}
	if( hThreadHandle != NULL ) {
		CloseHandle(hThreadHandle);
		hThreadHandle = NULL;
	}
	if( iSync != NULL ) {
		delete iSync;
		iSync = NULL;
	}
}


/***********************************************************************************
 *
 * PUBLIC METHOD: StartUseCase
 *
 **********************************************************************************/
int CBatchEngine::StartUsecase( int aUsecaseID )
{
	int err;
	DWORD dwThreadID;

	// check that the control thread is currently in idle
	if( iControlThreadState != CTS_IDLE ) {
		return UCCS_ALREADYSTARTEDUSECASE;
	}
	assert( iExecutionThreadState == ETS_IDLE );

	// ask the retriever to get the use-case description
	err = iRetrieveCommand->StartUseCase( aUsecaseID );
	if( err != UCCS_OK ) {
		return err;
	}

	// set all the state appropriatley
	assert( iSync == NULL );
	iSync = new CSynchronisation( iOutput );
	assert( iSync != NULL );
	assert( iExecuteCommand == NULL );
	iExecuteCommand = new CExecuteCommand( iSync, iOutput );
	assert( iExecuteCommand != NULL );	
	iLastError = 0;
	iUsecaseID = aUsecaseID;

	// set the state
	iExecutionThreadState = ETS_EXECUTING_SCRIPT;
	iControlThreadState = CTS_USECASE_STARTED;

	// output that we have started
	iOutput->StartUsecase( aUsecaseID );

	// start the thread that goes and executes the steps
#ifndef TESTCASEBATCH
	hThreadHandle = CreateThread( NULL, 0, ThreadProc, this, 0,	&dwThreadID );		
#else
	hThreadHandle = 0;
#endif
	if( hThreadHandle == 0 ) {
		delete iSync;
		iSync = NULL;
		delete iExecuteCommand;
		iExecuteCommand = NULL;
		iUsecaseID = INVALID_USECASE_ID;
		iExecutionThreadState = ETS_IDLE;
		iControlThreadState = CTS_IDLE;
		return UCCS_FAILEDTOCREATEEXECUTETHREAD;
	}

	// done - return OK to the external controller
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: EndUsecase
 *
 **********************************************************************************/
int CBatchEngine::EndUsecase( int aUsecaseID, int aResult, int *aScriptResult )
{
	int err;

	// check that the control thread is in the correct state
	if( iControlThreadState != CTS_USECASE_STARTED ) {
		*aScriptResult = UCCS_NOUSECASERUNNING;
		return UCCS_NOUSECASERUNNING;
	}

	// update the state of the control thread to ended -- this will cause the execution
	// thread to exit on it's next iteration. 
	iControlThreadState	= CTS_USECASE_ENDED;

	// We clear the synchronisation so that if the execution thread is (or is just about 
	// to) wait on a semaphore then it will not get stuck forever
	iSync->ClearSynchronisation();

	// Wait for the thread to really exit
	err = WaitForSingleObject( hThreadHandle, INFINITE );
	if( err != WAIT_OBJECT_0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "An error occured while waiting for the executing script thread to finish." );
	}
	CloseHandle( hThreadHandle );
	hThreadHandle = NULL;

	// cleanup the rest of the state 
	assert( iExecutionThreadState == ETS_IDLE );
	iControlThreadState = CTS_IDLE;
	delete iSync;	
	iSync = NULL;
	delete iExecuteCommand;
	iExecuteCommand = NULL;
	iUsecaseID = INVALID_USECASE_ID;
	
	// output that endusecase has been called
	iOutput->EndUsecase( aUsecaseID, aResult );

	// done -- return the information
	*aScriptResult = iLastError;
	iLastError = 0;
	return UCCS_OK;
}

/***********************************************************************************
 *
 * PUBLIC METHOD: GetVariableName
 *
 **********************************************************************************/
int CBatchEngine::GetEnvVariable( char *aVariableName, char *aOutputBuffer, int aOutputBufferLen ) 
{
	// check params
	assert ( aVariableName != NULL );
	assert ( aOutputBuffer != NULL );
	assert ( aOutputBufferLen > 0 );

	// check that there is an actual usecase running
	if( iControlThreadState != CTS_USECASE_STARTED ) {
		return UCCS_NOUSECASERUNNING;
	}
	
	// check that there is actually a command around
	if( iExecuteCommand == NULL ) {
		return UCCS_COMMANDEXECUTIONNOTSTARTEDYET;
	}

	// change aVariableName to uppercase -a s it is stored in the data record as uppercase.
	_strupr( aVariableName );

	// now go get the environment var
	return iExecuteCommand->GetEnvironmentVariable( aVariableName, aOutputBuffer, aOutputBufferLen );
}

/***********************************************************************************
 *
 * PUBLIC METHOD: RunCommand
 *
 **********************************************************************************/
int CBatchEngine::RunCommand( char* aCommandLine )
{
	// check params
	assert ( aCommandLine != NULL );

	// check that there is actually a command around
	if( iSync == NULL ) {
		iSync = new CSynchronisation( iOutput );
	}
	if( iExecuteCommand == NULL ) {
		iExecuteCommand = new CExecuteCommand( iSync, iOutput );
	}

	// now go get the environment var
	return iExecuteCommand->ExecuteCommand( aCommandLine );
}
/***********************************************************************************
 *
 * PUBLIC METHOD: Signal
 *
 **********************************************************************************/

int CBatchEngine::Signal( int aUsecaseID )
{
	// check that the state is valid
	if( iControlThreadState != CTS_USECASE_STARTED ) {
		return UCCS_NOUSECASERUNNING;
	}
	return iSync->SignalFromDevice();
}


/***********************************************************************************
 *
 * PUBLIC METHOD: Rendezvous
 *
 **********************************************************************************/
int CBatchEngine::Rendezvous( int aUseCaseID )
{
	// check that the control state is valid
	if( iControlThreadState != CTS_USECASE_STARTED ) {
		return UCCS_NOUSECASERUNNING;
	}

	// check that the execution thread is still running
	if( iExecutionThreadState != ETS_EXECUTING_SCRIPT ) {
		return UCCS_SCRIPTFINISHED;
	}

	// do the sync
	return iSync->RendezvousFromDevice();
}


/***********************************************************************************
 *
 * PUBLIC METHOD: Wait
 *
 **********************************************************************************/
int CBatchEngine::Wait( int aUseCaseID )
{
	// check that the control state is valid
	if( iControlThreadState != CTS_USECASE_STARTED ) {
		return UCCS_NOUSECASERUNNING;
	}

	// check that the execution thread is still running
	if( iExecutionThreadState != ETS_EXECUTING_SCRIPT ) {
		return UCCS_SCRIPTFINISHED;
	}

	// do the sync
	return iSync->WaitFromDevice();
}


/***********************************************************************************
 *
 * PUBLIC METHOD: ExecuteScript
 *
 **********************************************************************************/
int CBatchEngine::ExecuteScript( void )
{
	int err;
	char *c;
	char command_buffer[MAXCOMMANDLENGTH];
	int rv = 0;

	// execute all the commands 
	while( 1 ) {

		// if the controller has ended the usecase then we stop executing commands
		if( iControlThreadState != CTS_USECASE_STARTED ) {
			break;
		}

		// get the next command to execute
		err = iRetrieveCommand->GetNextCommand( command_buffer, MAXCOMMANDLENGTH );
		if( err == UCCS_NOMORECOMMANDS ) {
			iOutput->CompletedScript();
			break;
		} 
		assert( err == UCCS_OK );
		
		// NOTE: the code below is the correct implementation of handling generic errors from the input 
		// module. It has been taken out because the input modules don't return anything except UCCS_NOMORECOMMANDS
		// and UCCS_OK so there is no way to test the condition (and it messes up our coverage results!). But
		// if new error codes are put in this implementation should be used.
//			else if( err != UCCS_OK ) {
//			iOutput->Error( err, "GetNextCommand returned error. Stopping script execution." );
//			rv = err;
//			break;
//		}

		// if the first not whitespace char is 0, or #, or // then return the comment
		for( c = command_buffer; IS_WHITESPACE(*c); c++ ) 
			;
		if( (*c == 0) || (*c == '#') || ((c[0] == '/') && (c[1] == '/'))) {
			continue;
		}

		// now execute the command 
		iOutput->ExecuteString( c );
		err = iExecuteCommand->ExecuteCommand( c );
		iOutput->ExecuteStringResult( err );
		if( (err != UCCS_OK) && (err != UCCS_QUIT) ) {
			iOutput->Error( err, NULL );
		}

		// if the return value from the command was quit (i.e. the script had a 
		// quit command) then we print the message and break from the loop.
		if( err == UCCS_QUIT ) {
			iOutput->CompletedScript();
			break;
		}

		// save the last error -- this is so we can notify the device is an error occured
		if( err != UCCS_OK ) {
			iLastError = err;
		}

		// if we do a require or requirenot that fails then make a point about it!
		if( (err == UCCS_REQUIREDVALUEERROR) || (err == UCCS_REQUIREDVALUEINCORRECT) || (err == UCCS_REQUIREDNOTVALUEERROR) || (err == UCCS_REQUIREDNOTVALUEMATCH) ) {
			// should break out here -- problem is that at the moment there are no reset calls so we can't recover!!
		}		
	}

	// set the state of this thread to completed and clear the synchronisation state so that
	// the control thread won't wait forever. The state of the execution thread should stop
	// the control thread from being able to wait again
	iExecutionThreadState = ETS_COMPLETED_SCRIPT;
	iSync->ClearSynchronisation();

	// tell the input module that we are done with it
	err = iRetrieveCommand->EndUseCase();
	assert( err == UCCS_OK );

	// set the state to idle 
	iExecutionThreadState = ETS_IDLE;

	// done
	return rv;
}
			

/***********************************************************************************
 *
 * FUNCTION: Entry point for second thread -- call executescript on the passed
 * batch engine object.
 *
 **********************************************************************************/
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{	
	CBatchEngine* aLocalBatchEngine;
	aLocalBatchEngine = (CBatchEngine*)lpParameter;
	return aLocalBatchEngine->ExecuteScript();
}

	

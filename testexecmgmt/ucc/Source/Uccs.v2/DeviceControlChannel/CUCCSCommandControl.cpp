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
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

/**********************************************************************************************
 *
 * Local Includes
 *
 *********************************************************************************************/
#include "CUCCSCommandControl.h"
#include "../Core/UCCS_ErrorCodes.h"

/**********************************************************************************************
 *
 * Defines
 *
 *********************************************************************************************/

/**********************************************************************************************
 *
 * Constructor
 *
 *********************************************************************************************/
CUCCSCommandControl::CUCCSCommandControl( MUccsControl *aCallBackService, IOutput *aOutput )
{
	assert( aCallBackService != NULL );
	assert( aOutput != NULL );

	iOutput = aOutput;
	iCallBackService = aCallBackService;
	iStopFlag = 0;
}

/**********************************************************************************************
 *
 * Destructor
 *
 *********************************************************************************************/
CUCCSCommandControl::~CUCCSCommandControl()
{
}

/**********************************************************************************************
 *
 * Start() -- the main routine called to run the serial listener.
 *
 *********************************************************************************************/
TCommandControlError CUCCSCommandControl::Start( TRemoteControlTransport aTransport, char *aPortname, int *aErrorCode, int *aScriptError )
{
	int dataLength, script_result;
	TCommandControlError return_value = EAC_SUCCESS;
	int engine_result = 0;
	char commandBuffer[KMAXDATALENGTH];
	char *cmdbptr;
	TPCommand commandID;
	TCPError protErr;

	// Pointers to all structures to receive data for an ID.
	TStartUsecaseRequest *startusecase_data;
	TSignalRequest *signal_data;
	TRendezvousRequest *rendezvous_data;
	TWaitRequest *wait_data;
	TEndUsecaseRequest *endusecase_data;
	TGetVariableNameRequest *variablename_data;
	TRunCommandRequest *runcommand_data;

	// Pointers to all structures used in a reply to a request
	TStartUsecaseReply startusecase_rep;
	TSignalReply signal_rep;
	TRendezvousReply rendezvous_rep;
	TWaitReply wait_rep;
	TEndUsecaseReply endusecase_rep;
	TGetVariableNameReply variablename_rep;
	TRunCommandReply runcommand_rep;
	cmdbptr = &(commandBuffer[0]);

	//Check params
	assert ( aPortname != NULL );
	assert ( aErrorCode!= NULL );
	assert ( aScriptError != NULL );
	*aScriptError = 0;
	iTransport = aTransport;

	// Initialise the transport
	protErr = iProtocol.initialise( aTransport, aPortname, iOutput );
	if (protErr != TCP_SUCCESS)
	{
		*aErrorCode = protErr;
		return EAC_ERRINITTRANSPORT;
	}

	// receive bytes -- forever really
	while( 1 ) {

		// check the end flag
		if( iStopFlag != 0 ) {
			break;
		}

		// receive the command id and data via iProtocol
		dataLength = KMAXDATALENGTH;
		protErr = iProtocol.receiveMessage (&commandID, &dataLength, cmdbptr);

		// if there is an error in the protocol we cannot recover. 
		if (protErr != TCP_SUCCESS)
		{
			return_value =  EAC_RECEIVEBYTESERROR;
			*aErrorCode = protErr;
			break;
		}

		// msg for serial clients
		LocalUpdateMessageIn(commandID, cmdbptr);

		// now handle the command
		switch( commandID ) {

			case CMD_REQ_STARTUSECASEID:
				startusecase_data = (TStartUsecaseRequest*)cmdbptr;
				engine_result = iCallBackService->StartUsecase( startusecase_data->iUsecaseID );
				startusecase_rep.iResult = engine_result;
				protErr = iProtocol.sendReply(CMD_REP_STARTUSECASEID, sizeof(startusecase_rep), &startusecase_rep);
				break;

			case CMD_REQ_SIGNALID:
				signal_data = (TSignalRequest*)cmdbptr;
				engine_result = iCallBackService->Signal( signal_data->iUsecaseID );
				signal_rep.iResult = engine_result;
				protErr = iProtocol.sendReply(CMD_REP_SIGNALID, sizeof(signal_rep), &signal_rep);
				break;
			
			case CMD_REQ_RENDEZVOUSID:
				rendezvous_data = (TRendezvousRequest*)cmdbptr;
				engine_result = iCallBackService->Rendezvous( rendezvous_data->iUsecaseID );
				rendezvous_rep.iResult = engine_result;
				protErr = iProtocol.sendReply(CMD_REP_RENDEZVOUSID, sizeof(rendezvous_rep), &rendezvous_rep);
				break;
				
			case CMD_REQ_WAITID:
				wait_data = (TWaitRequest*)cmdbptr;
				engine_result = iCallBackService->Wait( wait_data->iUsecaseID );
				wait_rep.iResult = engine_result;
				protErr = iProtocol.sendReply(CMD_REP_WAITID, sizeof(wait_rep), &wait_rep);
				break;	

			case CMD_REQ_ENDUSECASEID:
				endusecase_data = (TEndUsecaseRequest*)cmdbptr;
				engine_result = iCallBackService->EndUsecase( endusecase_data->iUsecaseID, endusecase_data->iResult, &script_result);
				endusecase_rep.iCommandResult = engine_result;
				endusecase_rep.iScriptResult = script_result;
				protErr = iProtocol.sendReply(CMD_REP_ENDUSECASEID, sizeof(endusecase_rep), &endusecase_rep);
				iStopFlag = 1;
				break;

			case CMD_REQ_GETVARIABLENAMEID:
				variablename_data = (TGetVariableNameRequest*)cmdbptr;
				engine_result = iCallBackService->GetEnvVariable(variablename_data->iVariableName, variablename_rep.iVariableValue, MAXVARNAMELEN );
				variablename_rep.iResult = engine_result;
				protErr = iProtocol.sendReply(CMD_REP_GETVARIABLENAMEID, sizeof(variablename_rep), &variablename_rep);
				break;

			case CMD_QUITID:
				protErr = TCP_FAILED;
				return_value = EAC_QUIT;
				break;	

			case CMD_REQ_RUNCOMMAND:
				runcommand_data = (TRunCommandRequest*)cmdbptr;
				engine_result = iCallBackService->RunCommand( runcommand_data->iCommandLine );
				runcommand_rep.iResult = engine_result;
				protErr = iProtocol.sendReply(CMD_REP_RUNCOMMAND, sizeof(runcommand_rep), &runcommand_rep);
				break;

			default:
				iOutput->Error( UCCS_UNKNOWNCONTROLCOMMAND, NULL );
				protErr = TCP_SUCCESS;
				return_value = EAC_SUCCESS;
		}

		// print the message
		LocalUpdateMessageOut( engine_result );

		// if there was an error in the transport then exit
		if( protErr != TCP_SUCCESS  ) {
			if( return_value != EAC_QUIT ) {
				return_value = EAC_PROCESSINGFAILED;
			}
			break;
		}
	}

	// done
	iProtocol.disconnect();
	iStopFlag = 0;
	*aErrorCode = protErr;
	return return_value;
	
}		


/**********************************************************************************************
 *
 * Helper: Print messages when we are in serial mode
 *
 *********************************************************************************************/
void CUCCSCommandControl::LocalUpdateMessageIn(int aCommandId, char *aMsg)
{
	static char *cmdIdStrings[] = 
	{
		"StartUseCase (request)",		// CMD_REQ_STARTUSECASEID,
		"Signal  (request)",			// CMD_REQ_SIGNALID,
		"Rendezvous  (request)",		// CMD_REQ_RENDEZVOUSID,
		"WaitForSignal (request)",		// CMD_REQ_WAITID,
		"EndUseCase (request)",			// CMD_REQ_ENDUSECASEID,
		"GetVariableName (request)",	// CMD_REQ_GETVARIABLENAMEID,
		"Run (request)",				// CMD_REQ_RUNCOMMAND
		"StartUseCase (response)",		// CMD_REP_STARTUSECASEID,
		"Signal (response)"				// CMD_REP_SIGNALID,
		"Rendezvous (response)",		// CMD_REP_RENDEZVOUSID,
		"WaitForSignal  (response)",	// CMD_REP_WAITID,
		"EndUseCase (response)",		// CMD_REP_ENDUSECASEID,
		"GetVariableName (response)",	// CMD_REP_GETVARIABLENAMEID,
		"Run (response)",				// CMD_REP_RUNCOMMAND,
		"Quit",							// CMD_QUITID,
		"Unknown",						// CMD_UNKNOWN
	};

	// don't display for console;  - consolidate o/p
	if (iTransport != RCI_CONSOLE)
	{
		// display msg - belongs within protocol implementation?
		fprintf(stdout, "\n%s\n", aMsg);

		if (aCommandId > CMD_UNKNOWN)
			fprintf(stdout, "Error! unsupported command identifier %d\n", aCommandId);
		else
			fprintf(stdout, " command; \"%s\" (%d)\n", cmdIdStrings[aCommandId], aCommandId);
	}
}

void CUCCSCommandControl::LocalUpdateMessageOut(int aResult)
{
	// don't display for console;  - consolidate o/p
	if( iTransport != RCI_CONSOLE)
		fprintf(stdout, " result; \"%s\" (%d)\n", GetUccsErrorStringI(aResult), aResult);
}

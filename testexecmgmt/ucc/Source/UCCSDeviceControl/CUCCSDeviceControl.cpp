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



#include <e32uid.h>
#include <string.h>
#include <stdlib.h>


/**********************************************************************************
 *
 * Local Includes
 *
 *********************************************************************************/
#include "assert.h"
#include "CUCCSDeviceControl.h"

// Error Codes
#include "CProtocolTypes.h"
#include "UCCS_ErrorCodes.h"


/**********************************************************************************
 *
 * Dll Entry point
 *
 *********************************************************************************/
/**********************************************************************************
 *
 * Construction / Destruction
 *
 *********************************************************************************/
EXPORT_C CUCCSDeviceControl* CUCCSDeviceControl::New()
{
	CUCCSDeviceControl* self = new CUCCSDeviceControl();
    CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
    return self;
}


CUCCSDeviceControl::CUCCSDeviceControl()
{
}


void CUCCSDeviceControl::ConstructL()
{
	iProtocol = new (ELeave) CUCCSDeviceProtocol();
	assert( iProtocol != NULL );
}


EXPORT_C CUCCSDeviceControl::~CUCCSDeviceControl()
{
	int ret;
	ret = iProtocol->disconnect();
	assert (ret == TDC_SUCCESS);
	assert( iProtocol != NULL );
	delete iProtocol;
	iProtocol = NULL;
}


EXPORT_C TUint CUCCSDeviceControl::connectL( TDesC *aRemoteHost, TDPTransport aTransportType )
{
	int ret = iProtocol->initialise(*aRemoteHost,aTransportType);
	if (ret != TDP_SUCCESS)
		return TDC_ERRINTIALISING;
	return TDC_SUCCESS;
}


/**********************************************************************************
 *
 * startUseCase
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::startUseCase ( TUint aData, int *aErrorCode )
{		
	int ret, datalen;
	TStartUsecaseRequest req;
	TStartUsecaseReply rep;
	TPCommand cmdID;

	// Send the request
	cmdID = CMD_REQ_STARTUSECASEID;
	req.iUsecaseID = aData;
	datalen = sizeof(req);
	
	ret = iProtocol->sendMessage( cmdID, datalen, &req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	datalen = sizeof(rep);
	ret = iProtocol->receiveMessage ( &cmdID, &datalen, &rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check that the reply cmdID is what we expect it to be
	if ( cmdID != CMD_REP_STARTUSECASEID )
		return TDC_INVALIDREPLY;

	// Then return the result
	return rep.iResult;
}

/**********************************************************************************
 *
 * signal
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::signal( TUint aData, int *aErrorCode )
{
	int ret, datalen;
	TSignalRequest req;
	TSignalReply rep;
	TPCommand cmdID;

	// Send the request
	cmdID = CMD_REQ_SIGNALID;
	req.iUsecaseID = aData;
	datalen = sizeof(req);
	
	ret = iProtocol->sendMessage( cmdID, datalen, &req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	datalen = sizeof(rep);
	ret = iProtocol->receiveMessage ( &cmdID, &datalen, &rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check that the reply cmdID is what we expect it to be
	if ( cmdID != CMD_REP_SIGNALID )
		return TDC_INVALIDREPLY;

	// Then return the result
	return rep.iResult;
}

/**********************************************************************************
 *
 * waitfor
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::waitfor( TUint aData, int *aErrorCode )
{
	int ret, datalen;
	TWaitRequest req;
	TWaitReply rep;
	TPCommand cmdID;

	// Send the request
	cmdID = CMD_REQ_WAITID;
	req.iUsecaseID = aData;
	datalen = sizeof(req);
	
	ret = iProtocol->sendMessage( cmdID, datalen, &req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	datalen = sizeof(rep);
	ret = iProtocol->receiveMessage ( &cmdID, &datalen, &rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check that the reply cmdID is what we expect it to be
	if ( cmdID != CMD_REP_WAITID )
		return TDC_INVALIDREPLY;

	// Then return the result
	return rep.iResult;
}

/**********************************************************************************
 *
 * rendezvous
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::rendezvous( TUint aData, int *aErrorCode )
{
	int ret, datalen;
	TRendezvousRequest req;
	TRendezvousReply rep;
	TPCommand cmdID;

	// Send the request
	cmdID = CMD_REQ_RENDEZVOUSID;
	req.iUsecaseID = aData;
	datalen = sizeof(req);
	
	ret = iProtocol->sendMessage( cmdID, datalen, &req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	datalen = sizeof(rep);
	ret = iProtocol->receiveMessage ( &cmdID, &datalen, &rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check that the reply cmdID is what we expect it to be
	if ( cmdID != CMD_REP_RENDEZVOUSID )
		return TDC_INVALIDREPLY;

	// Then return the result
	return rep.iResult;
}

/**********************************************************************************
 *
 * endUseCase
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::endUseCase (TUint aUseCaseID, TUint aResult, int *aErrorCode )
{	
	int ret, datalen;
	TEndUsecaseRequest req;
	TEndUsecaseReply rep;
	TPCommand cmdID;

	// Send the request
	cmdID = CMD_REQ_ENDUSECASEID;
	req.iUsecaseID = aUseCaseID;
	req.iResult = aResult;
	datalen = sizeof(req);
	
	ret = iProtocol->sendMessage( cmdID, datalen, &req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	datalen = sizeof(rep);
	ret = iProtocol->receiveMessage ( &cmdID, &datalen, &rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check that the reply cmdID is what we expect it to be
	if ( cmdID != CMD_REP_ENDUSECASEID )
		return TDC_INVALIDREPLY;

	// Then return the result
	return rep.iCommandResult;
}

/**********************************************************************************
 *
 * getVariableName
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::getVariableName( char* aVariableName, char* aOutputBuffer, int aOutputBufferSize, int *aErrorCode )
{
	int ret, len, recDatalen;
	TGetVariableNameRequest var_req;
	TGetVariableNameReply var_rep;
	TPCommand cmd;

	// Check params
	assert ( aVariableName != NULL );
	assert ( aOutputBuffer != NULL );
	assert ( aOutputBufferSize >= 0);

	// Check if aVariableName can be copied into our structure
	len = strlen(aVariableName) + 1;
	if ( len > MAXVARNAMELEN )
		return TDC_VARNAMETOOLONG;

	// Then copy into our buffer
	memcpy ( var_req.iVariableName, aVariableName, len);

	// Send the request
	cmd = CMD_REQ_GETVARIABLENAMEID;
	ret = iProtocol->sendMessage( cmd, sizeof(var_req), &var_req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	recDatalen = sizeof(var_rep);
	ret = iProtocol->receiveMessage( &cmd, &recDatalen, &var_rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check whether we successfully retrieved the value we want
	if ( cmd != CMD_REP_GETVARIABLENAMEID)
		return TDC_INVALIDREPLY;			// Not what we are expecting 

	// Only copy the data if everything went ok at the UCCS end - UCCS_OK 
	if ( var_rep.iResult == UCCS_OK )
	{
		// Return the data
		// Check that the data received will fit into the return buffer
		len = strlen (var_rep.iVariableValue) + 1;
		if ( len > aOutputBufferSize )
			return TDC_VARVALTOOLONG;

		// Else copy the data into the return buffer
		memcpy ( aOutputBuffer, var_rep.iVariableValue, len);
	}

	return var_rep.iResult;
}

/**********************************************************************************
 *
 * runCommand
 *
 *********************************************************************************/
EXPORT_C TUint CUCCSDeviceControl::runCommandL( char* aCommandLine, int *aErrorCode )
{
	int ret, len, recDatalen;
	TRunCommandRequest* run_req = new (ELeave) (TRunCommandRequest);
	TRunCommandReply run_rep;
	TPCommand cmd;

	// Check params
	assert ( aCommandLine != NULL );

	// Check if aCommandLine can be copied into our structure
	len = strlen( aCommandLine )+1;
	if ( len > MAXCOMMANDLINELEN )
		return TDC_COMMANDLINETOOLONG;

	// Then copy into our buffer
	memset( run_req->iCommandLine, 0x00, sizeof(run_req->iCommandLine) );
	memcpy( run_req->iCommandLine, aCommandLine, len);

	// Send the request
	cmd = CMD_REQ_RUNCOMMAND;
	ret = iProtocol->sendMessage( cmd, sizeof(*run_req), run_req);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRSENDING;

	// Get the reply
	recDatalen = sizeof(run_rep);
	ret = iProtocol->receiveMessage( &cmd, &recDatalen, &run_rep);
	*aErrorCode = ret;
	if ( ret != TDP_SUCCESS )
		return TDC_ERRRECV;

	// Check whether we successfully retrieved the value we want
	if ( cmd != CMD_REP_RUNCOMMAND)
		return TDC_INVALIDREPLY;			// Not what we are expecting 

	return run_rep.iResult;
}


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
* Includes
*
*/



#include <e32math.h>
#include <e32std.h>

#include "CProtocolTypes.h"
#include "CSerialTransport.h"
#include "CTCPTransport.h"
#include "CUCCSDeviceProtocol.h"
#include "assert.h"


/*****************************************************************************
 *
 *	Constructor
 *
 ****************************************************************************/
CUCCSDeviceProtocol::CUCCSDeviceProtocol()
{		
	iTransport = NULL;
	iStatus = TDP_IDLE;

	// Seed the random number generator - this requires a 64 bit seed
	TTime now;
	now.HomeTime();
	iRandomSeed = now.Int64();
}


/*****************************************************************************
 *
 *	Destructor 
 *
 ****************************************************************************/
CUCCSDeviceProtocol::~CUCCSDeviceProtocol()
{
	assert ( iStatus == TDP_IDLE );
	assert ( iTransport == NULL );
}


/*****************************************************************************
 *
 *	Initialise 
 *
 ****************************************************************************/
TDPError CUCCSDeviceProtocol::initialise(TBufC16<40> aRemoteHost, TDPTransport aTransportType)
	{	
	// Check the state
	assert ( iStatus == TDP_IDLE );

	// Check params
	assert (aRemoteHost.Length() > 0);
	
	// Set the member data
	iRand_UID = Math::Rand( iRandomSeed );
	iRemoteHost = aRemoteHost;

	// Initialise the serial transport
	if( iTransport == NULL )
		{
		TInt r = 0;
		if( aTransportType == TDP_SERIAL )
			{
			TRAP ( r, iTransport = CSerialTransport::NewL(_L("ECUART")) );
			}
		else if( aTransportType == TDP_TCP )
			{
			TRAP ( r, iTransport = new (ELeave) CTCPTransport() );
			
			}
		else
			{
			return TDP_ERRINTIALISING;
			}

		if ( r != KErrNone)
			return TDP_ERRINTIALISING;
		
		TRAPD ( s,iTransport->InitialiseL() );
		if ( s != KErrNone)
			{	
			iTransport->Release();
			delete iTransport;
			iTransport = NULL;
			return TDP_ERRINTIALISING;
			}

		TRAPD ( t, iTransport->ConnectL(&iRemoteHost) );
		if ( t != KErrNone)
			{
			iTransport->Disconnect();
			iTransport->Release();
			delete iTransport;
			iTransport = NULL;
			return TDP_ERRINTIALISING;
			}
		}

	iStatus = TDP_CONNECTED;
	return TDP_SUCCESS;
	}


/*****************************************************************************
 *
 *	Disconnect 
 *
 ****************************************************************************/
TDPError CUCCSDeviceProtocol::disconnect()
{
	// check the state
	if( iStatus == TDP_IDLE ) {
		return TDP_SUCCESS;
	}
	assert ( iStatus == TDP_CONNECTED );

	// if the serial transport is up then bring it down
	if( iTransport != NULL ) {
		iTransport->Disconnect();
		iTransport->Release();	
		delete iTransport;
		iTransport = NULL;
	}

	// update the state and return
	iStatus = TDP_IDLE;
	return TDP_SUCCESS;
}


/*****************************************************************************
 *
 *	Receive Message 
 *
 ****************************************************************************/
TDPError CUCCSDeviceProtocol::receiveMessage(TPCommand* aCmd, int* aDataLength, void* aData)
{
	TPHeader header;
	TInt ret;
	int bufferLen;

	// Check the state
	assert ( iStatus == TDP_CONNECTED );

	// Check params
	assert( aCmd != NULL );
	assert( aDataLength != NULL );
	assert( aData != NULL );

	// Record the original length of the aData buffer b4 it gets overwritten
	bufferLen = *aDataLength;

	// Read the header
	TPtr8 p( (TUint8*)&header,sizeof(header), sizeof(header) );
	ret = iTransport->RequestReceive( &p, p.Size());	
	if ( ret != KErrNone)
		return TDP_RECVERROR;

	// Check that this is a response to our last message
	if ( header.iUid != iRand_UID)
		return TDP_UIDMISMATCH;

	// Extract the header information
	*aCmd = header.iCmdID;
	*aDataLength = header.iDataLen;

	// Now read the rest of the data
	TPtr8 q( (TUint8*)aData, bufferLen, bufferLen);
	ret = iTransport->RequestReceive( &q, q.Size());	
	if ( ret != KErrNone)
		return TDP_RECVERROR;

	// Increment the UID for next time
	iRand_UID += 1;
	return TDP_SUCCESS;
}


/*****************************************************************************
 *
 *	Send Message 
 *
 ****************************************************************************/
TDPError CUCCSDeviceProtocol::sendMessage(TPCommand aCmd, int aDataLength, void *aData)
{
	TPHeader header;
	TInt ret;

	// Check the state
	assert ( iStatus == TDP_CONNECTED );

	// Check params
	assert( aData != NULL );

	if( isValidCMDID(aCmd) == 0 ) 
	{
		return TDP_INVALIDCMDID;
	}

	// Create the header
	header.iCmdID = aCmd;
	header.iDataLen = aDataLength;
	header.iUid = (int)iRand_UID;
	
	// Send the header first
	TPtrC8 p( (unsigned char*)&header , sizeof(header) );
	ret = iTransport->RequestSend( &p, p.Size());

	if (ret != KErrNone)
		return TDP_SENDERROR;

	// Now send the rest of the data
	TPtrC8 q( (unsigned char*)aData , aDataLength );
	ret = iTransport->RequestSend( &q , q.Size());
	if (ret != KErrNone)
		return TDP_SENDERROR;

	return TDP_SUCCESS;
}


/**********************************************************************************************
 *
 * Check IDs
 *
 *********************************************************************************************/	
bool CUCCSDeviceProtocol::isValidCMDID(TPCommand aCommand)
{	
	bool ret_val = true;

	switch (aCommand) 
	{
		case CMD_REQ_SIGNALID:
			break;

		case CMD_REQ_RENDEZVOUSID:
			break;

		case CMD_REQ_WAITID:
			break;
   
		case CMD_REQ_STARTUSECASEID:
			break;

		case CMD_REQ_ENDUSECASEID:
			break;
	
		case CMD_REQ_GETVARIABLENAMEID:
			break;

		case CMD_QUITID:
			break;

		case CMD_REQ_RUNCOMMAND:
			break;

		default:
			ret_val = false;
			break;
   }

	return ret_val;
}

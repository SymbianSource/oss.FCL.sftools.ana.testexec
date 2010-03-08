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



#include <time.h>
#include <assert.h>

/**********************************************************************************************
 *
 * Local Includes
 *
 *********************************************************************************************/
#include "CUCCSCommandProtocol.h"
#include "CFrameBuffer.h"
#include "strncpynt.h"

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
CUCCSCommandProtocol::CUCCSCommandProtocol()
{
	iPort = NULL;
	iOutput = NULL;
 
	// Seed the random number generator
	srand( (unsigned)time(NULL) );
}

/**********************************************************************************************
 *
 * Destructor
 *
 *********************************************************************************************/
CUCCSCommandProtocol::~CUCCSCommandProtocol()
{
}
	
/**********************************************************************************************
 *
 * Initialise
 *
 *********************************************************************************************/
TCPError CUCCSCommandProtocol::initialise(TRemoteControlTransport aTransport, char* aRemoteHost, IOutput* aOutput)
{
	int ret;

	// Check params are not equal to NULL
	assert (aRemoteHost != NULL);
	assert (strlen(aRemoteHost) < (KMAXPORTNAME -1));
	assert (aOutput != NULL);

	// Assign the params to the relevant member data.
	if ( aTransport == RCI_SERIAL )
	{	
		iPort = &iSerialPort;
	}
	else if ( aTransport == RCI_CONSOLE)
	{
		iPort = &iConsolePort;
	}
	else if ( aTransport == RCI_TCP)
	{
		iPort = &iTcpPort;
	}
	else
	{
		return TCP_INVALIDTRANSPORT;
	}

	// Set the state members 
	STRNCPY_NULL_TERMINATE( iRemoteHost, aRemoteHost, KMAXPORTNAME );
	iOutput = aOutput;
	iRand_UID = (unsigned)rand();

	// Open the serial port
	ret = iPort->OpenPort(iRemoteHost);
	if( ret != 0 ) {
		return TCP_FAILEDTOOPENPORT;
	}
	return TCP_SUCCESS;
}

/**********************************************************************************************
 *
 * Disconnect
 *
 *********************************************************************************************/
TCPError CUCCSCommandProtocol::disconnect()				
{
	// Close the serial port.
	iPort->ClosePort();
	iPort = NULL;

	return TCP_SUCCESS;
}

/**********************************************************************************************
 *
 * SendReply
 *
 *********************************************************************************************/	
TCPError CUCCSCommandProtocol::sendReply( TPCommand aCommand, int aDataSize, void* aData )
{
	int iFrameSize, ret;
	TFBError err;

	if (!isValidCMDID(aCommand))
		return TCP_INVALIDCMDID;
	
	iFrameSize = KMAXFRAMESIZE;
	err = CFrameBuffer::createSendBuffer( iRand_UID, aCommand, aDataSize, (char*)aData, iFrame, &iFrameSize );
	if ( err != TFB_SUCCESS)
		return TCP_ERRCREATINGBUFFER;

	ret = iPort->SendBytes(iFrame, &iFrameSize);
	if ( ret != 0 )
		return TCP_ERRSENDINGBYTES;

	return TCP_SUCCESS;
}

/**********************************************************************************************
 *
 * ReceiveMessage
 *
 *********************************************************************************************/	
TCPError CUCCSCommandProtocol::receiveMessage( TPCommand* aCommand, int* aDataLength, void* aData )
{
	int res, len;
	TPHeader header;

	len = sizeof(header);

	// Read the first 12 bytes of header -  the uid, command and the data size.	
	res = iPort->ReceiveBytes((char*)&header, &len );
	if ( res != 0 )
		return TCP_ERRRECVINGBYTES;
	
	// Decode the response
	iRand_UID = header.iUid;        
	*aCommand = header.iCmdID;       
	*aDataLength = header.iDataLen; 
	
	// Get the rest
	res = iPort->ReceiveBytes((char*)aData, aDataLength);
	if ( res != 0 )
		return TCP_ERRRECVINGBYTES;
	
	return TCP_SUCCESS;
}

/**********************************************************************************************
 *
 * Check IDs
 *
 *********************************************************************************************/	
bool CUCCSCommandProtocol::isValidCMDID(TPCommand aCommand)
{	
	bool ret_val = true;

	switch (aCommand) 
	{
		case CMD_REP_SIGNALID:
			break;

		case CMD_REP_RENDEZVOUSID:
			break;

		case CMD_REP_WAITID:
			break;
   
		case CMD_REP_STARTUSECASEID:
			break;

		case CMD_REP_ENDUSECASEID:
			break;

		case CMD_REP_GETVARIABLENAMEID:
			break;

		case CMD_QUITID:
			break;

		case CMD_REP_RUNCOMMAND:
			break;

		default:
			ret_val = false;
			break;
   }

	return ret_val;
}


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
* CUCCSCommandProtocol Class
*
*/



#ifndef _CUCCSCommandProtocol_H
#define _CUCCSCommandProtocol_H

/*****************************************************************************
 *
 * System Includes 
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/
#include "../Core/UCCS_Interfaces.h"
#include "CSerialPort.h"
#include "CConsolePort.h"
#include "CTcpPort.h"
#include "CProtocolTypes.h"

/*****************************************************************************
 *
 *  Types
 *
 ****************************************************************************/
typedef enum {
	TCP_SUCCESS,
	TCP_ERRCREATINGBUFFER,
	TCP_ERRRECVINGBYTES,
	TCP_ERRSENDINGBYTES,
	TCP_INVALIDCMDID,
	TCP_INVALIDTRANSPORT,
	TCP_FAILEDTOOPENPORT,
	TCP_FAILED
} TCPError;

#define KMAXPORTNAME	64
#define KMAXFRAMESIZE 1024

/*****************************************************************************
 *
 * Class Definition
 *
 ****************************************************************************/
class CUCCSCommandProtocol
{
public:
	CUCCSCommandProtocol();
	~CUCCSCommandProtocol();
	
	TCPError initialise(TRemoteControlTransport aTransport, char* aRemoteHost, IOutput* aOutput);
	TCPError disconnect();				
	
	TCPError sendReply( TPCommand aCommand, int aDataSize, void* aData );
	TCPError receiveMessage( TPCommand* aCommand, int* aDataLength, void* aData );

private:
	bool isValidCMDID(TPCommand aCommand);

	CSerialPort iSerialPort;
	CConsolePort iConsolePort;
	CTcpPort iTcpPort;
	IPort* iPort;
	IOutput* iOutput;
	int iRand_UID;						// The last recieved UID
	char iRemoteHost[KMAXPORTNAME];
	char iFrame[KMAXFRAMESIZE];
};

#endif

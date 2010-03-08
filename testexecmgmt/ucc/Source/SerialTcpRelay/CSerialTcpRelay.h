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
* CSerialTcpRelay
*
*/



#ifndef __CSERIALTCPRELAY_H__
#define __CSERIALTCPRELAY_H__


/*************************************************************************************
 *
 * Local Includes
 *
 ************************************************************************************/
#include "CSerialPort.h"
#include "../ThreadLibrary/CAThread.h"


/*************************************************************************************
 *
 * Definitions
 *
 ************************************************************************************/
#define SERIALREADBUFFSIZE	(1024)
#define SOCKETREADBUFFSIZE	(1024)


/*************************************************************************************
 *
 * Types
 *
 ************************************************************************************/
typedef enum {
	RE_SUCCESS,	
	RE_SOCKET_FAILED,
	RE_CONNECT_FAILED,
	RE_INITIAL_SEND_FAILED
} TRelayError;


typedef enum {
	RS_NEW,
	RS_ACTIVE,
	RS_CLOSED,
} TRelayState;

/*************************************************************************************
 *
 * CSerialTcpRelay Definition
 *
 ************************************************************************************/
class CSerialTcpRelay
{
public:
	CSerialTcpRelay();
	~CSerialTcpRelay();
	TRelayError InitialiseRelay( CSerialPort *aSerialPort, SOCKADDR_IN aRemoteAddress, char *aInitialBuffer, int aBufferLength, int *aErrCode );
	TRelayError	ExecuteRelay();

private:
	TRelayError ExecuteSerialThread();
	TRelayError ExecuteSocketThread();

	static int SerialThreadProc( CSerialTcpRelay *aRelay );
	static int SocketThreadProc( CSerialTcpRelay *aRelay );

	CSerialPort *iSerialPort;
	SOCKET iSocket;
	SOCKADDR_IN iRemoteAddress;
	char iSerialReadBuffer[SERIALREADBUFFSIZE];
	char iSocketReadBuffer[SOCKETREADBUFFSIZE];
	TRelayState iState;
	TRelayState iSerialState;
	TRelayState iSocketState;
	CAThread iSerialThread;
	CAThread iSocketThread;
};

#endif

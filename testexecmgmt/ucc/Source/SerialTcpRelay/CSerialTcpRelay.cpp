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
#include <assert.h>


/*************************************************************************************
 *
 * LocalIncludes
 *
 ************************************************************************************/
#include "CSerialTcpRelay.h"


/*************************************************************************************
 *
 * Definitions
 *
 ************************************************************************************/
#define SOCKET_TIMEOUT (1000)


/*************************************************************************************
 *
 * PUBLIC METHOD: CSerialTcpRelay
 *
 ************************************************************************************/
CSerialTcpRelay::CSerialTcpRelay() : iSerialThread("SerialThread"), iSocketThread("SocketThread")
{
	iSerialPort = NULL;
	iSocket = INVALID_SOCKET;
	iState = iSerialState = iSocketState = RS_NEW;
}


/*************************************************************************************
 *
 * PUBLIC METHOD: ~CSerialTcpRelay
 *
 ************************************************************************************/
CSerialTcpRelay::~CSerialTcpRelay()
{
	assert( iSocket == INVALID_SOCKET );
	assert( iState != RS_ACTIVE );
	assert( iSerialState != RS_ACTIVE );
	assert( iSocketState != RS_ACTIVE );
}


/*************************************************************************************
 *
 * PUBLIC METHOD: InitialiseRelay
 *
 ************************************************************************************/
TRelayError CSerialTcpRelay::InitialiseRelay( CSerialPort *aSerialPort, SOCKADDR_IN aRemoteAddress, char *aInitialBuffer, int aBufferLength, int *aErrCode )
{
	int err;

	// check params
	assert( aSerialPort != NULL );
	assert( aErrCode != NULL );

	// save the state vars
	*aErrCode = 0;
	iSerialPort = aSerialPort;
	iRemoteAddress = aRemoteAddress;

	// open the socket
	iSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( iSocket == INVALID_SOCKET ){
		*aErrCode = WSAGetLastError();
		return RE_SOCKET_FAILED;
	}
	err = connect( iSocket, (struct sockaddr*)&iRemoteAddress, sizeof(iRemoteAddress) );
	if( err == SOCKET_ERROR ) {
		*aErrCode = WSAGetLastError();
		closesocket( iSocket );
		iSocket = INVALID_SOCKET;
		return RE_CONNECT_FAILED;
	}

	// if there is some initial data then send it
	if( (aInitialBuffer != NULL) && (aBufferLength > 0) ) {
		err = send( iSocket, aInitialBuffer, aBufferLength, 0 );
		if( err != aBufferLength ) {
			*aErrCode = WSAGetLastError();
			closesocket( iSocket );
			iSocket = INVALID_SOCKET;
			return RE_INITIAL_SEND_FAILED;
		}
	}

	// update the state
	iState = iSerialState = iSocketState = RS_ACTIVE;

	// ok we are ready to go
	return RE_SUCCESS;
}


/*************************************************************************************
 *
 * PUBLIC METHOD: ExecuteRelay
 *
 ************************************************************************************/
TRelayError CSerialTcpRelay::ExecuteRelay()
{
	TThreadError terr;
	int errcode;

	// create the serial thread
	terr = iSerialThread.StartThread( CSerialTcpRelay::SerialThreadProc, this, &errcode );
	assert( terr == TE_NONE );

	// create the socket thread
	terr = iSocketThread.StartThread( CSerialTcpRelay::SocketThreadProc, this, &errcode );
	assert( terr == TE_NONE );

	// wait for the serial thread to exit
	terr = iSerialThread.WaitForThread( INFINITE );
	assert( terr == TE_NONE );

	// wait for the socket thread to exit
	terr = iSocketThread.WaitForThread( INFINITE );
	assert( terr == TE_NONE );

	// done
	iState = RS_CLOSED;
	return RE_SUCCESS;
}


/*************************************************************************************
 *
 * PRIVATE METHOD: ExecuteSerialThread
 *
 ************************************************************************************/
TRelayError CSerialTcpRelay::ExecuteSerialThread()
{
	int err, bytes_read;
	DWORD dwModemStatus;

	// repeat...
	while( 1 ) {

		// read from the serial port
		bytes_read = SERIALREADBUFFSIZE;
		err = iSerialPort->ReceiveBytes( iSerialReadBuffer, &bytes_read );

		// if there was an error other than timeout then break
		if( err != 0 ) {
			break;
		}

		// write any received data to the socket - break on error
		if( bytes_read > 0 ) {
//			fprintf( stderr, "DEBUG: serial <-> socket (%d)\n", bytes_read );
			err = send( iSocket, iSerialReadBuffer, bytes_read, 0 );
			if( err != bytes_read ) {
				break;
			}
		}

		// check the serial line flags - if the peer has lowered cts or dsr then
		// we take this as meaning that they have closed the port
		err = GetCommModemStatus( iSerialPort->Handle(), &dwModemStatus );
		assert( err != 0 );
		if( ((dwModemStatus & MS_CTS_ON) == 0) ) {
			break;
		}

		// if the socket thread has exited then exit
		if( iSocketState == RS_CLOSED ) {
			break;
		}
	}
		
	// set my status to closed 
	iSerialState = RS_CLOSED;
	return RE_SUCCESS;
}


/*************************************************************************************
 *
 * PRIVATE METHOD: ExecuteSerialThread
 *
 ************************************************************************************/
TRelayError CSerialTcpRelay::ExecuteSocketThread()
{
	int err, bytes_read, bytes_written;
	struct timeval read_timeout;
	fd_set read_fds;

	// repeat...
	while( 1 ) {

		// setup things
		read_timeout.tv_sec = 0;
		read_timeout.tv_usec = (SOCKET_TIMEOUT*1000);
		FD_ZERO( &read_fds );
		FD_SET( iSocket, &read_fds );

		// see if data is available
		err = select( 1, &read_fds, NULL, NULL, &read_timeout );

		// break on errors
		if( err == SOCKET_ERROR ) {
			break;
		}

		// if there is data available then read it - break on error
		if( err != 0 ) {
			bytes_read = recv( iSocket, iSocketReadBuffer, SOCKETREADBUFFSIZE, 0 );
			if( bytes_read <= 0 ) {
				break;
			}
		}

		// write any received data to the serial port - break on error
		if( bytes_read > 0 ) {
			bytes_written = bytes_read;
//			fprintf( stderr, "DEBUG: socket <-> serial (%d)\n", bytes_read );
			err = iSerialPort->SendBytes( iSocketReadBuffer, &bytes_written );
			if( err != 0 ) {
				break;
			}
		}

		// if the serial thread has exited then exit
		if( iSerialState == RS_CLOSED ) {
			break;
		}

	}
	
	// close the socket
	closesocket( iSocket );
	iSocket = INVALID_SOCKET;

	// set my status to closed 
	iSocketState = RS_CLOSED;
	return RE_SUCCESS;
}


/*************************************************************************************
 *
 * PRIVATE METHOD: Thread Entry Procs
 *
 ************************************************************************************/
int CSerialTcpRelay::SerialThreadProc( CSerialTcpRelay *aRelay )
{
	return (int)aRelay->ExecuteSerialThread();
}


int CSerialTcpRelay::SocketThreadProc( CSerialTcpRelay *aRelay )
{
	return (int)aRelay->ExecuteSocketThread();
}


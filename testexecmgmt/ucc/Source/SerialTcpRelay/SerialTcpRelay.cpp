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
#include <assert.h>
#include <string.h>


/*************************************************************************************
 *
 * Local Includes
 *
 ************************************************************************************/
#include "CSerialTcpRelay.h"


/*************************************************************************************
 *
 * Definitions
 *
 ************************************************************************************/
#define TO_RETRY					(1000)
#define TO_TIMEWAIT					(3000)
#define INITIALSTRINGBUFFERSIZE		(64)


/*************************************************************************************
 *
 * Prototypes
 *
 ************************************************************************************/
void Relay( SOCKADDR_IN aSocketAddress, char *aSerialPortName );
void RelaySession( SOCKADDR_IN aSocketAddress, CSerialPort *aSerialPort );
void PrintRelayError( TRelayError aRelayError, int aErrCode );


/*************************************************************************************
 *
 * main()
 *
 ************************************************************************************/
int main( int argc, char *argv[] )
{
	char *serial_port_name;
	SOCKADDR_IN socket_address;
	WORD version;
	WSADATA wsaData;
	int err;

	// check params
	if( argc < 3 ) {
		fprintf( stderr, "usage: %s <tcp_host> <tcp_port> <serial_port>\n", argv[0] );
		return -1;
	}

	// process the params
	memset( &socket_address, 0, sizeof(socket_address) );
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons( atoi(argv[2]) );
	socket_address.sin_addr.S_un.S_addr = inet_addr( argv[1] );
	serial_port_name = argv[3];

	// start the socket subsystem
	version = MAKEWORD( 2, 2 );
	err = WSAStartup( version, &wsaData );
	assert( err == 0 );

	// start the relay
	Relay( socket_address, serial_port_name );

	// done
	WSACleanup();
	return 0;
}


/*************************************************************************************
 *
 * Relay()
 *
 ************************************************************************************/
void Relay( SOCKADDR_IN aSocketAddress, char *aSerialPortName )
{
	int err;
	CSerialPort serial_port;

	// repeat...
	while( 1 ) {

		// open the serial port - if this fails then pause and retry
		err = serial_port.OpenPort( aSerialPortName );
		if( err != 0 ) {
			fprintf( stderr, "Failed to open serial port %s (%d)\n", aSerialPortName, err );
			Sleep( TO_RETRY );
			continue;
		}

		// try and establish a session (might succeed, might fail, makes no difference here)
		RelaySession( aSocketAddress, &serial_port );

		// close the port
		serial_port.ClosePort();
	}

	// done
	return;
}


/*************************************************************************************
 *
 * RelaySession()
 *
 ************************************************************************************/
void RelaySession( SOCKADDR_IN aSocketAddress, CSerialPort *aSerialPort )
{
	char recv_buffer;
	int err, errcode, bytes_read = 0, slen, initial_string_buffer_length;
	CSerialTcpRelay relay;
	TRelayError rerr;

	int current_at_state = 0;
	char expected_at_char[] = { 'A', 'T', 0x0d, 0 };
	char initial_string_buffer[INITIALSTRINGBUFFERSIZE] = { 0 };

	// wait for some bytes to arrive on the port
	fprintf( stdout, "Waiting for new session\n" );
	while( 1 ) {

		// read from the serial port - if there is an error then exit
		bytes_read = 1;
		err = aSerialPort->ReceiveBytes( &recv_buffer, &bytes_read );
		if( err != 0 ) {
			fprintf( stderr, "Error reading from the serial port (%s)\n", strerror(err) );
			break;
		}

		// if nothing was read then just continue
		if( bytes_read == 0 ) {
			continue;
		}

		// if we read a char that does not match the ignore string then try and
		// create a session
		if( recv_buffer != expected_at_char[current_at_state] ) {
			strncpy( initial_string_buffer, expected_at_char, INITIALSTRINGBUFFERSIZE );
			initial_string_buffer[current_at_state] = 0;
			slen = strlen( initial_string_buffer );
			assert( slen < (INITIALSTRINGBUFFERSIZE-2) );
			initial_string_buffer[slen] = recv_buffer;
			initial_string_buffer[slen+1] = 0;
			initial_string_buffer_length = strlen( initial_string_buffer );
			break;
		}

		// otherwise update the at_state
		current_at_state += 1;
		current_at_state = ((expected_at_char[current_at_state] == 0) ? 0 : current_at_state);
	}
	
	// some data was received - try and start a session
	rerr = relay.InitialiseRelay( aSerialPort, aSocketAddress, initial_string_buffer, initial_string_buffer_length, &errcode );
	if( rerr != RE_SUCCESS ) {
		PrintRelayError( rerr, errcode );
		return;
	}
	fprintf( stdout, "Session established\n" );

	// session established - execute
	rerr = relay.ExecuteRelay();
	if( rerr != RE_SUCCESS ) {
		fprintf( stderr, "ExecuteRelay returned %d\n", rerr );
	}
	fprintf( stdout, "Session closed\n" );
	
	// do time-wait 
	Sleep( TO_TIMEWAIT );
}


/*************************************************************************************
 *
 * PrintRelayError()
 *
 ************************************************************************************/
void PrintRelayError( TRelayError aRelayError, int aErrCode )
{
	char *relay_error_string[] = { NULL, "socket() failed", "connect() failed", "initial send() failed" };
	fprintf( stderr, "Failed to initialise the relay - %s - %d\n", relay_error_string[aRelayError], aErrCode );
}




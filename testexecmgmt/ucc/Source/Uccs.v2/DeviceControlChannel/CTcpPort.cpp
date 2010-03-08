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
*
*/



#include <stdio.h>
#include <assert.h>

#include "CTcpPort.h"

CTcpPort::CTcpPort()
: iPort(0)
{
}

CTcpPort::~CTcpPort()
{
}

int CTcpPort::OpenPort( char *aPort )
{
	WORD version;
	WSADATA wsaData;

	version = MAKEWORD( 2, 2 );
	if( WSAStartup( version, &wsaData ) )
	{
		return WSAGetLastError();
	}

	// Create a socket
	iLocalSock = socket( AF_INET, SOCK_STREAM, 0 ) ;
	if( iLocalSock == INVALID_SOCKET )
	{
		return WSAGetLastError();
	}

	// Set iPort for later use
	iPort = atoi( aPort );

	// Set the socket
	iLocalAddr.sin_family = AF_INET;		
	iLocalAddr.sin_port = htons( iPort );
	iLocalAddr.sin_addr.S_un.S_addr = inet_addr( "0.0.0.0" );

	// Bind
	if( bind(iLocalSock,(struct sockaddr*)&iLocalAddr, sizeof(SOCKADDR_IN)) != 0 )
	{	
		return WSAGetLastError();
	}

	// Listen to the socket
	if( listen( iLocalSock, 1 ) == SOCKET_ERROR ) 
	{
		return WSAGetLastError();
	}
	
	// Wait for the next connection
	int remote_addr_len = sizeof(iRemoteAddr);
	iRemoteSock = accept( iLocalSock, (struct sockaddr*)&iRemoteAddr, &remote_addr_len );
	if( iRemoteSock == INVALID_SOCKET ) 
	{
		return WSAGetLastError();
	}

	return 0;
}

void CTcpPort::ClosePort()
{
	closesocket(iRemoteSock);
	closesocket(iLocalSock);
	WSACleanup();
}

int CTcpPort::ReceiveBytes( char *aBuff, int *aLength )
{
	int received_byte_count = 0;
	int bytes_to_receive = *aLength;
	int bytes_received_this_round = 1;

	while( received_byte_count < bytes_to_receive )
	{
		bytes_received_this_round = recv( iRemoteSock, &(aBuff[received_byte_count]), bytes_to_receive - received_byte_count, 0 );
		if( bytes_received_this_round == SOCKET_ERROR )
		{
			*aLength = received_byte_count;
			return WSAGetLastError();
		}
		else if( bytes_received_this_round == 0 )
		{
			// The socket has been killed

			// Now close it at this end
			closesocket(iRemoteSock);
			closesocket(iLocalSock);
			WSACleanup();

			// Wait for a new connection
			char port[20];
			sprintf(port, "%d", iPort);
			int ret = OpenPort( port );
			if( ret != 0 )
			{
				return ret;
			}
		}

		received_byte_count += bytes_received_this_round;
	}

	*aLength = received_byte_count;
	return 0;
}

int CTcpPort::SendBytes( char *aBuff, int *aLength )
{
	int bytes_sent_this_round;
	int total_bytes_to_send = *aLength;
	int total_bytes_sent = 0;

	while( total_bytes_sent < total_bytes_to_send )
	{
		bytes_sent_this_round = send( iRemoteSock, &(aBuff[total_bytes_sent]), total_bytes_to_send - total_bytes_sent, 0 );
		if( bytes_sent_this_round == SOCKET_ERROR )
		{
			return WSAGetLastError();
		}
		total_bytes_sent += bytes_sent_this_round;
	}
	return 0;
}


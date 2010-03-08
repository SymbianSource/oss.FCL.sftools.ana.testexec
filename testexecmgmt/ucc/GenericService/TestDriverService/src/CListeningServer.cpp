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
* CListeningServer.h
*
*/



#include <stdio.h>

#include "CListeningServer.h"

const int KTimeOut		= 120000; // 2 minute timeout for recieve calls

CListeningServer::CListeningServer()
	{
	}

CListeningServer::~CListeningServer()
	{
	}

int CListeningServer::Open( const int aPort )
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

	iPort = aPort;

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

	// Finally set the socket timeout for recieve calls (2 mins)
	return setsockopt( iRemoteSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&KTimeOut, sizeof(KTimeOut) );
	}

void CListeningServer::Close()
{
	closesocket(iRemoteSock);
	closesocket(iLocalSock);
	WSACleanup();
}

int CListeningServer::Send( const int aValue )
	{
	int ret = 0;
	int bytes_sent_this_round;
	int total_bytes_to_send = sizeof(int);
	int total_bytes_sent = 0;

	int pre_send_data = htonl(aValue);
	char* buf = (char*)&pre_send_data;

	while( (total_bytes_sent < total_bytes_to_send) && !ret )
		{
		bytes_sent_this_round = send( iRemoteSock, &(buf[total_bytes_sent]), total_bytes_to_send - total_bytes_sent, 0 );
		if( bytes_sent_this_round == SOCKET_ERROR )
			{
			ret = WSAGetLastError();
			}
		total_bytes_sent += bytes_sent_this_round;
		}
	return ret;
	}

int CListeningServer::Recieve( int& aValue )
	{
	int ret = 0;
	int received_byte_count = 0;
	int bytes_to_receive = sizeof(int);
	int bytes_received_this_round = 1;
	char buf[sizeof(int)];
	memset( buf, 0, sizeof(int) );

	while( (received_byte_count < bytes_to_receive) && !ret )
		{
		bytes_received_this_round = recv( iRemoteSock, &(buf[received_byte_count]), bytes_to_receive - received_byte_count, 0 );
		if( bytes_received_this_round == SOCKET_ERROR )
			{
			ret = WSAGetLastError();
			}
		else if( bytes_received_this_round == 0 )
			{
			// The socket has been killed
			// Now close it at this end
			closesocket(iRemoteSock);
			closesocket(iLocalSock);
			WSACleanup();
			ret = INVALID_SOCKET;
			}

		received_byte_count += bytes_received_this_round;
		}

	if( received_byte_count == bytes_to_receive )
		{
		aValue = *((int*)buf);
		aValue = ntohl(aValue);
		}

	return ret;
	}

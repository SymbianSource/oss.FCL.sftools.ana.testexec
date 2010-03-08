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



#include "CTCPTransport.h"

const int	KMaxIpAddressSize		= 30;
const int	KMaxPortSize			= 10;
const char	KSeparator[]			= { ":" };

CTCPTransport::CTCPTransport()
	: iSocket(INVALID_SOCKET)
	{
	}

CTCPTransport::~CTCPTransport()
	{
	WSACleanup();
	}

int CTCPTransport::Connect( const string &aDestination )
	{
	int ret = 0;

	// Extract the ipaddress andport number
	int offset = aDestination.find( KSeparator, 0 );
	if( offset == -1 )
		{
		return SOCKET_ERROR; // Find a better error code
		}
	
	char ipaddress[KMaxIpAddressSize];
	char port[KMaxPortSize];
	memset( ipaddress, 0, sizeof(ipaddress) );
	memset( port, 0, sizeof(port) );
	aDestination.copy( ipaddress, offset, 0 );
	aDestination.copy( port, aDestination.length()-offset, offset+1 );

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( atoi(port) );
	addr.sin_addr.S_un.S_addr =  inet_addr( ipaddress );
	
	WORD version;
	WSADATA wsaData;

	version = MAKEWORD( 2, 2 );
	if( WSAStartup( version, &wsaData ) )
		{
		return WSAGetLastError();
		}

	// open the socket
	iSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( iSocket == INVALID_SOCKET )
		{
		return WSAGetLastError();
		}

	ret = connect( iSocket, (struct sockaddr*)&addr, sizeof(addr) );
	if( ret == SOCKET_ERROR )
		{
		return WSAGetLastError();
		}

	return 0;
	}


void CTCPTransport::Disconnect()
	{
	if( iSocket != INVALID_SOCKET )
		{
		closesocket( iSocket );
		iSocket = INVALID_SOCKET;
		}
	}

int CTCPTransport::RequestSend( const char* aData, const int aLength )
	{
	int bytes_sent_this_round;
	int total_bytes_to_send = aLength;
	int total_bytes_sent = 0;

	while( total_bytes_sent < total_bytes_to_send )
		{
		bytes_sent_this_round = send( iSocket, &(aData[total_bytes_sent]), total_bytes_to_send - total_bytes_sent, 0 );
		if( bytes_sent_this_round == SOCKET_ERROR )
			{
			return WSAGetLastError();
			}
		total_bytes_sent += bytes_sent_this_round;
		}
	return 0;
	}

int CTCPTransport::RequestReceive( char *aBuff, const int aLength )
	{
	int received_byte_count = 0;
	int bytes_to_receive = aLength;
	int bytes_received_this_round = 1;

	while( received_byte_count < bytes_to_receive )
		{
		bytes_received_this_round = recv( iSocket, &(aBuff[received_byte_count]), bytes_to_receive - received_byte_count, 0 );
		if( bytes_received_this_round == SOCKET_ERROR )
			{
			return WSAGetLastError();
			}
		else if( bytes_received_this_round == 0 )
			{
			return SOCKET_ERROR;
			}

		received_byte_count += bytes_received_this_round;
		}

	return 0;
	}


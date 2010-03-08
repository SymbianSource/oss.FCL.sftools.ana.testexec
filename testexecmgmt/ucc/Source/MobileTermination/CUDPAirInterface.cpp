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
#include <memory.h>
#include <assert.h>
#ifndef WIN32
#include <errno.h>
#include <unistd.h>
#endif


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CUDPAirInterface.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#ifndef WIN32
#define SOCKET_ERROR        (-1)
#define INVALID_SOCKET      (-1)
#define ADDRESS_INTEGER     s_addr
#define closesocket(x)     (shutdown(x,SHUT_RDWR),close(x))
#else
typedef int socklen_t;
#endif


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * Constructor - sets the passed pointers and sets the state to disabled
 *
 ******************************************************************************/
CUDPAirInterface::CUDPAirInterface( TPhoneData *aPhoneData, CLog *aLog  )
{
	// verify parameters 
	assert( aPhoneData != NULL );
	assert( aLog != NULL );

	// init state
	iPhoneData = aPhoneData;
	iProcessData = NULL;
	iLog = aLog;
	iFilter = NULL;
	memset( &iRemoteAddress, 0, sizeof(iRemoteAddress) );
	memset( &iLocalAddress, 0, sizeof(iLocalAddress) );
	iSockIncoming = INVALID_SOCKET;
	iSockOutgoing = INVALID_SOCKET;
	iExitFlag = 0;
	iRemoteAddressValid = 0;
}

CUDPAirInterface::~CUDPAirInterface( )
{
	// check the status of the sockets
	assert( iSockIncoming == INVALID_SOCKET );
	assert( iSockOutgoing == INVALID_SOCKET );
}


/*******************************************************************************
 *
 * PUBLIC METHOD: LISTEN-THREAD: ListenOnInterface. Binds to local port and 
 * then starts listening on this port for more incoming data. Any incoming data
 * is then sent to the datalink layer.
 *
 ******************************************************************************/
TAirInterfaceError CUDPAirInterface::ListenOnInterface( int *aErrCode )
{
	TDataPathError derr;
	int err, errcode;
	struct sockaddr_in remote_recv_addr;
	socklen_t addrlen;

	// verify params
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// check that the local port is invalid
	assert( iSockIncoming == INVALID_SOCKET );

	// create the listening socket
	iSockIncoming = socket( AF_INET, SOCK_DGRAM, 0 );
	if( iSockIncoming == INVALID_SOCKET ) {
		*aErrCode = GetSocketError();
		return AIE_SOCKET_FAILED;
	}

	// if the exit flag is set then exit now
	if( iExitFlag != 0 ) {
	  closesocket( iSockIncoming );
	  iSockIncoming = INVALID_SOCKET;
	  return AIE_NONE;
	}
	

	// bind the listening socket -- bind to an ephemeral port
	iLocalAddress.sin_family = AF_INET;
	iLocalAddress.sin_port = htons(0);
	iLocalAddress.sin_addr.ADDRESS_INTEGER = INADDR_ANY;
	err = bind( iSockIncoming, (struct sockaddr*)&iLocalAddress, sizeof(iLocalAddress) );

	// get the allocated port info
	addrlen = sizeof(iLocalAddress);
	err = getsockname( iSockIncoming, (sockaddr*)&iLocalAddress, &addrlen );
	assert( err == 0 );

	// check for errors during the bind
	if( err != 0 ) {
	  closesocket( iSockIncoming );
	  iSockIncoming = INVALID_SOCKET;
	  *aErrCode = GetSocketError();
	  return AIE_BIND_FAILED;
	}

	// use the listening socket as the outgoing socket -- no reason we can't do 
	// this. It also means that we know the source port of the outgoing datagrams 
	// and hence we can do some proper NISTNET stuff
	iSockOutgoing = iSockIncoming;
	
	// read from the socket until an error occurs (this may be induced by another
	// thread closing the socket)
	while( 1 ) {
		
		// now read from the socket
		memset( &remote_recv_addr, 0, sizeof(remote_recv_addr) );
		addrlen = sizeof(remote_recv_addr);
		err = recvfrom( iSockIncoming, iPacketBuffer, KPCKTBUFFSIZE, 0, (struct sockaddr*)&remote_recv_addr, &addrlen );

		// check for errors
		if( err == SOCKET_ERROR ) {
		  closesocket( iSockIncoming );
		  iSockIncoming = iSockOutgoing = INVALID_SOCKET;
		  *aErrCode = GetSocketError();
		  return AIE_RECEIVE_FAILED;
		}

		// send the data to the filters
		if( iFilter != NULL ) {
			iFilter->ProcessIncomingData( iPacketBuffer, err );
		}

		// otherwise we have data to send to the datalink layer
		derr = iProcessData->ProcessUUData( iPacketBuffer, err, &errcode );
		if( derr != DPE_NONE ) {	
			iLog->WriteLogEntry( SV_WARNING, "CUDPAirInterface::ListenOnInterface", "ProcessUUData returned", derr, errcode );
		}
	}

	// code should never get here
	assert( !"INVALID CODE PATH" );
	return AIE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: MAIN-THREAD: StopInterface. Closes the socket (if it is open)
 * was called. We can't guarantee when the receiving thread will return. But it
 * must before any other calls can be made.
 *
 ******************************************************************************/
int CUDPAirInterface::StopInterface()
{
	// set the exit flag
	iExitFlag = 1;

	// close the socket if it is open
	if( iSockIncoming != INVALID_SOCKET ) {
	  closesocket( iSockIncoming );
	  iSockIncoming = iSockOutgoing = INVALID_SOCKET;
	}

	// done 
	return 0;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: GetLocalAddress 
 *
 ******************************************************************************/
void CUDPAirInterface::GetLocalAddress( struct sockaddr_in *aLocalAddress )
{
	assert( aLocalAddress != NULL );
	*aLocalAddress = iLocalAddress;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: SetRemoteAddress
 *
 ******************************************************************************/
void CUDPAirInterface::GetRemoteAddress( struct sockaddr_in *aRemoteAddress )
{
	*aRemoteAddress = iRemoteAddress;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: SetRemoteAddress
 *
 ******************************************************************************/
void CUDPAirInterface::SetRemoteAddress( struct sockaddr_in aRemoteAddress )
{
	iRemoteAddressValid = 1;
	iRemoteAddress = aRemoteAddress;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: SetDatalink
 *
 ******************************************************************************/
void CUDPAirInterface::SetDatalink( IProcessData *aProcessData )
{
	iProcessData = aProcessData;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD. SetFilter, called by the main setup call in 
 * cphone - links up all the bits.
 *
 ******************************************************************************/
void CUDPAirInterface::SetFilter( IFilter *aFilter )
{
	iFilter = aFilter;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: TE-THREAD: SendPacket, when data is received on the te 
 * channel it eventually makes its way here (in its thread) to send the 
 * data out the uu interface.
 *
 ******************************************************************************/
TDataPathError CUDPAirInterface::SendPacket( char *data, int len, int *aErrCode )
{
	int err;

	//verify the params
	assert( data != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// make sure the remote address is set - not an error
	if( iRemoteAddressValid == 0 ) {
		return DPE_NONE;
	}

	// send the packet - send in 4k chunks to avoid system defined UDP packet size limit
	err = sendto( iSockOutgoing, data, len, 0, (struct sockaddr*)&iRemoteAddress, sizeof(iRemoteAddress) );
	if( err == SOCKET_ERROR ) {
		*aErrCode = GetSocketError();
		return DPE_SEND_FAILED;
	}
	
	// done
	return DPE_NONE;
}


/*******************************************************************************
 *
 * PRIVATE METHODS: Helper functions
 *
 ******************************************************************************/
int CUDPAirInterface::GetSocketError()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

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
#ifndef WIN32
#include <unistd.h>
#include <errno.h>
#endif


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CTCPTEChannel.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#ifndef WIN32
#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)
#define Sleep(x) sleep(x/1000)
#else
#define SHUT_RDWR SD_BOTH
#endif


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/
#ifndef WIN32
#define closesocket(x) (shutdown(x,SHUT_RDWR),close(x))
#endif


/*******************************************************************************
 *
 * Constructor -- initialise all state
 *
 ******************************************************************************/
CTcpTeChannel::CTcpTeChannel( TPhoneData *aPhoneData, CLog *aLog )
{
	// verify args
	assert( aPhoneData != NULL );
	assert( aLog != NULL );

	// initialise state
	iPhoneData = aPhoneData;
	iDatalink = NULL;
	iFilter = NULL;
	iLog = aLog;
	iSock = INVALID_SOCKET;
	iStreamBuffer[0] = 0;
	iExitFlag = 0;
	iSocketSetFlag = 0;
}

CTcpTeChannel::~CTcpTeChannel()
{
	assert( iSock == INVALID_SOCKET );
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD. SetDatalink, called by the main setup call in 
 * cphone - links up all the bits.
 *
 ******************************************************************************/
void CTcpTeChannel::SetDatalink( IProcessData *aDatalink )
{
	iDatalink = aDatalink;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD. SetFilter, called by the main setup call in 
 * cphone - links up all the bits.
 *
 ******************************************************************************/
void CTcpTeChannel::SetFilter( IFilter *aFilter )
{
	iFilter = aFilter;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: SERVER-THREAD: 
 *
 ******************************************************************************/
TChannelError CTcpTeChannel::SetSocket( int aSocket )
{
	// check that the socket is sane
	assert( aSocket >= 0 );

	// check that we haven't already set if
	if( iSocketSetFlag != 0 ) {
		return CE_SOCKET_ALREADY_SET;
	}

	// ok - set the socket
	iSock = aSocket;
	iSocketSetFlag = 1;
	return CE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: LISTEN-THREAD: ListenOnChannel. This is the entry point for 
 * the listening thread. It sits in the loop below and passes all received data 
 * to the datalink layer. 
 *
 ******************************************************************************/
TChannelError CTcpTeChannel::ListenOnChannel( int *aErrCode )
{
	TDataPathError derr;
	int err;
	int errcode;

	// check the params
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// listen on the socket until it closes
	while( 1 ) {

		// if the socket is not defined then just wait for it
		if( iSock == INVALID_SOCKET ) {
			Sleep( 1000 );
			if( iExitFlag != 0 ) {
				return CE_NONE;
			}
			continue;
		}

		// read from socket if it is defined
		err = recv( iSock, iStreamBuffer, KBUFFSIZE, 0 );

		// if the socket has been closed then cleanup and return
		if( err == 0 ) {
		  closesocket( iSock );
		  iSock = INVALID_SOCKET;
		  return CE_NONE;
		}

		// if an error has occured then report it and cleanup
		if( err == SOCKET_ERROR ) {
			*aErrCode = GetSocketError();
			closesocket( iSock );
			iSock = INVALID_SOCKET;
			return CE_RECEIVE_FAILED;
		}

		// pass the data received through the filters. 
		// : the control flow below allows the filter to analyse the data, and also change 
		// the data in-place. However, it doesn't allow the interface to change the size of the 
		// data (limiting the modifications it can make). This isn't a problem now since no 
		// filters do this -- but when this is needed it should be implemented here, and in the same
		// place in the uu interface.
		if( iFilter != NULL ) {
			iFilter->ProcessOutgoingData( iStreamBuffer, err );
		}

		// Now pass the data through to the datalink layer. A datalink layer MUST be
		// defined if the socket is defined.
		assert( iDatalink != NULL );
		derr = iDatalink->ProcessTEData( iStreamBuffer, err, &errcode );
		if( derr != DPE_NONE ) {	
			iLog->WriteLogEntry( SV_WARNING, "CTcpTeChannel::ListenOnChannel", "ProcessTEData returned error", derr, errcode );
		}
	}

	// should never get here
	assert( !"INVALID CODE PATH" );
	return CE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: MAIN-THREAD: StopChannel. This closes the socket (if open) 
 * which will force the recv to return 0 and the listen-thread will return. It
 * also sets the exit flag for when we want to stop the channel before it 
 * really even started (poor thing).
 *
 ******************************************************************************/
void CTcpTeChannel::StopChannel()
{
	// close the socket
	if( iSock != INVALID_SOCKET ) {
	  closesocket( iSock );
	  iSock = INVALID_SOCKET;
	}

	// set the exit flag
	iExitFlag = 1;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: AIR INTERFACE THREAD: SendPacket. This is called by the data
 * link layer when it receives some data from the air interface that it now
 * decides to forward on.
 *
 ******************************************************************************/
TDataPathError CTcpTeChannel::SendPacket( char *data, int len, int *aErrCode )
{
	int err;
	int bytes_to_send;
	int bytes_sent;

	// check params
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// if there is no connection then return. This is not unexpected or bad so
	// no error is returned.
	if( iSock == INVALID_SOCKET ) {
		return DPE_NONE;
	}

	// otherwise send data
	bytes_to_send = len;
	bytes_sent = 0;
	while( bytes_sent < bytes_to_send ) {
		err = send( iSock, &(data[bytes_sent]), bytes_to_send - bytes_sent, 0 );
		if( err == -1 ) {
			*aErrCode = GetSocketError();
			closesocket( iSock );
			iSock = INVALID_SOCKET;
			iSocketSetFlag = 0;
			return DPE_SEND_FAILED;
		}
		bytes_sent += err;
	}

	// success
	return DPE_NONE;
}


/*******************************************************************************
 *
 * PRIVATE METHODS: HELPER FUNCTIONS
 *
 ******************************************************************************/
int CTcpTeChannel::GetSocketError()
{
#ifdef WIN32
  return WSAGetLastError();
#else
  return errno;
#endif
}

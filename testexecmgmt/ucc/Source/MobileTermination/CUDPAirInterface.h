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
* CUDPAirInterface
* Air Interface for the mobile termination. Sends and receives data over UDP
* sockets using addresses and ports specified.
*
*/



#ifndef __CUDPAIRINTERFACE_H__
#define __CUDPAIRINTERFACE_H__

/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "MTInterfaces.h"
#include "CLog.h"

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define KPCKTBUFFSIZE ((1024)*64)


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum
{
	AIE_NONE,
	AIE_RECEIVE_FAILED,
	AIE_SOCKET_FAILED,
	AIE_BIND_FAILED,
} TAirInterfaceError;


/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CUDPAirInterface : public IAirInterface
{
public:
	// Construction
	CUDPAirInterface( TPhoneData *aPhoneData, CLog *aLog  );
	virtual ~CUDPAirInterface();

	// Control
	TAirInterfaceError ListenOnInterface( int *aErrCode );
	int StopInterface();
	
	// Accessors
	void GetLocalAddress( struct sockaddr_in *aLocalAddress );
	void GetRemoteAddress( struct sockaddr_in *aRemoteAddress );
	void SetRemoteAddress( struct sockaddr_in aRemoteAddress );
	void SetDatalink( IProcessData *aProcessData );
	void SetFilter( IFilter *aFilter );

	// IAirInterface
	virtual TDataPathError SendPacket( char *data, int len, int *aErrCode );

private:
	int GetSocketError();

private:
	TPhoneData *iPhoneData;
	IProcessData *iProcessData;
	CLog *iLog;
	IFilter *iFilter;

	struct sockaddr_in iRemoteAddress;
	struct sockaddr_in iLocalAddress;

	char iPacketBuffer[KPCKTBUFFSIZE];

	int iSockIncoming;
	int iSockOutgoing;
	int iExitFlag;
	int iRemoteAddressValid;
};

#endif //__CUDPAIRINTERFACE_H__

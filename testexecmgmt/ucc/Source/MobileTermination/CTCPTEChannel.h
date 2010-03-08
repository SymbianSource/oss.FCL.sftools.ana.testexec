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
* CTCPTEChannel
* MSL Channel for the mobile termination. Sends and receives data over a TCP
* socket. This end is the server and listens on a address / port. 
*
*/



#ifndef __CTCPTECHANNEL_H__
#define __CTCPTECHANNEL_H__

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
#define KBUFFSIZE			4096


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum
{
	CE_NONE,
	CE_RECEIVE_FAILED,
	CE_SOCKET_ALREADY_SET
} TChannelError;


/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CTcpTeChannel : public ITEChannel
{
public:
	// Construction
	CTcpTeChannel( TPhoneData *aPhoneData, CLog *aLog );
	virtual ~CTcpTeChannel();

	// Set connections
	void SetDatalink( IProcessData *aDatalink );
	void SetFilter( IFilter *aFilter );
	TChannelError SetSocket( int aSocket );

	// Control
	TChannelError ListenOnChannel( int *aErrCode );
	void StopChannel();

	// ITEChannel
	virtual TDataPathError SendPacket( char *data, int len, int *aErrCode );

private:
	void CleanupChannel();
	int GetSocketError();

private:
	TPhoneData *iPhoneData;
	IProcessData *iDatalink;
	IFilter *iFilter;
	CLog *iLog;
	int iSock;
	int iExitFlag;
	int iSocketSetFlag;
	char iStreamBuffer[KBUFFSIZE];
};

#endif //__CTCPTECHANNEL_H__ 

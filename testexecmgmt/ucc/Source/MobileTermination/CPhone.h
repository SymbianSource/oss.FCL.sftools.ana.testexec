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
* CPhone
* Stores the state of the phone.
*
*/



#ifndef __CPHONE_H__
#define __CPHONE_H__


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
#include "../ThreadLibrary/CAThread.h"
#include "MTInterfaces.h"
#include "CDatalinkNull.h"
#include "CDatalinkPacketise.h"
#include "CTCPTEChannel.h"
#include "CUDPAirInterface.h"
#include "CLogPPPFilter.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define FILTER_NONE  0
#define FILTER_PPP   1


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum {
	MTL_SUCCESS,
	MTL_INVALID_FILTER_CONFIG,
	MTL_INVALID_DATALINK_LAYER,	
	MTL_FAILED_TO_CREATE_AIR_INTERFACE_THREAD,
	MTL_FAILED_TO_CREATE_TE_CHANNEL_THREAD,
	MTL_FAILED_TO_CREATE_MAIN_THREAD,
	MTL_TE_CHANNEL_SOCKET_ALREADY_SET,
	MTL_INVALID_STATE,
} MTError;

// This structure is defined in mobster.x 
#ifndef __MOBSTER_H__
typedef enum {
	DL_INVALID,
	DL_NULL,
	DL_PACKETISE
} TDatalinkLayer;
#endif

typedef enum {
	MTS_INIT,
	MTS_RUNNING,
	MTS_SHUTDOWN_ALL_BUT_MAIN,
	MTS_SHUTDOWN_ALL,
} MTStatus;

/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CPhone
{
public:
	// Construction
	CPhone();
	~CPhone();

	// Control interface
	MTError StartPhone( int aPhoneID, int aDatalinkConfig, int aFilterConfig, int *aErrCode ); 
	MTError StopPhone();
	MTError GetLocalUUAddress( struct sockaddr_in *sockaddr );
	MTError GetRemoteUUAddress( struct sockaddr_in *sockaddr );
	MTError SetRemoteUUAddress( struct sockaddr_in sockaddr );
	MTError SetTeSocket( int aSock );
	MTStatus GetStatus();
	CLog *GetLog();

private:
	MTError InternalInitialisePhone( int aPhoneID, int aDatalinkConfig, int aFilterConfig, int *aErrCode );
	MTError InternalRunPhone( int *aErrCode );
	void CleanupState( int aRequestSource );
	MTError CreateDatalinkLayer( int aDatalinkConfig );
	void DeleteDatalinkLayer();
	MTError CreateFilters( int aFilterConfig );
	void DeleteFilters();

	// Thread entry point
	friend int MainThreadProc( CPhone *aPhone );
	friend int AirInterfaceThreadProc( CPhone *aPhone );
	friend int TEChannelThreadProc( CPhone *aPhone );

private:
	// Device logfile
	CLog iLog;

	// All the data associated with the phone
	TPhoneData iPhoneData;

	// Air Interface
	CUDPAirInterface iUdpAirInterface;

	// TE Channel
	CTcpTeChannel iTcpTeChannel;

	// Flags
	int iExitFlag;
	MTStatus iStatus;

	// Filters 
	CLogPppFilter iFilterPpp;
	IFilter *iFilter;

	// Datalink Layer
	CDatalinkNull *iDatalinkNull;
	CDatalinkPacketise *iDatalinkPacketise;
	IProcessData *iProcessData;

	// Handles for each of the threads
	CAThread iMainThread;
	CAThread iAirInterfaceThread;
	CAThread iTEChannelThread;
};

#endif //__CPHONE_H__

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
* Standard Interface Values
*
*/


#include "../include/penstd.x"

/*******************************************************************************
 *
 * Mobster Specific Definitions
 *
 ******************************************************************************/
const MAXADDRLEN			= 64;
const MAXPORTLEN			= 16;
const INVALID_DEVICE_ID		= 0xFFFFFFFF;

/*******************************************************************************
 *
 * Error codes
 *
 ******************************************************************************/
const ERR_DEVICETHREADCREATIONFAILED	= -100;
const ERR_DEVICENOTRUNNING				= -101;
const ERR_STOPFAILED					= -102;
const ERR_STARTUPFAILED					= -103;
const ERR_CREATE_SOCKET_FAILED			= -104;
const ERR_BIND_FAILED					= -105;
const ERR_LISTEN_FAILED					= -106;
const ERR_CREATE_SERVER_THREAD_FAILED	= -117;
const ERR_ACCEPT_FAILED					= -107;
const ERR_CSPROTOCOLFAILED				= -108;
const ERR_CREATELOGFAILED				= -109;
const ERR_STARTPHONEFAILED				= -110;
const ERR_CONNECTSOCKETFAILED			= -111;
const ERR_OPENSERIALPORTFAILED			= -112;
const ERR_DEVICEINITIALISING					= -113;
const ERR_STOPREQUESTED							= -114;
const ERR_FAILEDTORESOLVEOUTGOINGAIRADDRESS		= -115;
const ERR_FAILEDTORESETSTATE					= -116;
const ERR_CHANNEL_SOCKET_ALREADY_SET			= -118;

/*******************************************************************************
 *
 * Values passed as parameters
 *
 ******************************************************************************/
const FILTER_NONE						= 0;
const FILTER_PPP						= 1;

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
enum TDatalinkLayer {
	DL_INVALID,
	DL_NULL,
	DL_PACKETISE
};

enum TChannelStatus {
	CS_INACTIVE,
	CS_ACTIVE,
	CS_CLOSED
};

enum TDeviceExitReason {
	DER_INVALID,
	DER_USER_REQUEST,
	DER_START_PHONE_FAILED,
	DER_UNKNOWN
};

struct TChannelAddress {
	int iPort;
};

struct TUUAddress {
	int iDeviceID;
	int iRemoteAddress;
	int iRemotePort;
};

struct TDeviceDesc {

	/* Input Values */
	TDatalinkLayer	iDatalinkConfig;
	int				iFilterConfig;

	/* Public State */
	int					iMTID;
	int					iTEID;
	char				iLocalAirInterfacePort[MAXPORTLEN];
	char				iRemoteAirInterfaceAddress[MAXADDRLEN];
	char				iRemoteAirInterfacePort[MAXPORTLEN];
	TChannelStatus		iDeviceStatus;
	TDeviceExitReason	iDeviceExitReason;
	int					iDeviceExitSubReason;
	int					iDeviceExitDetail;
};

typedef opaque TVarData<>;

/*******************************************************************************
 *
 * MOBSTER Interface
 *
 ******************************************************************************/
program MOBSTER { 
	version MOBSTER_VERSION {

		int SS_STARTUPRPCSERVICE( struct TChannelAddress )				= 1;
		int SC_SHUTDOWNRPCSERVICE( int )								= 2;
		TComponentList LIST_DEVICES( void )								= 9;

		int CSTR_STARTDEVICE( TDeviceDesc )								= 3;
		int DSTR_REMOVEDEVICE( int )									= 7;

		TDeviceDesc GETDEVICEINFO( int )								= 4;
		TVarData GETDEVICELOG( int )									= 5;
		int STOPDEVICE( int )											= 6;
		int SETREMOTEUUADDRESS( struct TUUAddress )						= 10;
	} = 1; 
} = 0x34630201;


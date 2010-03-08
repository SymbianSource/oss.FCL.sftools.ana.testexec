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




#ifndef __MOBSTER_H__
#define __MOBSTER_H__
#include <rpc/types.h>
#include <rpc/rpc.h>

#define ERR_NONE 0
#define ERR_INVALID_ERROR_CODE -16
#define ERR_SERVICE_ALREADY_STARTED -1
#define ERR_SERVICE_NOT_RUNNING -2
#define ERR_ACTIVE_USER_SESSIONS -3
#define ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS -4
#define ERR_FAILED_TO_CREATE_COMPONENT_MANAGER -5
#define ERR_FAILED_TO_RETRIEVE_KEY -6
#define ERR_INSTANCE_DOES_NOT_EXIST -7
#define ERR_CANNOT_CREATE_NEW_INSTANCE -9
#define ERR_MISSING_PARAMETER -17
#define ERR_INVALID_METHOD -8
#define ERR_STUB_ALREADY_CONNECTED -10
#define ERR_FAILED_TO_CONNECT -11
#define ERR_STUB_NOT_CONNECTED -12
#define ERR_INVALID_RV_POINTER -13
#define ERR_RPC_ERROR -14
#define ERR_INVALID_SERVICE -15
#define ERR_INVALIDARG -50
#define ERR_INVALIDSTATE -51
#define ERR_GENERAL -52
#define INFO_MAXIMUM_OBJECTS 256

typedef struct {
	u_int TComponentList_len;
	int *TComponentList_val;
} TComponentList;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TComponentList(...);
}
#else
bool_t xdr_mobster_TComponentList();
#endif

#define MAXADDRLEN 64
#define MAXPORTLEN 16
#define INVALID_DEVICE_ID 0xFFFFFFFF
#define ERR_DEVICETHREADCREATIONFAILED -100
#define ERR_DEVICENOTRUNNING -101
#define ERR_STOPFAILED -102
#define ERR_STARTUPFAILED -103
#define ERR_CREATE_SOCKET_FAILED -104
#define ERR_BIND_FAILED -105
#define ERR_LISTEN_FAILED -106
#define ERR_CREATE_SERVER_THREAD_FAILED -117
#define ERR_ACCEPT_FAILED -107
#define ERR_CSPROTOCOLFAILED -108
#define ERR_CREATELOGFAILED -109
#define ERR_STARTPHONEFAILED -110
#define ERR_CONNECTSOCKETFAILED -111
#define ERR_OPENSERIALPORTFAILED -112
#define ERR_DEVICEINITIALISING -113
#define ERR_STOPREQUESTED -114
#define ERR_FAILEDTORESOLVEOUTGOINGAIRADDRESS -115
#define ERR_FAILEDTORESETSTATE -116
#define ERR_CHANNEL_SOCKET_ALREADY_SET -118
#define FILTER_NONE 0
#define FILTER_PPP 1

enum TDatalinkLayer {
	DL_INVALID = 0,
	DL_NULL = 1,
	DL_PACKETISE = 2,
};
typedef enum TDatalinkLayer TDatalinkLayer;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TDatalinkLayer(...);
}
#else
bool_t xdr_mobster_TDatalinkLayer();
#endif


enum TChannelStatus {
	CS_INACTIVE = 0,
	CS_ACTIVE = 1,
	CS_CLOSED = 2,
};
typedef enum TChannelStatus TChannelStatus;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TChannelStatus(...);
}
#else
bool_t xdr_mobster_TChannelStatus();
#endif


enum TDeviceExitReason {
	DER_INVALID = 0,
	DER_USER_REQUEST = 1,
	DER_START_PHONE_FAILED = 2,
	DER_UNKNOWN = 3,
};
typedef enum TDeviceExitReason TDeviceExitReason;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TDeviceExitReason(...);
}
#else
bool_t xdr_mobster_TDeviceExitReason();
#endif


struct TChannelAddress {
	int iPort;
};
typedef struct TChannelAddress TChannelAddress;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TChannelAddress(...);
}
#else
bool_t xdr_mobster_TChannelAddress();
#endif


struct TUUAddress {
	int iDeviceID;
	int iRemoteAddress;
	int iRemotePort;
};
typedef struct TUUAddress TUUAddress;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TUUAddress(...);
}
#else
bool_t xdr_mobster_TUUAddress();
#endif


struct TDeviceDesc {
	TDatalinkLayer iDatalinkConfig;
	int iFilterConfig;
	int iMTID;
	int iTEID;
	char iLocalAirInterfacePort[MAXPORTLEN];
	char iRemoteAirInterfaceAddress[MAXADDRLEN];
	char iRemoteAirInterfacePort[MAXPORTLEN];
	TChannelStatus iDeviceStatus;
	TDeviceExitReason iDeviceExitReason;
	int iDeviceExitSubReason;
	int iDeviceExitDetail;
};
typedef struct TDeviceDesc TDeviceDesc;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TDeviceDesc(...);
}
#else
bool_t xdr_mobster_TDeviceDesc();
#endif


typedef struct {
	u_int TVarData_len;
	char *TVarData_val;
} TVarData;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mobster_TVarData(...);
}
#else
bool_t xdr_mobster_TVarData();
#endif


#define MOBSTER ((u_long)0x34630201)
#define MOBSTER_VERSION ((u_long)1)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_1( TChannelAddress * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_1();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_1( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_1();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)9)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_1( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_1();
#endif /* __cplusplus */
#define CSTR_STARTDEVICE ((u_long)3)
#ifdef __cplusplus
extern "C" {
extern int *cstr_startdevice_1( TDeviceDesc * arg, CLIENT *cl );
}
#else
extern int *cstr_startdevice_1();
#endif /* __cplusplus */
#define DSTR_REMOVEDEVICE ((u_long)7)
#ifdef __cplusplus
extern "C" {
extern int *dstr_removedevice_1( int * arg, CLIENT *cl );
}
#else
extern int *dstr_removedevice_1();
#endif /* __cplusplus */
#define GETDEVICEINFO ((u_long)4)
#ifdef __cplusplus
extern "C" {
extern TDeviceDesc *getdeviceinfo_1( int * arg, CLIENT *cl );
}
#else
extern TDeviceDesc *getdeviceinfo_1();
#endif /* __cplusplus */
#define GETDEVICELOG ((u_long)5)
#ifdef __cplusplus
extern "C" {
extern TVarData *getdevicelog_1( int * arg, CLIENT *cl );
}
#else
extern TVarData *getdevicelog_1();
#endif /* __cplusplus */
#define STOPDEVICE ((u_long)6)
#ifdef __cplusplus
extern "C" {
extern int *stopdevice_1( int * arg, CLIENT *cl );
}
#else
extern int *stopdevice_1();
#endif /* __cplusplus */
#define SETREMOTEUUADDRESS ((u_long)10)
#ifdef __cplusplus
extern "C" {
extern int *setremoteuuaddress_1( TUUAddress * arg, CLIENT *cl );
}
#else
extern int *setremoteuuaddress_1();
#endif /* __cplusplus */

#endif /* __MOBSTER_H__ */

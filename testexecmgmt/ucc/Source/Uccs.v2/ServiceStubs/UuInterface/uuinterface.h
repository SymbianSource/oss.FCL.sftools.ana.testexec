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




#ifndef __UUINTERFACE_H__
#define __UUINTERFACE_H__
#include <rpc/types.h>
#include <rpc/rpc.h>

#define ERR_NONE 0
#define ERR_SERVICE_ALREADY_STARTED -1
#define ERR_SERVICE_NOT_RUNNING -2
#define ERR_ACTIVE_USER_SESSIONS -3
#define ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS -4
#define ERR_FAILED_TO_CREATE_COMPONENT_MANAGER -5
#define ERR_FAILED_TO_RETRIEVE_KEY -6
#define ERR_INSTANCE_DOES_NOT_EXIST -7
#define ERR_INVALID_METHOD -8
#define ERR_CANNOT_CREATE_NEW_INSTANCE -9
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
bool_t xdr_uuinterface_TComponentList(...);
}
#else
bool_t xdr_uuinterface_TComponentList();
#endif

#define X_MAX_ADDRESS_LENGTH 64
#define X_TCP 6
#define X_UDP 17
#define ERR_FAILEDTOEXECUTECOMMAND -100
#define ERR_PROCESSEXECUTEFAILED -101
#define ERR_PORTNOTSPECIFIED -103

struct TStartupInfo {
	int iEmpty;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_uuinterface_TStartupInfo(...);
}
#else
bool_t xdr_uuinterface_TStartupInfo();
#endif


struct TConfigValue {
	char iSourceAddress[X_MAX_ADDRESS_LENGTH];
	char iDestinationAddress[X_MAX_ADDRESS_LENGTH];
	int iProtocol;
	int iSrcPort;
	int iDstPort;
	int iValueIntegerPart;
	int iValueFractionPart;
	int iSigmaIntegerPart;
	int iSigmaFractionPart;
	int iCorrelationIntegerPart;
	int iCorrelationFractionPart;
	int iMaximumValueIntegerPart;
	int iMaximumValueFractionPart;
	int iCongestionIntegerPart;
	int iCongestionFractionPart;
};
typedef struct TConfigValue TConfigValue;
#ifdef __cplusplus
extern "C" {
bool_t xdr_uuinterface_TConfigValue(...);
}
#else
bool_t xdr_uuinterface_TConfigValue();
#endif


struct TResult {
	int iStandardResult;
	int iExtendedCode;
	int iSystemError;
};
typedef struct TResult TResult;
#ifdef __cplusplus
extern "C" {
bool_t xdr_uuinterface_TResult(...);
}
#else
bool_t xdr_uuinterface_TResult();
#endif


#define UUINTERFACE ((u_long)0x34630202)
#define UUINTERFACEVER ((u_long)2)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_2( TStartupInfo * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_2();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_2( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_2();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_2( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_2();
#endif /* __cplusplus */
#define ST_SETDELAY ((u_long)3)
#ifdef __cplusplus
extern "C" {
extern TResult *st_setdelay_2( TConfigValue * arg, CLIENT *cl );
}
#else
extern TResult *st_setdelay_2();
#endif /* __cplusplus */
#define ST_SETDROPPERCENTAGE ((u_long)4)
#ifdef __cplusplus
extern "C" {
extern TResult *st_setdroppercentage_2( TConfigValue * arg, CLIENT *cl );
}
#else
extern TResult *st_setdroppercentage_2();
#endif /* __cplusplus */
#define ST_SETDUPLICATEPERCENTAGE ((u_long)5)
#ifdef __cplusplus
extern "C" {
extern TResult *st_setduplicatepercentage_2( TConfigValue * arg, CLIENT *cl );
}
#else
extern TResult *st_setduplicatepercentage_2();
#endif /* __cplusplus */
#define ST_SETBANDWIDTH ((u_long)6)
#ifdef __cplusplus
extern "C" {
extern TResult *st_setbandwidth_2( TConfigValue * arg, CLIENT *cl );
}
#else
extern TResult *st_setbandwidth_2();
#endif /* __cplusplus */
#define ST_SETDRD ((u_long)7)
#ifdef __cplusplus
extern "C" {
extern TResult *st_setdrd_2( TConfigValue * arg, CLIENT *cl );
}
#else
extern TResult *st_setdrd_2();
#endif /* __cplusplus */
#define ST_CLEARSETTINGS ((u_long)8)
#ifdef __cplusplus
extern "C" {
extern TResult *st_clearsettings_2( TConfigValue * arg, CLIENT *cl );
}
#else
extern TResult *st_clearsettings_2();
#endif /* __cplusplus */
#define ST_STOPSERVICE ((u_long)9)
#ifdef __cplusplus
extern "C" {
extern TResult *st_stopservice_2( void * arg, CLIENT *cl );
}
#else
extern TResult *st_stopservice_2();
#endif /* __cplusplus */
#define ST_RESET ((u_long)10)
#ifdef __cplusplus
extern "C" {
extern TResult *st_reset_2( void * arg, CLIENT *cl );
}
#else
extern TResult *st_reset_2();
#endif /* __cplusplus */

#endif /* __UUINTERFACE_H__ */

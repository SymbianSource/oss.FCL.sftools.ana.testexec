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




#ifndef __PPPCONTROLLER_H__
#define __PPPCONTROLLER_H__
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
bool_t xdr_pppcontroller_TComponentList(...);
}
#else
bool_t xdr_pppcontroller_TComponentList();
#endif

#define MAXADDRLEN 64
#define MAXPORTLEN 16
#define MAXCONFIGURATIONLENGTH 1024
#define MAXSESSIONNAMELEN 16
#define INVALID_SESSION_ID 0xFFFFFFFF
#define ERR_WAIT_PROCESS_ERROR -103
#define ERR_START_PROCESS_ERROR -105
#define ERR_STOP_PROCESS_ERROR -106
#define ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE -111
#define ERR_SET_SIGNAL_ERROR -120
#define ERR_LOG_FILE_ERROR -121
#define ERR_MULTIPLE_MATCHES_IN_LOG_FILE -122

enum TPppSessionStatus {
	SS_NOT_STARTED = 0,
	SS_CONNECTING = 1,
	SS_CONNECTED = 2,
	SS_DISCONNECTING = 3,
	SS_DISCONNECTED = 4,
};
typedef enum TPppSessionStatus TPppSessionStatus;
#ifdef __cplusplus
extern "C" {
bool_t xdr_pppcontroller_TPppSessionStatus(...);
}
#else
bool_t xdr_pppcontroller_TPppSessionStatus();
#endif


struct TStartupInfo {
	int iEmpty;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_pppcontroller_TStartupInfo(...);
}
#else
bool_t xdr_pppcontroller_TStartupInfo();
#endif


struct TPppSessionConfig {
	char iMobsterAddress[MAXADDRLEN];
	char iMobsterPort[MAXPORTLEN];
	char iSessionConfiguration[MAXCONFIGURATIONLENGTH];
	int iMTID;
};
typedef struct TPppSessionConfig TPppSessionConfig;
#ifdef __cplusplus
extern "C" {
bool_t xdr_pppcontroller_TPppSessionConfig(...);
}
#else
bool_t xdr_pppcontroller_TPppSessionConfig();
#endif


struct TPppSessionDesc {
	int iErrorCode;
	int iErrorDetail;
	TPppSessionConfig iConfig;
	TPppSessionStatus iSessionStatus;
	char iInterfaceName[MAXSESSIONNAMELEN];
	int iLocalIPAddress;
	int iRemoteIPAddress;
	int iProcessStatus;
	int iProcessExitReason;
	int iProcessExitCode;
};
typedef struct TPppSessionDesc TPppSessionDesc;
#ifdef __cplusplus
extern "C" {
bool_t xdr_pppcontroller_TPppSessionDesc(...);
}
#else
bool_t xdr_pppcontroller_TPppSessionDesc();
#endif


struct TResult {
	int iStandardResult;
	int iExtendedCode;
	int iSystemError;
};
typedef struct TResult TResult;
#ifdef __cplusplus
extern "C" {
bool_t xdr_pppcontroller_TResult(...);
}
#else
bool_t xdr_pppcontroller_TResult();
#endif


typedef struct {
	u_int TVarData_len;
	char *TVarData_val;
} TVarData;
#ifdef __cplusplus
extern "C" {
bool_t xdr_pppcontroller_TVarData(...);
}
#else
bool_t xdr_pppcontroller_TVarData();
#endif


#define PPPCONTROLLER ((u_long)0x34630204)
#define PPPCONTROLLER_VERSION ((u_long)4)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_4( TStartupInfo * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_4();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_4( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_4();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_4( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_4();
#endif /* __cplusplus */
#define CSTR_STARTPPPSESSION ((u_long)31)
#ifdef __cplusplus
extern "C" {
extern TResult *cstr_startpppsession_4( TPppSessionConfig * arg, CLIENT *cl );
}
#else
extern TResult *cstr_startpppsession_4();
#endif /* __cplusplus */
#define DSTR_REMOVEPPPSESSION ((u_long)32)
#ifdef __cplusplus
extern "C" {
extern TResult *dstr_removepppsession_4( int * arg, CLIENT *cl );
}
#else
extern TResult *dstr_removepppsession_4();
#endif /* __cplusplus */
#define KILLSESSION ((u_long)5)
#ifdef __cplusplus
extern "C" {
extern TResult *killsession_4( int * arg, CLIENT *cl );
}
#else
extern TResult *killsession_4();
#endif /* __cplusplus */
#define STOPSESSION ((u_long)6)
#ifdef __cplusplus
extern "C" {
extern TResult *stopsession_4( int * arg, CLIENT *cl );
}
#else
extern TResult *stopsession_4();
#endif /* __cplusplus */
#define GETSESSIONINFO ((u_long)7)
#ifdef __cplusplus
extern "C" {
extern TPppSessionDesc *getsessioninfo_4( int * arg, CLIENT *cl );
}
#else
extern TPppSessionDesc *getsessioninfo_4();
#endif /* __cplusplus */
#define GETPPPLOG ((u_long)8)
#ifdef __cplusplus
extern "C" {
extern TVarData *getppplog_4( int * arg, CLIENT *cl );
}
#else
extern TVarData *getppplog_4();
#endif /* __cplusplus */

#endif /* __PPPCONTROLLER_H__ */

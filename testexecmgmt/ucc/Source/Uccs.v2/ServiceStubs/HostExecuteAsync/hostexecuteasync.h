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




#ifndef __HOSTEXECUTEASYNC_H__
#define __HOSTEXECUTEASYNC_H__
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
bool_t xdr_hostexecuteasync_TComponentList(...);
}
#else
bool_t xdr_hostexecuteasync_TComponentList();
#endif

#define MAXCOMMANDLINE 1024
#define ERR_WAIT_PROCESS_ERROR -103
#define ERR_START_PROCESS_ERROR -105
#define ERR_STOP_PROCESS_ERROR -106
#define ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE -111

struct TResult {
	int iStandardResult;
	int iExtendedCode;
	int iSystemError;
};
typedef struct TResult TResult;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecuteasync_TResult(...);
}
#else
bool_t xdr_hostexecuteasync_TResult();
#endif


struct THostExecuteAsyncProcessInfo {
	int iErrorCode;
	int iErrorDetail;
	char iCommandLine[MAXCOMMANDLINE];
	int iProcessStatus;
	int iProcessExitReason;
	int iExitCode;
};
typedef struct THostExecuteAsyncProcessInfo THostExecuteAsyncProcessInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecuteasync_THostExecuteAsyncProcessInfo(...);
}
#else
bool_t xdr_hostexecuteasync_THostExecuteAsyncProcessInfo();
#endif


typedef struct {
	u_int TVarData_len;
	char *TVarData_val;
} TVarData;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecuteasync_TVarData(...);
}
#else
bool_t xdr_hostexecuteasync_TVarData();
#endif


struct TStartupInfo {
	int iDummy;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecuteasync_TStartupInfo(...);
}
#else
bool_t xdr_hostexecuteasync_TStartupInfo();
#endif


#define HOSTEXECUTEASYNC ((u_long)0x34630209)
#define HOSTEXECUTEASYNC_VERSION ((u_long)9)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_9( TStartupInfo * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_9();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_9( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_9();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_9( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_9();
#endif /* __cplusplus */
#define CSTR_STARTPROCESS ((u_long)31)
#ifdef __cplusplus
extern "C" {
extern TResult *cstr_startprocess_9( char ** arg, CLIENT *cl );
}
#else
extern TResult *cstr_startprocess_9();
#endif /* __cplusplus */
#define DSTR_REMOVEPROCESS ((u_long)32)
#ifdef __cplusplus
extern "C" {
extern TResult *dstr_removeprocess_9( int * arg, CLIENT *cl );
}
#else
extern TResult *dstr_removeprocess_9();
#endif /* __cplusplus */
#define KILLPROCESS ((u_long)5)
#ifdef __cplusplus
extern "C" {
extern TResult *killprocess_9( int * arg, CLIENT *cl );
}
#else
extern TResult *killprocess_9();
#endif /* __cplusplus */
#define STOPPROCESS ((u_long)6)
#ifdef __cplusplus
extern "C" {
extern TResult *stopprocess_9( int * arg, CLIENT *cl );
}
#else
extern TResult *stopprocess_9();
#endif /* __cplusplus */
#define GETPROCESSINFO ((u_long)7)
#ifdef __cplusplus
extern "C" {
extern THostExecuteAsyncProcessInfo *getprocessinfo_9( int * arg, CLIENT *cl );
}
#else
extern THostExecuteAsyncProcessInfo *getprocessinfo_9();
#endif /* __cplusplus */
#define GETSTANDARDOUTPUT ((u_long)8)
#ifdef __cplusplus
extern "C" {
extern TVarData *getstandardoutput_9( int * arg, CLIENT *cl );
}
#else
extern TVarData *getstandardoutput_9();
#endif /* __cplusplus */
#define GETSTANDARDERROR ((u_long)9)
#ifdef __cplusplus
extern "C" {
extern TVarData *getstandarderror_9( int * arg, CLIENT *cl );
}
#else
extern TVarData *getstandarderror_9();
#endif /* __cplusplus */

#endif /* __HOSTEXECUTEASYNC_H__ */

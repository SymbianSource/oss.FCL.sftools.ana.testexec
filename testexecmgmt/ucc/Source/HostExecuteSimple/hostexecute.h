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




#ifndef __HOSTEXECUTE_H__
#define __HOSTEXECUTE_H__
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
bool_t xdr_hostexecute_TComponentList(...);
}
#else
bool_t xdr_hostexecute_TComponentList();
#endif

#define MAXCOMMANDLINE 1024
#define ERR_WAIT_PROCESS_ERROR -103
#define ERR_START_PROCESS_ERROR -105
#define ERR_STOP_PROCESS_ERROR -106
#define ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE -111
#define ERR_SET_SIGNAL_ERROR -120
#define ERR_EXECUTE_PROCESS_ERROR -122

typedef struct {
	u_int TVarData_len;
	char *TVarData_val;
} TVarData;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecute_TVarData(...);
}
#else
bool_t xdr_hostexecute_TVarData();
#endif


struct TStartupInfo {
	int iEmpty;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecute_TStartupInfo(...);
}
#else
bool_t xdr_hostexecute_TStartupInfo();
#endif


struct TExecuteRequest {
	char iCommandLine[MAXCOMMANDLINE];
	int iRecordStandardOut;
	int iRecordStandartError;
	int iTimeout;
};
typedef struct TExecuteRequest TExecuteRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecute_TExecuteRequest(...);
}
#else
bool_t xdr_hostexecute_TExecuteRequest();
#endif


struct TExecuteResult {
	int iResult;
	int iErrorCode;
	int iErrorDetail;
	int iProcessExitReason;
	int iProcessExitDetail;
	TVarData iStandardOut;
	TVarData iStandardError;
};
typedef struct TExecuteResult TExecuteResult;
#ifdef __cplusplus
extern "C" {
bool_t xdr_hostexecute_TExecuteResult(...);
}
#else
bool_t xdr_hostexecute_TExecuteResult();
#endif


#define HOSTEXECUTE ((u_long)0x34630203)
#define HOSTEXECUTE_VERSION ((u_long)3)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_3( TStartupInfo * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_3();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_3( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_3();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_3( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_3();
#endif /* __cplusplus */
#define ST_EXECUTECOMMAND ((u_long)3)
#ifdef __cplusplus
extern "C" {
extern TExecuteResult *st_executecommand_3( TExecuteRequest * arg, CLIENT *cl );
}
#else
extern TExecuteResult *st_executecommand_3();
#endif /* __cplusplus */

#endif /* __HOSTEXECUTE_H__ */

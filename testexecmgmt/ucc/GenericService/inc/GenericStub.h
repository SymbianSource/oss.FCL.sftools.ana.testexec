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



#ifndef __GENERICSTUB_H__
#define __GENERICSTUB_H__
#include <rpc/types.h>
#include <rpc/rpc.h>

#define ERR_NONE                                 0

// errors
#define ERR_SERVICE_ALREADY_STARTED             -1
#define ERR_SERVICE_NOT_RUNNING                 -2
#define ERR_ACTIVE_USER_SESSIONS                -3
#define ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS    -4
#define ERR_FAILED_TO_CREATE_COMPONENT_MANAGER  -5
#define ERR_FAILED_TO_RETRIEVE_KEY              -6
#define ERR_INSTANCE_DOES_NOT_EXIST             -7
#define ERR_INVALID_METHOD                      -8
#define ERR_CANNOT_CREATE_NEW_INSTANCE          -9
#define ERR_STUB_ALREADY_CONNECTED              -10
#define ERR_FAILED_TO_CONNECT                   -11
#define ERR_STUB_NOT_CONNECTED                  -12
#define ERR_INVALID_RV_POINTER                  -13
#define ERR_RPC_ERROR                           -14
#define ERR_INVALID_SERVICE                     -15
#define ERR_INVALID_ERROR_CODE                  -16
#define ERR_MISSING_PARAMETER                   -17
#define ERR_INVALID_CALL                        -18

#define ERR_INVALIDARG                          -50
#define ERR_INVALIDSTATE                        -51
#define ERR_GENERAL                             -52


#define INFO_MAXIMUM_OBJECTS 256

#ifdef __cplusplus
extern "C" {
#endif
extern int StartUCCService( int anIID, int aVersion );
#ifdef __cplusplus
} // extern "C"
#endif

typedef struct {
	u_int TComponentList_len;
	int *TComponentList_val;
} TComponentList;
#ifdef __cplusplus
extern "C" {
bool_t xdr_GenericStub_TComponentList(...);
}
#else
bool_t xdr_GenericStub_TComponentList();
#endif

#define MAXPARAMNAMELENGTH 128
#define MAXPARAMVALUELENGTH 1024

struct TStartupInfo {
	int iEmpty;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_GenericStub_TStartupInfo(...);
}
#else
bool_t xdr_GenericStub_TStartupInfo();
#endif


struct TParam {
	char iName[MAXPARAMNAMELENGTH];
	char iValue[MAXPARAMVALUELENGTH];
};
typedef struct TParam TParam;
#ifdef __cplusplus
extern "C" {
bool_t xdr_GenericStub_TParam(...);
}
#else
bool_t xdr_GenericStub_TParam();
#endif


struct TCall {
	int iCallID;
	struct {
		u_int iParams_len;
		TParam *iParams_val;
	} iParams;
};
typedef struct TCall TCall;
#ifdef __cplusplus
extern "C" {
bool_t xdr_GenericStub_TCall(...);
}
#else
bool_t xdr_GenericStub_TCall();
#endif

#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_1( TStartupInfo * arg, CLIENT *cl );
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
#define LIST_CONNECTIONS ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_connections_1( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_connections_1();
#endif /* __cplusplus */
#define CSTR_STARTPROCESS ((u_long)31)
#ifdef __cplusplus
extern "C" {
extern int *cstr_startprocess_1( char ** arg, CLIENT *cl );
}
#else
extern int *cstr_startprocess_1();
#endif /* __cplusplus */
#define DSTR_REMOVEPROCESS ((u_long)32)
#ifdef __cplusplus
extern "C" {
extern int *dstr_removeprocess_1( int * arg, CLIENT *cl );
}
#else
extern int *dstr_removeprocess_1();
#endif /* __cplusplus */
#define RUN_COMMAND ((u_long)10)
#ifdef __cplusplus
extern "C" {
extern int *run_command_1( TCall * arg, CLIENT *cl );
}
#else
extern int *run_command_1();
#endif /* __cplusplus */

#endif /* __GENERICSTUB_H__ */

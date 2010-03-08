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




#ifndef __MNCONTROLLER_H__
#define __MNCONTROLLER_H__
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
bool_t xdr_mncontroller_TComponentList(...);
}
#else
bool_t xdr_mncontroller_TComponentList();
#endif

#define X_MAXINTERFACENAMELEN 64
#define X_MAXNAILEN 128
#define X_MAXOPTIONTOKENSIZE 512
#define X_MAXINFOLEN 128
#define X_MAXERRORSTRING 256
#define ERR_WAIT_PROCESS_ERROR -103
#define ERR_START_PROCESS_ERROR -105
#define ERR_STOP_PROCESS_ERROR -106
#define ERR_DYNAMICS_CALL_FAILED -107
#define ERR_CONFIG_FILE_ERROR -108
#define ERR_POLICY_OUTPUT_PARSING_FAILED -110
#define ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE -111
#define ERR_SET_INTERFACE_ADDRESS_ERROR -112
#define FP_EARLY_EXPIRE 1
#define FP_NEWEST_FA 2
#define FP_EAGER 4
#define FP_NEWEST_ADV 8

enum TMnTunnelingMode {
	TM_NO_TUNNEL = 0,
	TM_FULL_TUNNEL = 1,
	TM_TRIANGLE_TUNNEL = 2,
};
typedef enum TMnTunnelingMode TMnTunnelingMode;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TMnTunnelingMode(...);
}
#else
bool_t xdr_mncontroller_TMnTunnelingMode();
#endif


enum TSetInterfaceErrors {
	SIE_SOCKET_FAILED = 0,
	SIE_IOCTL_GET_DESTINATION_ADDRESS_FAILED = 1,
	SIE_IOCTL_SET_ADDRESS_FAILED = 2,
	SIE_IOCTL_SET_DESTINATION_ADDRESS_FAILED = 3,
};
typedef enum TSetInterfaceErrors TSetInterfaceErrors;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TSetInterfaceErrors(...);
}
#else
bool_t xdr_mncontroller_TSetInterfaceErrors();
#endif


struct TResult {
	int iServiceResult;
	int iSubComponentResult;
	int iData0;
	int iData1;
};
typedef struct TResult TResult;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TResult(...);
}
#else
bool_t xdr_mncontroller_TResult();
#endif


struct THomeAddressDesc {
	int iAgentID;
	int iHomeAddressPrefix;
	int iHomeAddress;
	int iHomeAgentAddress;
	char iInterfaceName[X_MAXINTERFACENAMELEN];
};
typedef struct THomeAddressDesc THomeAddressDesc;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_THomeAddressDesc(...);
}
#else
bool_t xdr_mncontroller_THomeAddressDesc();
#endif


struct TOptionDesc {
	int iAgentID;
	char iOptionToken[X_MAXOPTIONTOKENSIZE];
	char iOptionValue[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockStart[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockEnd[X_MAXOPTIONTOKENSIZE];
};
typedef struct TOptionDesc TOptionDesc;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TOptionDesc(...);
}
#else
bool_t xdr_mncontroller_TOptionDesc();
#endif


struct TTunnelingModeDesc {
	int iAgentID;
	TMnTunnelingMode iTunnelMode;
};
typedef struct TTunnelingModeDesc TTunnelingModeDesc;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TTunnelingModeDesc(...);
}
#else
bool_t xdr_mncontroller_TTunnelingModeDesc();
#endif


struct TPolicyRequest {
	int iAgentID;
	int iPolicy;
};
typedef struct TPolicyRequest TPolicyRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TPolicyRequest(...);
}
#else
bool_t xdr_mncontroller_TPolicyRequest();
#endif


struct TForeignAgentInfoRequest {
	int iAgentID;
	int iForeignAgentID;
};
typedef struct TForeignAgentInfoRequest TForeignAgentInfoRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TForeignAgentInfoRequest(...);
}
#else
bool_t xdr_mncontroller_TForeignAgentInfoRequest();
#endif


struct TTimeoutRequest {
	int iAgentID;
	int iTimeout;
};
typedef struct TTimeoutRequest TTimeoutRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TTimeoutRequest(...);
}
#else
bool_t xdr_mncontroller_TTimeoutRequest();
#endif


struct TMobileNodeStatus {
	TResult iCallResult;
	int iConnected;
	int iTunnelUp;
	u_int iLocalAddress;
	u_int iColocatedAddress;
	u_int iForeignAgentAddress;
	u_int iHomeAgentAddress;
	u_int iHomeAddress;
	int iLifetimeRemaining;
	int iTunnelMode;
	int iSecondsSinceLastRequest;
	int iSecondsSinceLastReply;
	int iReplyCode;
	char iInfoText[X_MAXINFOLEN];
	char iLastErrorString[X_MAXERRORSTRING];
};
typedef struct TMobileNodeStatus TMobileNodeStatus;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TMobileNodeStatus(...);
}
#else
bool_t xdr_mncontroller_TMobileNodeStatus();
#endif


struct TForeignAgentInfo {
	TResult iCallResult;
	u_int iForeignAgentAddress;
	char iInterfaceName[X_MAXINTERFACENAMELEN];
	int iPriority;
	int iInUse;
	int iInterfaceIndex;
	int iLastAdvertisement;
	char iNAI[X_MAXNAILEN];
	int iAdvertisementExpiry;
};
typedef struct TForeignAgentInfo TForeignAgentInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TForeignAgentInfo(...);
}
#else
bool_t xdr_mncontroller_TForeignAgentInfo();
#endif


typedef struct {
	u_int TForeignAgentList_len;
	TForeignAgentInfo *TForeignAgentList_val;
} TForeignAgentList;
#ifdef __cplusplus
extern "C" {
bool_t xdr_mncontroller_TForeignAgentList(...);
}
#else
bool_t xdr_mncontroller_TForeignAgentList();
#endif


#define MNCONTROLLER ((u_long)0x34630205)
#define MNCONTROLLER_VERSION ((u_long)5)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_5( void * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_5();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_5( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_5();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_5( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_5();
#endif /* __cplusplus */
#define CSTR_CREATEAGENT ((u_long)31)
#ifdef __cplusplus
extern "C" {
extern TResult *cstr_createagent_5( void * arg, CLIENT *cl );
}
#else
extern TResult *cstr_createagent_5();
#endif /* __cplusplus */
#define DSTR_REMOVEAGENT ((u_long)32)
#ifdef __cplusplus
extern "C" {
extern int *dstr_removeagent_5( int * arg, CLIENT *cl );
}
#else
extern int *dstr_removeagent_5();
#endif /* __cplusplus */
#define STARTMOBILEAGENT ((u_long)3)
#ifdef __cplusplus
extern "C" {
extern TResult *startmobileagent_5( int * arg, CLIENT *cl );
}
#else
extern TResult *startmobileagent_5();
#endif /* __cplusplus */
#define STOPMOBILEAGENT ((u_long)4)
#ifdef __cplusplus
extern "C" {
extern TResult *stopmobileagent_5( int * arg, CLIENT *cl );
}
#else
extern TResult *stopmobileagent_5();
#endif /* __cplusplus */
#define GETMOBILEAGENTSTATUS ((u_long)5)
#ifdef __cplusplus
extern "C" {
extern TResult *getmobileagentstatus_5( int * arg, CLIENT *cl );
}
#else
extern TResult *getmobileagentstatus_5();
#endif /* __cplusplus */
#define SETSINGLEOPTION ((u_long)6)
#ifdef __cplusplus
extern "C" {
extern TResult *setsingleoption_5( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *setsingleoption_5();
#endif /* __cplusplus */
#define REMOVESINGLEOPTION ((u_long)33)
#ifdef __cplusplus
extern "C" {
extern TResult *removesingleoption_5( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *removesingleoption_5();
#endif /* __cplusplus */
#define ADDLISTOPTION ((u_long)7)
#ifdef __cplusplus
extern "C" {
extern TResult *addlistoption_5( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *addlistoption_5();
#endif /* __cplusplus */
#define REMOVELISTOPTION ((u_long)8)
#ifdef __cplusplus
extern "C" {
extern TResult *removelistoption_5( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *removelistoption_5();
#endif /* __cplusplus */
#define SETHOMEADDRESS ((u_long)23)
#ifdef __cplusplus
extern "C" {
extern TResult *sethomeaddress_5( THomeAddressDesc * arg, CLIENT *cl );
}
#else
extern TResult *sethomeaddress_5();
#endif /* __cplusplus */
#define GETCAREOFADDRESS ((u_long)9)
#ifdef __cplusplus
extern "C" {
extern TResult *getcareofaddress_5( int * arg, CLIENT *cl );
}
#else
extern TResult *getcareofaddress_5();
#endif /* __cplusplus */
#define CONNECT ((u_long)12)
#ifdef __cplusplus
extern "C" {
extern TResult *connect_5( TTunnelingModeDesc * arg, CLIENT *cl );
}
#else
extern TResult *connect_5();
#endif /* __cplusplus */
#define DISCONNECT ((u_long)13)
#ifdef __cplusplus
extern "C" {
extern TResult *disconnect_5( int * arg, CLIENT *cl );
}
#else
extern TResult *disconnect_5();
#endif /* __cplusplus */
#define CONFIRM ((u_long)16)
#ifdef __cplusplus
extern "C" {
extern TResult *confirm_5( int * arg, CLIENT *cl );
}
#else
extern TResult *confirm_5();
#endif /* __cplusplus */
#define RESCANINTERFACES ((u_long)17)
#ifdef __cplusplus
extern "C" {
extern TResult *rescaninterfaces_5( int * arg, CLIENT *cl );
}
#else
extern TResult *rescaninterfaces_5();
#endif /* __cplusplus */
#define SETFAPOLICY ((u_long)18)
#ifdef __cplusplus
extern "C" {
extern TResult *setfapolicy_5( TPolicyRequest * arg, CLIENT *cl );
}
#else
extern TResult *setfapolicy_5();
#endif /* __cplusplus */
#define GETFAPOLICY ((u_long)19)
#ifdef __cplusplus
extern "C" {
extern TResult *getfapolicy_5( int * arg, CLIENT *cl );
}
#else
extern TResult *getfapolicy_5();
#endif /* __cplusplus */
#define GETSTATUS ((u_long)11)
#ifdef __cplusplus
extern "C" {
extern TMobileNodeStatus *getstatus_5( int * arg, CLIENT *cl );
}
#else
extern TMobileNodeStatus *getstatus_5();
#endif /* __cplusplus */
#define LISTFOREIGNAGENTS ((u_long)20)
#ifdef __cplusplus
extern "C" {
extern TForeignAgentList *listforeignagents_5( int * arg, CLIENT *cl );
}
#else
extern TForeignAgentList *listforeignagents_5();
#endif /* __cplusplus */
#define GETFOREIGNAGENTINFO ((u_long)21)
#ifdef __cplusplus
extern "C" {
extern TForeignAgentInfo *getforeignagentinfo_5( TForeignAgentInfoRequest * arg, CLIENT *cl );
}
#else
extern TForeignAgentInfo *getforeignagentinfo_5();
#endif /* __cplusplus */
#define SETTIMEOUT ((u_long)22)
#ifdef __cplusplus
extern "C" {
extern void *settimeout_5( TTimeoutRequest * arg, CLIENT *cl );
}
#else
extern void *settimeout_5();
#endif /* __cplusplus */

#endif /* __MNCONTROLLER_H__ */

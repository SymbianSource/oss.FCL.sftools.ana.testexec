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




#ifndef __FACONTROLLER_H__
#define __FACONTROLLER_H__
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
bool_t xdr_facontroller_TComponentList(...);
}
#else
bool_t xdr_facontroller_TComponentList();
#endif

#define X_MAXOPTIONTOKENSIZE 512
#define X_MAXINTERFACENAMESIZE 64
#define ERR_WAIT_PROCESS_ERROR -103
#define ERR_START_PROCESS_ERROR -105
#define ERR_STOP_PROCESS_ERROR -106
#define ERR_DYNAMICS_CALL_FAILED -107
#define ERR_CONFIG_FILE_ERROR -108
#define ERR_POLICY_OUTPUT_PARSING_FAILED -110
#define ERR_PROCESS_TERMINATED_OUTSIDE_SCOPE -111
#define ERR_NO_MORE_INTERFACES -112
#define ERR_CREATE_INTERFACE_ERROR -113
#define ERR_DESTROY_INTERFACE_ERROR -114
#define ERR_INTEGER_ALLOCATOR_SETUP_ERROR -115

enum TSoliticationMode {
	SM_SEND_NEVER = -1,
	SM_SEND_IN_RESPONSE_TO_SOLICITATION_ONLY = 0,
	SM_SEND_REGULAR = 1,
};
typedef enum TSoliticationMode TSoliticationMode;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TSoliticationMode(...);
}
#else
bool_t xdr_facontroller_TSoliticationMode();
#endif


struct TStartupInfo {
	int iBaseInterfaceIndex;
	int iNetworkMask;
	int iStartHostAddressRange;
	int iStopHostAddressRange;
};
typedef struct TStartupInfo TStartupInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TStartupInfo(...);
}
#else
bool_t xdr_facontroller_TStartupInfo();
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
bool_t xdr_facontroller_TResult(...);
}
#else
bool_t xdr_facontroller_TResult();
#endif


struct TOptionDesc {
	int iAgentID;
	char iOptionFilename[X_MAXOPTIONTOKENSIZE];
	char iOptionToken[X_MAXOPTIONTOKENSIZE];
	char iOptionValue[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockStart[X_MAXOPTIONTOKENSIZE];
	char iOptionBlockEnd[X_MAXOPTIONTOKENSIZE];
};
typedef struct TOptionDesc TOptionDesc;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TOptionDesc(...);
}
#else
bool_t xdr_facontroller_TOptionDesc();
#endif


struct TTimeoutRequest {
	int iAgentID;
	int iTimeout;
};
typedef struct TTimeoutRequest TTimeoutRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TTimeoutRequest(...);
}
#else
bool_t xdr_facontroller_TTimeoutRequest();
#endif


struct TGetTunnelRequest {
	int iAgentID;
	int iMobileNodeAddress;
};
typedef struct TGetTunnelRequest TGetTunnelRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TGetTunnelRequest(...);
}
#else
bool_t xdr_facontroller_TGetTunnelRequest();
#endif


struct TStartAgentRequest {
	int iAgentID;
	char iLowerInterface[X_MAXINTERFACENAMESIZE];
	TSoliticationMode iSolicitationMode;
	int iSolicitationInterval;
};
typedef struct TStartAgentRequest TStartAgentRequest;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TStartAgentRequest(...);
}
#else
bool_t xdr_facontroller_TStartAgentRequest();
#endif


struct TFaStatusInfo {
	TResult iCallResult;
	int iTunnelCount;
	int iPendingRegistrationRequests;
	int iRequestsRejected;
	int iRequestsAccepted;
	int iReplysAccepted;
	int iReplysRejected;
	int iDiscardedUnknownExtension;
	int iDiscardedMalformed;
	int iDiscardedVendor;
	int iAdvertisementsSent;
};
typedef struct TFaStatusInfo TFaStatusInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TFaStatusInfo(...);
}
#else
bool_t xdr_facontroller_TFaStatusInfo();
#endif


struct TFaTunnelID {
	int iAgentID;
	int iMobileNodeAddress;
	int iHomeAgentAddress;
	int iID;
};
typedef struct TFaTunnelID TFaTunnelID;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TFaTunnelID(...);
}
#else
bool_t xdr_facontroller_TFaTunnelID();
#endif


struct TFaTunnelInfo {
	TResult iCallResult;
	u_int iMobileNodeAddress;
	u_int iCareofAddress;
	u_int iHomeAgentAddress;
	int iPrivateHomeAgentID;
	int iCreationTime;
	int iExpirationTime;
	int iRefreshTime;
	int iLastTimestamp;
	int iSPI;
	int iTimeout;
};
typedef struct TFaTunnelInfo TFaTunnelInfo;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TFaTunnelInfo(...);
}
#else
bool_t xdr_facontroller_TFaTunnelInfo();
#endif


typedef struct {
	u_int TFaTunnelList_len;
	TFaTunnelID *TFaTunnelList_val;
} TFaTunnelList;
#ifdef __cplusplus
extern "C" {
bool_t xdr_facontroller_TFaTunnelList(...);
}
#else
bool_t xdr_facontroller_TFaTunnelList();
#endif


#define FACONTROLLER ((u_long)0x34630207)
#define FACONTROLLER_VERSION ((u_long)7)
#define SS_STARTUPRPCSERVICE ((u_long)1)
#ifdef __cplusplus
extern "C" {
extern int *ss_startuprpcservice_7( TStartupInfo * arg, CLIENT *cl );
}
#else
extern int *ss_startuprpcservice_7();
#endif /* __cplusplus */
#define SC_SHUTDOWNRPCSERVICE ((u_long)2)
#ifdef __cplusplus
extern "C" {
extern int *sc_shutdownrpcservice_7( int * arg, CLIENT *cl );
}
#else
extern int *sc_shutdownrpcservice_7();
#endif /* __cplusplus */
#define LIST_DEVICES ((u_long)30)
#ifdef __cplusplus
extern "C" {
extern TComponentList *list_devices_7( void * arg, CLIENT *cl );
}
#else
extern TComponentList *list_devices_7();
#endif /* __cplusplus */
#define CSTR_CREATEAGENT ((u_long)31)
#ifdef __cplusplus
extern "C" {
extern TResult *cstr_createagent_7( void * arg, CLIENT *cl );
}
#else
extern TResult *cstr_createagent_7();
#endif /* __cplusplus */
#define DSTR_REMOVEAGENT ((u_long)32)
#ifdef __cplusplus
extern "C" {
extern int *dstr_removeagent_7( int * arg, CLIENT *cl );
}
#else
extern int *dstr_removeagent_7();
#endif /* __cplusplus */
#define STARTMOBILEAGENT ((u_long)3)
#ifdef __cplusplus
extern "C" {
extern TResult *startmobileagent_7( TStartAgentRequest * arg, CLIENT *cl );
}
#else
extern TResult *startmobileagent_7();
#endif /* __cplusplus */
#define STOPMOBILEAGENT ((u_long)4)
#ifdef __cplusplus
extern "C" {
extern TResult *stopmobileagent_7( int * arg, CLIENT *cl );
}
#else
extern TResult *stopmobileagent_7();
#endif /* __cplusplus */
#define GETMOBILEAGENTSTATUS ((u_long)5)
#ifdef __cplusplus
extern "C" {
extern TResult *getmobileagentstatus_7( int * arg, CLIENT *cl );
}
#else
extern TResult *getmobileagentstatus_7();
#endif /* __cplusplus */
#define SETSINGLEOPTION ((u_long)6)
#ifdef __cplusplus
extern "C" {
extern TResult *setsingleoption_7( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *setsingleoption_7();
#endif /* __cplusplus */
#define REMOVESINGLEOPTION ((u_long)33)
#ifdef __cplusplus
extern "C" {
extern TResult *removesingleoption_7( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *removesingleoption_7();
#endif /* __cplusplus */
#define ADDLISTOPTION ((u_long)7)
#ifdef __cplusplus
extern "C" {
extern TResult *addlistoption_7( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *addlistoption_7();
#endif /* __cplusplus */
#define REMOVELISTOPTION ((u_long)8)
#ifdef __cplusplus
extern "C" {
extern TResult *removelistoption_7( TOptionDesc * arg, CLIENT *cl );
}
#else
extern TResult *removelistoption_7();
#endif /* __cplusplus */
#define GETSTATUS ((u_long)9)
#ifdef __cplusplus
extern "C" {
extern TFaStatusInfo *getstatus_7( int * arg, CLIENT *cl );
}
#else
extern TFaStatusInfo *getstatus_7();
#endif /* __cplusplus */
#define DESTROYTUNNELID ((u_long)11)
#ifdef __cplusplus
extern "C" {
extern TResult *destroytunnelid_7( TFaTunnelID * arg, CLIENT *cl );
}
#else
extern TResult *destroytunnelid_7();
#endif /* __cplusplus */
#define LISTTUNNELS ((u_long)12)
#ifdef __cplusplus
extern "C" {
extern TFaTunnelList *listtunnels_7( int * arg, CLIENT *cl );
}
#else
extern TFaTunnelList *listtunnels_7();
#endif /* __cplusplus */
#define GETTUNNELINFO ((u_long)15)
#ifdef __cplusplus
extern "C" {
extern TFaTunnelInfo *gettunnelinfo_7( TGetTunnelRequest * arg, CLIENT *cl );
}
#else
extern TFaTunnelInfo *gettunnelinfo_7();
#endif /* __cplusplus */
#define SETTIMEOUT ((u_long)13)
#ifdef __cplusplus
extern "C" {
extern void *settimeout_7( TTimeoutRequest * arg, CLIENT *cl );
}
#else
extern void *settimeout_7();
#endif /* __cplusplus */

#endif /* __FACONTROLLER_H__ */

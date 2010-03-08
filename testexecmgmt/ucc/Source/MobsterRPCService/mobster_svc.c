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




#include <stdio.h>
#include <rpc/rpc.h>
#ifdef WIN32
#include <rpc/PMAP_CLN.H>
#else
#include <rpc/pmap_clnt.h>
#endif
#include "mobster.h"

static void mobster_1();

int main( void )
{
	SVCXPRT *transp;

#ifdef WIN32
	rpc_nt_init();
#endif

	(void)pmap_unset(MOBSTER, MOBSTER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
#ifdef WIN32
		rpc_nt_exit();
#endif
		exit(1);
	}
	if (!svc_register(transp, MOBSTER, MOBSTER_VERSION, mobster_1, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (MOBSTER, MOBSTER_VERSION, udp).\n");
#ifdef WIN32
		rpc_nt_exit();
#endif
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
#ifdef WIN32
		rpc_nt_exit();
#endif
		exit(1);
	}
	if (!svc_register(transp, MOBSTER, MOBSTER_VERSION, mobster_1, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (MOBSTER, MOBSTER_VERSION, tcp).\n");
#ifdef WIN32
		rpc_nt_exit();
#endif
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
#ifdef WIN32
	rpc_nt_exit();
#endif
	exit(1);
}

#ifdef MULTITHREAD
struct call_params {
	struct svc_req *rqstp;
	SVCXPRT *transp;
	void *arg;
	char *(*local)();
	bool_t (*xdr_argument)(), (*xdr_result)();
};
static void mobster_1_a ();
#endif

static void
mobster_1(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		struct TChannelAddress ss_startuprpcservice_1_arg;
		int sc_shutdownrpcservice_1_arg;
		TDeviceDesc cstr_startdevice_1_arg;
		int dstr_removedevice_1_arg;
		int getdeviceinfo_1_arg;
		int getdevicelog_1_arg;
		int stopdevice_1_arg;
		struct TUUAddress setremoteuuaddress_1_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();


#ifdef MULTITHREAD
	DWORD TID = 0;
	HANDLE threadHandle = NULL;
	struct call_params	*params;
#endif
	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_mobster_TChannelAddress;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_1;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_1;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_mobster_TComponentList;
		local = (char *(*)()) list_devices_1;
		break;

	case CSTR_STARTDEVICE:
		xdr_argument = xdr_mobster_TDeviceDesc;
		xdr_result = xdr_int;
		local = (char *(*)()) cstr_startdevice_1;
		break;

	case DSTR_REMOVEDEVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dstr_removedevice_1;
		break;

	case GETDEVICEINFO:
		xdr_argument = xdr_int;
		xdr_result = xdr_mobster_TDeviceDesc;
		local = (char *(*)()) getdeviceinfo_1;
		break;

	case GETDEVICELOG:
		xdr_argument = xdr_int;
		xdr_result = xdr_mobster_TVarData;
		local = (char *(*)()) getdevicelog_1;
		break;

	case STOPDEVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) stopdevice_1;
		break;

	case SETREMOTEUUADDRESS:
		xdr_argument = xdr_mobster_TUUAddress;
		xdr_result = xdr_int;
		local = (char *(*)()) setremoteuuaddress_1;
		break;

	default:
		svcerr_noproc(transp);
		return;
	}
	bzero((char *)&argument, sizeof(argument));
	if (!svc_getargs(transp, (void*)xdr_argument, (void*)&argument)) {
		svcerr_decode(transp);
		return;
	}

#ifdef MULTITHREAD
	params = (struct call_params*) malloc (sizeof(struct call_params));

	params->rqstp = (struct svc_req*) malloc (sizeof(struct svc_req));
	bcopy(rqstp, params->rqstp, sizeof(struct svc_req));

	params->transp = transp;

	params->arg = malloc (sizeof(argument));
	bcopy(&argument, params->arg, sizeof(argument));

	params->local = local;

	params->xdr_argument = xdr_argument;
	params->xdr_result = xdr_result;

	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mobster_1_a, params, 0, &TID);
	if (!threadHandle)
		mobster_1_a(&params);
}

static void
mobster_1_a(params)
struct call_params *params;
{
	struct svc_req *rqstp;
	SVCXPRT *transp;
	void *argument;
	void (*destroy_proc)();

	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();


	argument = params->arg;
	rqstp = params->rqstp;
	transp = params->transp;
	xdr_argument = params->xdr_argument;
	xdr_result = params->xdr_result;
	local = params->local;
	destroy_proc = transp->xp_ops->xp_destroy;
	transp->xp_ops->xp_destroy = xprt_unregister;
#endif

	result = (*local)(&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (void*)xdr_result, result)) {
		svcerr_systemerr(transp);
	}
#ifdef MULTITHREAD
	free(params->rqstp);
	free(params);
#endif
	if (!svc_freeargs(transp, (void*)xdr_argument, (void*)&argument)) {
		(void)fprintf(stderr, "unable to free arguments\n");
#ifdef WIN32
		rpc_nt_exit();
#endif
		exit(1);
	}
#ifdef MULTITHREAD
	free(argument);
	transp->xp_ops->xp_destroy = destroy_proc;
#endif
}


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
#include "GenericStub.h"

static void genericstub_1();

int StartUCCService( int anIID, int aVersion )
{
	SVCXPRT *transp;

#ifdef WIN32
	rpc_nt_init();
#endif

	(void)pmap_unset(anIID, aVersion);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
#ifdef WIN32
		rpc_nt_exit();
#endif
		exit(1);
	}
	if (!svc_register(transp, anIID, aVersion, genericstub_1, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (GENERICSTUB, GENERICSTUB_VERSION, udp).\n");
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
	if (!svc_register(transp, anIID, aVersion, genericstub_1, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (GENERICSTUB, GENERICSTUB_VERSION, tcp).\n");
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
static void genericstub_1_a ();
#endif

static void
genericstub_1(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_1_arg;
		int sc_shutdownrpcservice_1_arg;
		char *cstr_startprocess_1_arg;
		int dstr_removeprocess_1_arg;
		TCall run_command_1_arg;
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
		xdr_argument = xdr_GenericStub_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_1;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_1;
		break;

	case LIST_CONNECTIONS:
		xdr_argument = xdr_void;
		xdr_result = xdr_GenericStub_TComponentList;
		local = (char *(*)()) list_connections_1;
		break;

	case CSTR_STARTPROCESS:
		xdr_argument = xdr_wrapstring;
		xdr_result = xdr_int;
		local = (char *(*)()) cstr_startprocess_1;
		break;

	case DSTR_REMOVEPROCESS:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dstr_removeprocess_1;
		break;

	case RUN_COMMAND:
		xdr_argument = xdr_GenericStub_TCall;
		xdr_result = xdr_int;
		local = (char *(*)()) run_command_1;
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

	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)genericstub_1_a, params, 0, &TID);
	if (!threadHandle)
		genericstub_1_a(&params);
}

static void
genericstub_1_a(params)
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


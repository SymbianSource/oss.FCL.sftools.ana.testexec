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
#include "hostexecuteasync.h"

static void hostexecuteasync_9();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(HOSTEXECUTEASYNC, HOSTEXECUTEASYNC_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, HOSTEXECUTEASYNC, HOSTEXECUTEASYNC_VERSION, hostexecuteasync_9, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (HOSTEXECUTEASYNC, HOSTEXECUTEASYNC_VERSION, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, HOSTEXECUTEASYNC, HOSTEXECUTEASYNC_VERSION, hostexecuteasync_9, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (HOSTEXECUTEASYNC, HOSTEXECUTEASYNC_VERSION, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
hostexecuteasync_9(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_9_arg;
		int sc_shutdownrpcservice_9_arg;
		char *cstr_startprocess_9_arg;
		int dstr_removeprocess_9_arg;
		int killprocess_9_arg;
		int stopprocess_9_arg;
		int getprocessinfo_9_arg;
		int getstandardoutput_9_arg;
		int getstandarderror_9_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_hostexecuteasync_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_9;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_9;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_hostexecuteasync_TComponentList;
		local = (char *(*)()) list_devices_9;
		break;

	case CSTR_STARTPROCESS:
		xdr_argument = xdr_wrapstring;
		xdr_result = xdr_hostexecuteasync_TResult;
		local = (char *(*)()) cstr_startprocess_9;
		break;

	case DSTR_REMOVEPROCESS:
		xdr_argument = xdr_int;
		xdr_result = xdr_hostexecuteasync_TResult;
		local = (char *(*)()) dstr_removeprocess_9;
		break;

	case KILLPROCESS:
		xdr_argument = xdr_int;
		xdr_result = xdr_hostexecuteasync_TResult;
		local = (char *(*)()) killprocess_9;
		break;

	case STOPPROCESS:
		xdr_argument = xdr_int;
		xdr_result = xdr_hostexecuteasync_TResult;
		local = (char *(*)()) stopprocess_9;
		break;

	case GETPROCESSINFO:
		xdr_argument = xdr_int;
		xdr_result = xdr_hostexecuteasync_THostExecuteAsyncProcessInfo;
		local = (char *(*)()) getprocessinfo_9;
		break;

	case GETSTANDARDOUTPUT:
		xdr_argument = xdr_int;
		xdr_result = xdr_hostexecuteasync_TVarData;
		local = (char *(*)()) getstandardoutput_9;
		break;

	case GETSTANDARDERROR:
		xdr_argument = xdr_int;
		xdr_result = xdr_hostexecuteasync_TVarData;
		local = (char *(*)()) getstandarderror_9;
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
	result = (*local)(&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (void*)xdr_result, result)) {
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, (void*)xdr_argument, (void*)&argument)) {
		(void)fprintf(stderr, "unable to free arguments\n");
		exit(1);
	}
}


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
#include "pppcontroller.h"

static void pppcontroller_4();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(PPPCONTROLLER, PPPCONTROLLER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, PPPCONTROLLER, PPPCONTROLLER_VERSION, pppcontroller_4, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (PPPCONTROLLER, PPPCONTROLLER_VERSION, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, PPPCONTROLLER, PPPCONTROLLER_VERSION, pppcontroller_4, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (PPPCONTROLLER, PPPCONTROLLER_VERSION, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
pppcontroller_4(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_4_arg;
		int sc_shutdownrpcservice_4_arg;
		TPppSessionConfig cstr_startpppsession_4_arg;
		int dstr_removepppsession_4_arg;
		int killsession_4_arg;
		int stopsession_4_arg;
		int getsessioninfo_4_arg;
		int getppplog_4_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_pppcontroller_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_4;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_4;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_pppcontroller_TComponentList;
		local = (char *(*)()) list_devices_4;
		break;

	case CSTR_STARTPPPSESSION:
		xdr_argument = xdr_pppcontroller_TPppSessionConfig;
		xdr_result = xdr_pppcontroller_TResult;
		local = (char *(*)()) cstr_startpppsession_4;
		break;

	case DSTR_REMOVEPPPSESSION:
		xdr_argument = xdr_int;
		xdr_result = xdr_pppcontroller_TResult;
		local = (char *(*)()) dstr_removepppsession_4;
		break;

	case KILLSESSION:
		xdr_argument = xdr_int;
		xdr_result = xdr_pppcontroller_TResult;
		local = (char *(*)()) killsession_4;
		break;

	case STOPSESSION:
		xdr_argument = xdr_int;
		xdr_result = xdr_pppcontroller_TResult;
		local = (char *(*)()) stopsession_4;
		break;

	case GETSESSIONINFO:
		xdr_argument = xdr_int;
		xdr_result = xdr_pppcontroller_TPppSessionDesc;
		local = (char *(*)()) getsessioninfo_4;
		break;

	case GETPPPLOG:
		xdr_argument = xdr_int;
		xdr_result = xdr_pppcontroller_TVarData;
		local = (char *(*)()) getppplog_4;
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


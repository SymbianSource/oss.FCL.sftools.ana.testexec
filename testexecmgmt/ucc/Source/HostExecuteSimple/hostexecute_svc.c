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
#include "hostexecute.h"

static void hostexecute_3();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(HOSTEXECUTE, HOSTEXECUTE_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, HOSTEXECUTE, HOSTEXECUTE_VERSION, hostexecute_3, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (HOSTEXECUTE, HOSTEXECUTE_VERSION, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, HOSTEXECUTE, HOSTEXECUTE_VERSION, hostexecute_3, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (HOSTEXECUTE, HOSTEXECUTE_VERSION, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
hostexecute_3(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_3_arg;
		int sc_shutdownrpcservice_3_arg;
		TExecuteRequest st_executecommand_3_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_hostexecute_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_3;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_3;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_hostexecute_TComponentList;
		local = (char *(*)()) list_devices_3;
		break;

	case ST_EXECUTECOMMAND:
		xdr_argument = xdr_hostexecute_TExecuteRequest;
		xdr_result = xdr_hostexecute_TExecuteResult;
		local = (char *(*)()) st_executecommand_3;
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


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
#include "hacontroller.h"

static void hacontroller_8();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(HACONTROLLER, HACONTROLLER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, HACONTROLLER, HACONTROLLER_VERSION, hacontroller_8, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (HACONTROLLER, HACONTROLLER_VERSION, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, HACONTROLLER, HACONTROLLER_VERSION, hacontroller_8, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (HACONTROLLER, HACONTROLLER_VERSION, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
hacontroller_8(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_8_arg;
		int sc_shutdownrpcservice_8_arg;
		int dstr_removeagent_8_arg;
		int startmobileagent_8_arg;
		int stopmobileagent_8_arg;
		int getmobileagentstatus_8_arg;
		TOptionDesc setsingleoption_8_arg;
		TOptionDesc removesingleoption_8_arg;
		TOptionDesc addlistoption_8_arg;
		TOptionDesc removelistoption_8_arg;
		int getstatus_8_arg;
		THaTunnelID destroytunnelid_8_arg;
		int listtunnels_8_arg;
		TGetTunnelRequest gettunnelinfo_8_arg;
		TTimeoutRequest settimeout_8_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_hacontroller_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_8;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_8;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_hacontroller_TComponentList;
		local = (char *(*)()) list_devices_8;
		break;

	case CSTR_CREATEAGENT:
		xdr_argument = xdr_void;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) cstr_createagent_8;
		break;

	case DSTR_REMOVEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) dstr_removeagent_8;
		break;

	case STARTMOBILEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) startmobileagent_8;
		break;

	case STOPMOBILEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) stopmobileagent_8;
		break;

	case GETMOBILEAGENTSTATUS:
		xdr_argument = xdr_int;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) getmobileagentstatus_8;
		break;

	case SETSINGLEOPTION:
		xdr_argument = xdr_hacontroller_TOptionDesc;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) setsingleoption_8;
		break;

	case REMOVESINGLEOPTION:
		xdr_argument = xdr_hacontroller_TOptionDesc;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) removesingleoption_8;
		break;

	case ADDLISTOPTION:
		xdr_argument = xdr_hacontroller_TOptionDesc;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) addlistoption_8;
		break;

	case REMOVELISTOPTION:
		xdr_argument = xdr_hacontroller_TOptionDesc;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) removelistoption_8;
		break;

	case GETSTATUS:
		xdr_argument = xdr_int;
		xdr_result = xdr_hacontroller_THaStatus;
		local = (char *(*)()) getstatus_8;
		break;

	case DESTROYTUNNELID:
		xdr_argument = xdr_hacontroller_THaTunnelID;
		xdr_result = xdr_hacontroller_TResult;
		local = (char *(*)()) destroytunnelid_8;
		break;

	case LISTTUNNELS:
		xdr_argument = xdr_int;
		xdr_result = xdr_hacontroller_THaTunnelList;
		local = (char *(*)()) listtunnels_8;
		break;

	case GETTUNNELINFO:
		xdr_argument = xdr_hacontroller_TGetTunnelRequest;
		xdr_result = xdr_hacontroller_THaTunnelInfo;
		local = (char *(*)()) gettunnelinfo_8;
		break;

	case SETTIMEOUT:
		xdr_argument = xdr_hacontroller_TTimeoutRequest;
		xdr_result = xdr_void;
		local = (char *(*)()) settimeout_8;
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


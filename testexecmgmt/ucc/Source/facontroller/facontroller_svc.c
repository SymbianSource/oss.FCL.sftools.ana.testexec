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
#include "facontroller.h"

static void facontroller_7();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(FACONTROLLER, FACONTROLLER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, FACONTROLLER, FACONTROLLER_VERSION, facontroller_7, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (FACONTROLLER, FACONTROLLER_VERSION, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, FACONTROLLER, FACONTROLLER_VERSION, facontroller_7, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (FACONTROLLER, FACONTROLLER_VERSION, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
facontroller_7(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_7_arg;
		int sc_shutdownrpcservice_7_arg;
		int dstr_removeagent_7_arg;
		TStartAgentRequest startmobileagent_7_arg;
		int stopmobileagent_7_arg;
		int getmobileagentstatus_7_arg;
		TOptionDesc setsingleoption_7_arg;
		TOptionDesc removesingleoption_7_arg;
		TOptionDesc addlistoption_7_arg;
		TOptionDesc removelistoption_7_arg;
		int getstatus_7_arg;
		TFaTunnelID destroytunnelid_7_arg;
		int listtunnels_7_arg;
		TGetTunnelRequest gettunnelinfo_7_arg;
		TTimeoutRequest settimeout_7_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_facontroller_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_7;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_7;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_facontroller_TComponentList;
		local = (char *(*)()) list_devices_7;
		break;

	case CSTR_CREATEAGENT:
		xdr_argument = xdr_void;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) cstr_createagent_7;
		break;

	case DSTR_REMOVEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dstr_removeagent_7;
		break;

	case STARTMOBILEAGENT:
		xdr_argument = xdr_facontroller_TStartAgentRequest;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) startmobileagent_7;
		break;

	case STOPMOBILEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) stopmobileagent_7;
		break;

	case GETMOBILEAGENTSTATUS:
		xdr_argument = xdr_int;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) getmobileagentstatus_7;
		break;

	case SETSINGLEOPTION:
		xdr_argument = xdr_facontroller_TOptionDesc;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) setsingleoption_7;
		break;

	case REMOVESINGLEOPTION:
		xdr_argument = xdr_facontroller_TOptionDesc;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) removesingleoption_7;
		break;

	case ADDLISTOPTION:
		xdr_argument = xdr_facontroller_TOptionDesc;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) addlistoption_7;
		break;

	case REMOVELISTOPTION:
		xdr_argument = xdr_facontroller_TOptionDesc;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) removelistoption_7;
		break;

	case GETSTATUS:
		xdr_argument = xdr_int;
		xdr_result = xdr_facontroller_TFaStatusInfo;
		local = (char *(*)()) getstatus_7;
		break;

	case DESTROYTUNNELID:
		xdr_argument = xdr_facontroller_TFaTunnelID;
		xdr_result = xdr_facontroller_TResult;
		local = (char *(*)()) destroytunnelid_7;
		break;

	case LISTTUNNELS:
		xdr_argument = xdr_int;
		xdr_result = xdr_facontroller_TFaTunnelList;
		local = (char *(*)()) listtunnels_7;
		break;

	case GETTUNNELINFO:
		xdr_argument = xdr_facontroller_TGetTunnelRequest;
		xdr_result = xdr_facontroller_TFaTunnelInfo;
		local = (char *(*)()) gettunnelinfo_7;
		break;

	case SETTIMEOUT:
		xdr_argument = xdr_facontroller_TTimeoutRequest;
		xdr_result = xdr_void;
		local = (char *(*)()) settimeout_7;
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


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
#include "mncontroller.h"

static void mncontroller_5();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(MNCONTROLLER, MNCONTROLLER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, MNCONTROLLER, MNCONTROLLER_VERSION, mncontroller_5, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (MNCONTROLLER, MNCONTROLLER_VERSION, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, MNCONTROLLER, MNCONTROLLER_VERSION, mncontroller_5, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (MNCONTROLLER, MNCONTROLLER_VERSION, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
mncontroller_5(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		int sc_shutdownrpcservice_5_arg;
		int dstr_removeagent_5_arg;
		int startmobileagent_5_arg;
		int stopmobileagent_5_arg;
		int getmobileagentstatus_5_arg;
		TOptionDesc setsingleoption_5_arg;
		TOptionDesc removesingleoption_5_arg;
		TOptionDesc addlistoption_5_arg;
		TOptionDesc removelistoption_5_arg;
		THomeAddressDesc sethomeaddress_5_arg;
		int getcareofaddress_5_arg;
		TTunnelingModeDesc connect_5_arg;
		int disconnect_5_arg;
		int confirm_5_arg;
		int rescaninterfaces_5_arg;
		TPolicyRequest setfapolicy_5_arg;
		int getfapolicy_5_arg;
		int getstatus_5_arg;
		int listforeignagents_5_arg;
		TForeignAgentInfoRequest getforeignagentinfo_5_arg;
		TTimeoutRequest settimeout_5_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_void;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_5;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_5;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_mncontroller_TComponentList;
		local = (char *(*)()) list_devices_5;
		break;

	case CSTR_CREATEAGENT:
		xdr_argument = xdr_void;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) cstr_createagent_5;
		break;

	case DSTR_REMOVEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dstr_removeagent_5;
		break;

	case STARTMOBILEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) startmobileagent_5;
		break;

	case STOPMOBILEAGENT:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) stopmobileagent_5;
		break;

	case GETMOBILEAGENTSTATUS:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) getmobileagentstatus_5;
		break;

	case SETSINGLEOPTION:
		xdr_argument = xdr_mncontroller_TOptionDesc;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) setsingleoption_5;
		break;

	case REMOVESINGLEOPTION:
		xdr_argument = xdr_mncontroller_TOptionDesc;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) removesingleoption_5;
		break;

	case ADDLISTOPTION:
		xdr_argument = xdr_mncontroller_TOptionDesc;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) addlistoption_5;
		break;

	case REMOVELISTOPTION:
		xdr_argument = xdr_mncontroller_TOptionDesc;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) removelistoption_5;
		break;

	case SETHOMEADDRESS:
		xdr_argument = xdr_mncontroller_THomeAddressDesc;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) sethomeaddress_5;
		break;

	case GETCAREOFADDRESS:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) getcareofaddress_5;
		break;

	case CONNECT:
		xdr_argument = xdr_mncontroller_TTunnelingModeDesc;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) connect_5;
		break;

	case DISCONNECT:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) disconnect_5;
		break;

	case CONFIRM:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) confirm_5;
		break;

	case RESCANINTERFACES:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) rescaninterfaces_5;
		break;

	case SETFAPOLICY:
		xdr_argument = xdr_mncontroller_TPolicyRequest;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) setfapolicy_5;
		break;

	case GETFAPOLICY:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TResult;
		local = (char *(*)()) getfapolicy_5;
		break;

	case GETSTATUS:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TMobileNodeStatus;
		local = (char *(*)()) getstatus_5;
		break;

	case LISTFOREIGNAGENTS:
		xdr_argument = xdr_int;
		xdr_result = xdr_mncontroller_TForeignAgentList;
		local = (char *(*)()) listforeignagents_5;
		break;

	case GETFOREIGNAGENTINFO:
		xdr_argument = xdr_mncontroller_TForeignAgentInfoRequest;
		xdr_result = xdr_mncontroller_TForeignAgentInfo;
		local = (char *(*)()) getforeignagentinfo_5;
		break;

	case SETTIMEOUT:
		xdr_argument = xdr_mncontroller_TTimeoutRequest;
		xdr_result = xdr_void;
		local = (char *(*)()) settimeout_5;
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


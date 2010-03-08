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
#include "uuinterface.h"

static void uuinterface_2();

int main( void )
{
	SVCXPRT *transp;

	(void)pmap_unset(UUINTERFACE, UUINTERFACEVER);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, UUINTERFACE, UUINTERFACEVER, uuinterface_2, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (UUINTERFACE, UUINTERFACEVER, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, UUINTERFACE, UUINTERFACEVER, uuinterface_2, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (UUINTERFACE, UUINTERFACEVER, tcp).\n");
		exit(1);
	}
	svc_run();
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
uuinterface_2(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		TStartupInfo ss_startuprpcservice_2_arg;
		int sc_shutdownrpcservice_2_arg;
		TConfigValue st_setdelay_2_arg;
		TConfigValue st_setdroppercentage_2_arg;
		TConfigValue st_setduplicatepercentage_2_arg;
		TConfigValue st_setbandwidth_2_arg;
		TConfigValue st_setdrd_2_arg;
		TConfigValue st_clearsettings_2_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, (void*)xdr_void, (char *)NULL);
		return;

	case SS_STARTUPRPCSERVICE:
		xdr_argument = xdr_uuinterface_TStartupInfo;
		xdr_result = xdr_int;
		local = (char *(*)()) ss_startuprpcservice_2;
		break;

	case SC_SHUTDOWNRPCSERVICE:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) sc_shutdownrpcservice_2;
		break;

	case LIST_DEVICES:
		xdr_argument = xdr_void;
		xdr_result = xdr_uuinterface_TComponentList;
		local = (char *(*)()) list_devices_2;
		break;

	case ST_SETDELAY:
		xdr_argument = xdr_uuinterface_TConfigValue;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_setdelay_2;
		break;

	case ST_SETDROPPERCENTAGE:
		xdr_argument = xdr_uuinterface_TConfigValue;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_setdroppercentage_2;
		break;

	case ST_SETDUPLICATEPERCENTAGE:
		xdr_argument = xdr_uuinterface_TConfigValue;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_setduplicatepercentage_2;
		break;

	case ST_SETBANDWIDTH:
		xdr_argument = xdr_uuinterface_TConfigValue;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_setbandwidth_2;
		break;

	case ST_SETDRD:
		xdr_argument = xdr_uuinterface_TConfigValue;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_setdrd_2;
		break;

	case ST_CLEARSETTINGS:
		xdr_argument = xdr_uuinterface_TConfigValue;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_clearsettings_2;
		break;

	case ST_STOPSERVICE:
		xdr_argument = xdr_void;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_stopservice_2;
		break;

	case ST_RESET:
		xdr_argument = xdr_void;
		xdr_result = xdr_uuinterface_TResult;
		local = (char *(*)()) st_reset_2;
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


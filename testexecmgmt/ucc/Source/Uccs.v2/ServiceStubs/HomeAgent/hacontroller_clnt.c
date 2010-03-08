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




#include <rpc/rpc.h>
#include "hacontroller.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_8(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_hacontroller_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_8(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_hacontroller_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
cstr_createagent_8(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_CREATEAGENT, xdr_void, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
dstr_removeagent_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEAGENT, xdr_int, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
startmobileagent_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STARTMOBILEAGENT, xdr_int, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
stopmobileagent_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPMOBILEAGENT, xdr_int, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
getmobileagentstatus_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETMOBILEAGENTSTATUS, xdr_int, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
setsingleoption_8(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETSINGLEOPTION, xdr_hacontroller_TOptionDesc, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
removesingleoption_8(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, REMOVESINGLEOPTION, xdr_hacontroller_TOptionDesc, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
addlistoption_8(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ADDLISTOPTION, xdr_hacontroller_TOptionDesc, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
removelistoption_8(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, REMOVELISTOPTION, xdr_hacontroller_TOptionDesc, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


THaStatus *
getstatus_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static THaStatus res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETSTATUS, xdr_int, argp, xdr_hacontroller_THaStatus, &res, TIMEOUT);
	return (&res);
}


TResult *
destroytunnelid_8(argp, clnt)
	THaTunnelID *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DESTROYTUNNELID, xdr_hacontroller_THaTunnelID, argp, xdr_hacontroller_TResult, &res, TIMEOUT);
	return (&res);
}


THaTunnelList *
listtunnels_8(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static THaTunnelList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LISTTUNNELS, xdr_int, argp, xdr_hacontroller_THaTunnelList, &res, TIMEOUT);
	return (&res);
}


THaTunnelInfo *
gettunnelinfo_8(argp, clnt)
	TGetTunnelRequest *argp;
	CLIENT *clnt;
{
	static THaTunnelInfo res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETTUNNELINFO, xdr_hacontroller_TGetTunnelRequest, argp, xdr_hacontroller_THaTunnelInfo, &res, TIMEOUT);
	return (&res);
}


void *
settimeout_8(argp, clnt)
	TTimeoutRequest *argp;
	CLIENT *clnt;
{
	static char res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETTIMEOUT, xdr_hacontroller_TTimeoutRequest, argp, xdr_void, &res, TIMEOUT);
	return ((void *)&res);
}


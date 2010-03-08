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
#include "facontroller.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_7(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_facontroller_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_7(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_7(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_facontroller_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
cstr_createagent_7(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_CREATEAGENT, xdr_void, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


int *
dstr_removeagent_7(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEAGENT, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TResult *
startmobileagent_7(argp, clnt)
	TStartAgentRequest *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STARTMOBILEAGENT, xdr_facontroller_TStartAgentRequest, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
stopmobileagent_7(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPMOBILEAGENT, xdr_int, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
getmobileagentstatus_7(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETMOBILEAGENTSTATUS, xdr_int, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
setsingleoption_7(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETSINGLEOPTION, xdr_facontroller_TOptionDesc, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
removesingleoption_7(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, REMOVESINGLEOPTION, xdr_facontroller_TOptionDesc, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
addlistoption_7(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ADDLISTOPTION, xdr_facontroller_TOptionDesc, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
removelistoption_7(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, REMOVELISTOPTION, xdr_facontroller_TOptionDesc, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TFaStatusInfo *
getstatus_7(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TFaStatusInfo res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETSTATUS, xdr_int, argp, xdr_facontroller_TFaStatusInfo, &res, TIMEOUT);
	return (&res);
}


TResult *
destroytunnelid_7(argp, clnt)
	TFaTunnelID *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DESTROYTUNNELID, xdr_facontroller_TFaTunnelID, argp, xdr_facontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TFaTunnelList *
listtunnels_7(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TFaTunnelList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LISTTUNNELS, xdr_int, argp, xdr_facontroller_TFaTunnelList, &res, TIMEOUT);
	return (&res);
}


TFaTunnelInfo *
gettunnelinfo_7(argp, clnt)
	TGetTunnelRequest *argp;
	CLIENT *clnt;
{
	static TFaTunnelInfo res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETTUNNELINFO, xdr_facontroller_TGetTunnelRequest, argp, xdr_facontroller_TFaTunnelInfo, &res, TIMEOUT);
	return (&res);
}


void *
settimeout_7(argp, clnt)
	TTimeoutRequest *argp;
	CLIENT *clnt;
{
	static char res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETTIMEOUT, xdr_facontroller_TTimeoutRequest, argp, xdr_void, &res, TIMEOUT);
	return ((void *)&res);
}


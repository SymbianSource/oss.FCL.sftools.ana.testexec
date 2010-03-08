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
#include "mncontroller.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_5(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_void, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_5(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_mncontroller_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
cstr_createagent_5(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_CREATEAGENT, xdr_void, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


int *
dstr_removeagent_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEAGENT, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TResult *
startmobileagent_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STARTMOBILEAGENT, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
stopmobileagent_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPMOBILEAGENT, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
getmobileagentstatus_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETMOBILEAGENTSTATUS, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
setsingleoption_5(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETSINGLEOPTION, xdr_mncontroller_TOptionDesc, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
removesingleoption_5(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, REMOVESINGLEOPTION, xdr_mncontroller_TOptionDesc, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
addlistoption_5(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ADDLISTOPTION, xdr_mncontroller_TOptionDesc, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
removelistoption_5(argp, clnt)
	TOptionDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, REMOVELISTOPTION, xdr_mncontroller_TOptionDesc, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
sethomeaddress_5(argp, clnt)
	THomeAddressDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETHOMEADDRESS, xdr_mncontroller_THomeAddressDesc, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
getcareofaddress_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETCAREOFADDRESS, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
connect_5(argp, clnt)
	TTunnelingModeDesc *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CONNECT, xdr_mncontroller_TTunnelingModeDesc, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
disconnect_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DISCONNECT, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
confirm_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CONFIRM, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
rescaninterfaces_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, RESCANINTERFACES, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
setfapolicy_5(argp, clnt)
	TPolicyRequest *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETFAPOLICY, xdr_mncontroller_TPolicyRequest, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
getfapolicy_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETFAPOLICY, xdr_int, argp, xdr_mncontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TMobileNodeStatus *
getstatus_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TMobileNodeStatus res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETSTATUS, xdr_int, argp, xdr_mncontroller_TMobileNodeStatus, &res, TIMEOUT);
	return (&res);
}


TForeignAgentList *
listforeignagents_5(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TForeignAgentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LISTFOREIGNAGENTS, xdr_int, argp, xdr_mncontroller_TForeignAgentList, &res, TIMEOUT);
	return (&res);
}


TForeignAgentInfo *
getforeignagentinfo_5(argp, clnt)
	TForeignAgentInfoRequest *argp;
	CLIENT *clnt;
{
	static TForeignAgentInfo res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETFOREIGNAGENTINFO, xdr_mncontroller_TForeignAgentInfoRequest, argp, xdr_mncontroller_TForeignAgentInfo, &res, TIMEOUT);
	return (&res);
}


void *
settimeout_5(argp, clnt)
	TTimeoutRequest *argp;
	CLIENT *clnt;
{
	static char res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETTIMEOUT, xdr_mncontroller_TTimeoutRequest, argp, xdr_void, &res, TIMEOUT);
	return ((void *)&res);
}


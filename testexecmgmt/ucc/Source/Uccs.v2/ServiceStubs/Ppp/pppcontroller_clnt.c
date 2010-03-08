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
#include "pppcontroller.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_4(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_pppcontroller_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_4(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_4(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_pppcontroller_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
cstr_startpppsession_4(argp, clnt)
	TPppSessionConfig *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_STARTPPPSESSION, xdr_pppcontroller_TPppSessionConfig, argp, xdr_pppcontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
dstr_removepppsession_4(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEPPPSESSION, xdr_int, argp, xdr_pppcontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
killsession_4(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, KILLSESSION, xdr_int, argp, xdr_pppcontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
stopsession_4(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPSESSION, xdr_int, argp, xdr_pppcontroller_TResult, &res, TIMEOUT);
	return (&res);
}


TPppSessionDesc *
getsessioninfo_4(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TPppSessionDesc res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETSESSIONINFO, xdr_int, argp, xdr_pppcontroller_TPppSessionDesc, &res, TIMEOUT);
	return (&res);
}


TVarData *
getppplog_4(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TVarData res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETPPPLOG, xdr_int, argp, xdr_pppcontroller_TVarData, &res, TIMEOUT);
	return (&res);
}


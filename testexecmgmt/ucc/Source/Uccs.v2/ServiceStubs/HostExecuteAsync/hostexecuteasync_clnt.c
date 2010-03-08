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
#include "hostexecuteasync.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_9(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_hostexecuteasync_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_9(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_hostexecuteasync_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
cstr_startprocess_9(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_STARTPROCESS, xdr_wrapstring, argp, xdr_hostexecuteasync_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
dstr_removeprocess_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEPROCESS, xdr_int, argp, xdr_hostexecuteasync_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
killprocess_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, KILLPROCESS, xdr_int, argp, xdr_hostexecuteasync_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
stopprocess_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPPROCESS, xdr_int, argp, xdr_hostexecuteasync_TResult, &res, TIMEOUT);
	return (&res);
}


THostExecuteAsyncProcessInfo *
getprocessinfo_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static THostExecuteAsyncProcessInfo res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETPROCESSINFO, xdr_int, argp, xdr_hostexecuteasync_THostExecuteAsyncProcessInfo, &res, TIMEOUT);
	return (&res);
}


TVarData *
getstandardoutput_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TVarData res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETSTANDARDOUTPUT, xdr_int, argp, xdr_hostexecuteasync_TVarData, &res, TIMEOUT);
	return (&res);
}


TVarData *
getstandarderror_9(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TVarData res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETSTANDARDERROR, xdr_int, argp, xdr_hostexecuteasync_TVarData, &res, TIMEOUT);
	return (&res);
}


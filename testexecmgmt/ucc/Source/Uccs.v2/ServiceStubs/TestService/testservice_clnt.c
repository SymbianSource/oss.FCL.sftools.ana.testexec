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
#include "testservice.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_11(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_testservice_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_11(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_11(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_testservice_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
st_discard_11(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_DISCARD, xdr_int, argp, xdr_testservice_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_block_11(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_BLOCK, xdr_int, argp, xdr_testservice_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_wait_11(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_WAIT, xdr_int, argp, xdr_testservice_TResult, &res, TIMEOUT);
	return (&res);
}


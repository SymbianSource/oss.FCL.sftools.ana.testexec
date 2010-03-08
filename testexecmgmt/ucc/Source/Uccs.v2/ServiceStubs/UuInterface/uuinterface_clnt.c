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
#include "uuinterface.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_2(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_uuinterface_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_2(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_2(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_uuinterface_TComponentList, &res, TIMEOUT);
	return (&res);
}


TResult *
st_setdelay_2(argp, clnt)
	TConfigValue *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_SETDELAY, xdr_uuinterface_TConfigValue, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_setdroppercentage_2(argp, clnt)
	TConfigValue *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_SETDROPPERCENTAGE, xdr_uuinterface_TConfigValue, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_setduplicatepercentage_2(argp, clnt)
	TConfigValue *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_SETDUPLICATEPERCENTAGE, xdr_uuinterface_TConfigValue, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_setbandwidth_2(argp, clnt)
	TConfigValue *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_SETBANDWIDTH, xdr_uuinterface_TConfigValue, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_setdrd_2(argp, clnt)
	TConfigValue *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_SETDRD, xdr_uuinterface_TConfigValue, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_clearsettings_2(argp, clnt)
	TConfigValue *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_CLEARSETTINGS, xdr_uuinterface_TConfigValue, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_stopservice_2(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_STOPSERVICE, xdr_void, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


TResult *
st_reset_2(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TResult res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ST_RESET, xdr_void, argp, xdr_uuinterface_TResult, &res, TIMEOUT);
	return (&res);
}


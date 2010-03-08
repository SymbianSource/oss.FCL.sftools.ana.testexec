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
#include "APICommandHandler.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_10(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_APICommandHandler_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_10(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_connections_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_CONNECTIONS, xdr_void, argp, xdr_APICommandHandler_TComponentList, &res, TIMEOUT);
	return (&res);
}


int *
cstr_startprocess_1(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_STARTPROCESS, xdr_wrapstring, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
dstr_removeprocess_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEPROCESS, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
startsimulator_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STARTSIMULATOR, xdr_void, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
stopsimulator_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPSIMULATOR, xdr_void, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setfielddefault_1(argp, clnt)
	TField *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETFIELDDEFAULT, xdr_APICommandHandler_TField, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setsatellitedefault_1(argp, clnt)
	TSatellite *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETSATELLITEDEFAULT, xdr_APICommandHandler_TSatellite, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
positionset_1(argp, clnt)
	TPositionInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, POSITIONSET, xdr_APICommandHandler_TPositionInfo, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
courseset_1(argp, clnt)
	TCourse *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, COURSESET, xdr_APICommandHandler_TCourse, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
accuracyset_1(argp, clnt)
	TAccuracy *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, ACCURACYSET, xdr_APICommandHandler_TAccuracy, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
satelliteset_1(argp, clnt)
	TSatellite *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SATELLITESET, xdr_APICommandHandler_TSatellite, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
batchappendsentence_1(argp, clnt)
	TAppendSentence *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, BATCHAPPENDSENTENCE, xdr_APICommandHandler_TAppendSentence, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
batchappendusersentence_1(argp, clnt)
	TAppendUserSentence *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, BATCHAPPENDUSERSENTENCE, xdr_APICommandHandler_TAppendUserSentence, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
batchsetdelay_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, BATCHSETDELAY, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
batchreset_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, BATCHRESET, xdr_void, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setcomport_1(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETCOMPORT, xdr_wrapstring, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setchunkmode_1(argp, clnt)
	bool_t *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETCHUNKMODE, xdr_bool, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setchunksize_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETCHUNKSIZE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setchunkdelay_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETCHUNKDELAY, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
startcomms_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STARTCOMMS, xdr_void, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
stopcomms_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPCOMMS, xdr_void, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
batchappenduserstring_1(argp, clnt)
	TAppendString *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, BATCHAPPENDUSERSTRING, xdr_APICommandHandler_TAppendString, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


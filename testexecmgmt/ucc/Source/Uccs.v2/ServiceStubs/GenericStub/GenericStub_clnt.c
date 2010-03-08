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
#include "GenericStub.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };
static struct timeval RUNCMD_TIMEOUT = { 3600, 0 }; // One hour (should be plenty)

int *
ss_startuprpcservice_11(argp, clnt)
	TStartupInfo *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_GenericStub_TStartupInfo, argp, xdr_int, &res, TIMEOUT);
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
list_connections_11(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_CONNECTIONS, xdr_void, argp, xdr_GenericStub_TComponentList, &res, TIMEOUT);
	return (&res);
}


int *
cstr_startprocess_11(argp, clnt)
	char **argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_STARTPROCESS, xdr_wrapstring, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
dstr_removeprocess_11(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEPROCESS, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
run_command_11(argp, clnt)
	TCall *argp;
	CLIENT *clnt;
{
	static int res;

	// Increase the timeout for this call
	clnt_control(clnt, CLSET_TIMEOUT, &RUNCMD_TIMEOUT);

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, RUN_COMMAND, xdr_GenericStub_TCall, argp, xdr_int, &res, RUNCMD_TIMEOUT);

	// Revert back to the default timeout 
	clnt_control(clnt, CLSET_TIMEOUT, &TIMEOUT);
	
	return (&res);
}


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
#include "./../MobsterRPCService/mobster.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
ss_startuprpcservice_1(argp, clnt)
	struct TChannelAddress *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SS_STARTUPRPCSERVICE, xdr_TChannelAddress, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
sc_shutdownrpcservice_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SC_SHUTDOWNRPCSERVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TComponentList *
list_devices_1(argp, clnt)
	void *argp;
	CLIENT *clnt;
{
	static TComponentList res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, LIST_DEVICES, xdr_void, argp, xdr_TComponentList, &res, TIMEOUT);
	return (&res);
}


int *
cstr_startdevice_1(argp, clnt)
	TDeviceDesc *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, CSTR_STARTDEVICE, xdr_TDeviceDesc, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
dstr_removedevice_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, DSTR_REMOVEDEVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


TDeviceDesc *
getdeviceinfo_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TDeviceDesc res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETDEVICEINFO, xdr_int, argp, xdr_TDeviceDesc, &res, TIMEOUT);
	return (&res);
}


TVarData *
getdevicelog_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static TVarData res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, GETDEVICELOG, xdr_int, argp, xdr_TVarData, &res, TIMEOUT);
	return (&res);
}


int *
stopdevice_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, STOPDEVICE, xdr_int, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


int *
setremoteuuaddress_1(argp, clnt)
	struct TUUAddress *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	clnt_call(clnt, SETREMOTEUUADDRESS, xdr_TUUAddress, argp, xdr_int, &res, TIMEOUT);
	return (&res);
}


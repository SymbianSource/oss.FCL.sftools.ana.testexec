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


bool_t
xdr_hostexecuteasync_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecuteasync_TResult(xdrs, objp)
	XDR *xdrs;
	TResult *objp;
{
	if (!xdr_int(xdrs, &objp->iStandardResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iExtendedCode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSystemError)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecuteasync_THostExecuteAsyncProcessInfo(xdrs, objp)
	XDR *xdrs;
	THostExecuteAsyncProcessInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iErrorCode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iErrorDetail)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iCommandLine, MAXCOMMANDLINE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessStatus)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessExitReason)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iExitCode)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecuteasync_TVarData(xdrs, objp)
	XDR *xdrs;
	TVarData *objp;
{
	if (!xdr_bytes(xdrs, (char **)&objp->TVarData_val, (u_int *)&objp->TVarData_len, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecuteasync_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iDummy)) {
		return (FALSE);
	}
	return (TRUE);
}



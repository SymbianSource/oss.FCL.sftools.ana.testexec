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
#include "hostexecute.h"


bool_t
xdr_hostexecute_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecute_TVarData(xdrs, objp)
	XDR *xdrs;
	TVarData *objp;
{
	if (!xdr_bytes(xdrs, (char **)&objp->TVarData_val, (u_int *)&objp->TVarData_len, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecute_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iEmpty)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecute_TExecuteRequest(xdrs, objp)
	XDR *xdrs;
	TExecuteRequest *objp;
{
	if (!xdr_vector(xdrs, (char *)objp->iCommandLine, MAXCOMMANDLINE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRecordStandardOut)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRecordStandartError)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTimeout)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hostexecute_TExecuteResult(xdrs, objp)
	XDR *xdrs;
	TExecuteResult *objp;
{
	if (!xdr_int(xdrs, &objp->iResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iErrorCode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iErrorDetail)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessExitReason)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessExitDetail)) {
		return (FALSE);
	}
	if (!xdr_hostexecute_TVarData(xdrs, &objp->iStandardOut)) {
		return (FALSE);
	}
	if (!xdr_hostexecute_TVarData(xdrs, &objp->iStandardError)) {
		return (FALSE);
	}
	return (TRUE);
}



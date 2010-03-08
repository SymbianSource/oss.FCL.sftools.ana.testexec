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


bool_t
xdr_pppcontroller_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_pppcontroller_TPppSessionStatus(xdrs, objp)
	XDR *xdrs;
	TPppSessionStatus *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_pppcontroller_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iEmpty)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_pppcontroller_TPppSessionConfig(xdrs, objp)
	XDR *xdrs;
	TPppSessionConfig *objp;
{
	if (!xdr_vector(xdrs, (char *)objp->iMobsterAddress, MAXADDRLEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iMobsterPort, MAXPORTLEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iSessionConfiguration, MAXCONFIGURATIONLENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMTID)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_pppcontroller_TPppSessionDesc(xdrs, objp)
	XDR *xdrs;
	TPppSessionDesc *objp;
{
	if (!xdr_int(xdrs, &objp->iErrorCode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iErrorDetail)) {
		return (FALSE);
	}
	if (!xdr_pppcontroller_TPppSessionConfig(xdrs, &objp->iConfig)) {
		return (FALSE);
	}
	if (!xdr_pppcontroller_TPppSessionStatus(xdrs, &objp->iSessionStatus)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iInterfaceName, MAXSESSIONNAMELEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iLocalIPAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRemoteIPAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessStatus)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessExitReason)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProcessExitCode)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_pppcontroller_TResult(xdrs, objp)
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
xdr_pppcontroller_TVarData(xdrs, objp)
	XDR *xdrs;
	TVarData *objp;
{
	if (!xdr_bytes(xdrs, (char **)&objp->TVarData_val, (u_int *)&objp->TVarData_len, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}



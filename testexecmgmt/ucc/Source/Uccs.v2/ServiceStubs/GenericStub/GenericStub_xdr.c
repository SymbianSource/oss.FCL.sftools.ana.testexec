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


bool_t
xdr_GenericStub_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_GenericStub_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iEmpty)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_GenericStub_TParam(xdrs, objp)
	XDR *xdrs;
	TParam *objp;
{
	if (!xdr_vector(xdrs, (char *)objp->iName, MAXPARAMNAMELENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iValue, MAXPARAMVALUELENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_GenericStub_TCall(xdrs, objp)
	XDR *xdrs;
	TCall *objp;
{
	if (!xdr_int(xdrs, &objp->iCallID)) {
		return (FALSE);
	}
	if (!xdr_array(xdrs, (char **)&objp->iParams.iParams_val, (u_int *)&objp->iParams.iParams_len, ~0, sizeof(TParam), (void*)xdr_GenericStub_TParam)) {
		return (FALSE);
	}
	return (TRUE);
}



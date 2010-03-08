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


bool_t
xdr_uuinterface_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_uuinterface_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iEmpty)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_uuinterface_TConfigValue(xdrs, objp)
	XDR *xdrs;
	TConfigValue *objp;
{
	if (!xdr_vector(xdrs, (char *)objp->iSourceAddress, X_MAX_ADDRESS_LENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iDestinationAddress, X_MAX_ADDRESS_LENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iProtocol)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSrcPort)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iDstPort)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iValueIntegerPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iValueFractionPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSigmaIntegerPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSigmaFractionPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iCorrelationIntegerPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iCorrelationFractionPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMaximumValueIntegerPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMaximumValueFractionPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iCongestionIntegerPart)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iCongestionFractionPart)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_uuinterface_TResult(xdrs, objp)
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



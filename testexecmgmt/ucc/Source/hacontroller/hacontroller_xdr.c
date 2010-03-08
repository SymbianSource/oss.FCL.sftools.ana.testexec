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
#include "hacontroller.h"


bool_t
xdr_hacontroller_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iBaseInterfaceIndex)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iBaseInterfaceNetworkMask)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iBaseInterfaceStartHostAddressRange)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iBaseInterfaceStopHostAddressRange)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iVirtualNetworkPrefix)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iVirtualNetworkPrefixMask)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iVirtualNetworkAllocSize)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_TResult(xdrs, objp)
	XDR *xdrs;
	TResult *objp;
{
	if (!xdr_int(xdrs, &objp->iServiceResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSubComponentResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iData0)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iData1)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_TOptionDesc(xdrs, objp)
	XDR *xdrs;
	TOptionDesc *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iOptionFilename, X_MAXOPTIONTOKENSIZE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iOptionToken, X_MAXOPTIONTOKENSIZE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iOptionValue, X_MAXOPTIONTOKENSIZE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iOptionBlockStart, X_MAXOPTIONTOKENSIZE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iOptionBlockEnd, X_MAXOPTIONTOKENSIZE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_THaStatus(xdrs, objp)
	XDR *xdrs;
	THaStatus *objp;
{
	if (!xdr_hacontroller_TResult(xdrs, &objp->iCallResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTunnelCount)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRequestsRejected)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRequestsAccepted)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iDiscardedUnknownExtension)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iDiscardedMalformed)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iDiscardedVendor)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iAdvertisementsSent)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iHomeAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iVirtualNetworkStartAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iVirtualNetworkSize)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_THaTunnelInfo(xdrs, objp)
	XDR *xdrs;
	THaTunnelInfo *objp;
{
	if (!xdr_hacontroller_TResult(xdrs, &objp->iCallResult)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iMobileNodeAddress)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iCareofAddress)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iHomeAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iCreationTime)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iExpirationTime)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRefreshTime)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iLastTimestamp)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSPI)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTimeout)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_THaTunnelID(xdrs, objp)
	XDR *xdrs;
	THaTunnelID *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMobileNodeAddress)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_THaTunnelList(xdrs, objp)
	XDR *xdrs;
	THaTunnelList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->THaTunnelList_val, (u_int *)&objp->THaTunnelList_len, ~0, sizeof(THaTunnelID), (void*)xdr_hacontroller_THaTunnelID)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_TTimeoutRequest(xdrs, objp)
	XDR *xdrs;
	TTimeoutRequest *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTimeout)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_hacontroller_TGetTunnelRequest(xdrs, objp)
	XDR *xdrs;
	TGetTunnelRequest *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMobileNodeAddress)) {
		return (FALSE);
	}
	return (TRUE);
}



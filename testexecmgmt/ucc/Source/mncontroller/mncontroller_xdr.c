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
#include "mncontroller.h"


bool_t
xdr_mncontroller_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TMnTunnelingMode(xdrs, objp)
	XDR *xdrs;
	TMnTunnelingMode *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TSetInterfaceErrors(xdrs, objp)
	XDR *xdrs;
	TSetInterfaceErrors *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TResult(xdrs, objp)
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
xdr_mncontroller_THomeAddressDesc(xdrs, objp)
	XDR *xdrs;
	THomeAddressDesc *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iHomeAddressPrefix)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iHomeAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iHomeAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iInterfaceName, X_MAXINTERFACENAMELEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TOptionDesc(xdrs, objp)
	XDR *xdrs;
	TOptionDesc *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
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
xdr_mncontroller_TTunnelingModeDesc(xdrs, objp)
	XDR *xdrs;
	TTunnelingModeDesc *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_mncontroller_TMnTunnelingMode(xdrs, &objp->iTunnelMode)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TPolicyRequest(xdrs, objp)
	XDR *xdrs;
	TPolicyRequest *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iPolicy)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TForeignAgentInfoRequest(xdrs, objp)
	XDR *xdrs;
	TForeignAgentInfoRequest *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iForeignAgentID)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TTimeoutRequest(xdrs, objp)
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
xdr_mncontroller_TMobileNodeStatus(xdrs, objp)
	XDR *xdrs;
	TMobileNodeStatus *objp;
{
	if (!xdr_mncontroller_TResult(xdrs, &objp->iCallResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iConnected)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTunnelUp)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iLocalAddress)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iColocatedAddress)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iForeignAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iHomeAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iHomeAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iLifetimeRemaining)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTunnelMode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSecondsSinceLastRequest)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSecondsSinceLastReply)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iReplyCode)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iInfoText, X_MAXINFOLEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iLastErrorString, X_MAXERRORSTRING, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TForeignAgentInfo(xdrs, objp)
	XDR *xdrs;
	TForeignAgentInfo *objp;
{
	if (!xdr_mncontroller_TResult(xdrs, &objp->iCallResult)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->iForeignAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iInterfaceName, X_MAXINTERFACENAMELEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iPriority)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iInUse)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iInterfaceIndex)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iLastAdvertisement)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iNAI, X_MAXNAILEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iAdvertisementExpiry)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mncontroller_TForeignAgentList(xdrs, objp)
	XDR *xdrs;
	TForeignAgentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TForeignAgentList_val, (u_int *)&objp->TForeignAgentList_len, ~0, sizeof(TForeignAgentInfo), (void*)xdr_mncontroller_TForeignAgentInfo)) {
		return (FALSE);
	}
	return (TRUE);
}



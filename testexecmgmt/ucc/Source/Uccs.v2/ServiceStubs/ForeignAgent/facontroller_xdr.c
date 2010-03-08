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
#include "facontroller.h"


bool_t
xdr_facontroller_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_facontroller_TSoliticationMode(xdrs, objp)
	XDR *xdrs;
	TSoliticationMode *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_facontroller_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iBaseInterfaceIndex)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iNetworkMask)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iStartHostAddressRange)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iStopHostAddressRange)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_facontroller_TResult(xdrs, objp)
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
xdr_facontroller_TOptionDesc(xdrs, objp)
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
xdr_facontroller_TTimeoutRequest(xdrs, objp)
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
xdr_facontroller_TGetTunnelRequest(xdrs, objp)
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




bool_t
xdr_facontroller_TStartAgentRequest(xdrs, objp)
	XDR *xdrs;
	TStartAgentRequest *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iLowerInterface, X_MAXINTERFACENAMESIZE, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_facontroller_TSoliticationMode(xdrs, &objp->iSolicitationMode)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSolicitationInterval)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_facontroller_TFaStatusInfo(xdrs, objp)
	XDR *xdrs;
	TFaStatusInfo *objp;
{
	if (!xdr_facontroller_TResult(xdrs, &objp->iCallResult)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTunnelCount)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iPendingRegistrationRequests)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRequestsRejected)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRequestsAccepted)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iReplysAccepted)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iReplysRejected)) {
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
	return (TRUE);
}




bool_t
xdr_facontroller_TFaTunnelID(xdrs, objp)
	XDR *xdrs;
	TFaTunnelID *objp;
{
	if (!xdr_int(xdrs, &objp->iAgentID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMobileNodeAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iHomeAgentAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iID)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_facontroller_TFaTunnelInfo(xdrs, objp)
	XDR *xdrs;
	TFaTunnelInfo *objp;
{
	if (!xdr_facontroller_TResult(xdrs, &objp->iCallResult)) {
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
	if (!xdr_int(xdrs, &objp->iPrivateHomeAgentID)) {
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
xdr_facontroller_TFaTunnelList(xdrs, objp)
	XDR *xdrs;
	TFaTunnelList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TFaTunnelList_val, (u_int *)&objp->TFaTunnelList_len, ~0, sizeof(TFaTunnelID), (void*)xdr_facontroller_TFaTunnelID)) {
		return (FALSE);
	}
	return (TRUE);
}



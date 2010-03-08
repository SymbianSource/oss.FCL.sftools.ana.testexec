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
#include "mobster.h"


bool_t
xdr_mobster_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TDatalinkLayer(xdrs, objp)
	XDR *xdrs;
	TDatalinkLayer *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TChannelStatus(xdrs, objp)
	XDR *xdrs;
	TChannelStatus *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TDeviceExitReason(xdrs, objp)
	XDR *xdrs;
	TDeviceExitReason *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TChannelAddress(xdrs, objp)
	XDR *xdrs;
	TChannelAddress *objp;
{
	if (!xdr_int(xdrs, &objp->iPort)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TUUAddress(xdrs, objp)
	XDR *xdrs;
	TUUAddress *objp;
{
	if (!xdr_int(xdrs, &objp->iDeviceID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRemoteAddress)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iRemotePort)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TDeviceDesc(xdrs, objp)
	XDR *xdrs;
	TDeviceDesc *objp;
{
	if (!xdr_mobster_TDatalinkLayer(xdrs, &objp->iDatalinkConfig)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iFilterConfig)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iMTID)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iTEID)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iLocalAirInterfacePort, MAXPORTLEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iRemoteAirInterfaceAddress, MAXADDRLEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iRemoteAirInterfacePort, MAXPORTLEN, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_mobster_TChannelStatus(xdrs, &objp->iDeviceStatus)) {
		return (FALSE);
	}
	if (!xdr_mobster_TDeviceExitReason(xdrs, &objp->iDeviceExitReason)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iDeviceExitSubReason)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iDeviceExitDetail)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_mobster_TVarData(xdrs, objp)
	XDR *xdrs;
	TVarData *objp;
{
	if (!xdr_bytes(xdrs, (char **)&objp->TVarData_val, (u_int *)&objp->TVarData_len, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}



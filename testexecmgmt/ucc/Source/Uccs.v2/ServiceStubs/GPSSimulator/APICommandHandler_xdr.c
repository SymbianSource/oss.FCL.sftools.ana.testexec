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
#include "APICommandHandler.h"


bool_t
xdr_APICommandHandler_TComponentList(xdrs, objp)
	XDR *xdrs;
	TComponentList *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->TComponentList_val, (u_int *)&objp->TComponentList_len, ~0, sizeof(int), (void*)xdr_int)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_FIELD_ID(xdrs, objp)
	XDR *xdrs;
	FIELD_ID *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_STATE(xdrs, objp)
	XDR *xdrs;
	STATE *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_NORTH_SOUTH(xdrs, objp)
	XDR *xdrs;
	NORTH_SOUTH *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_EAST_WEST(xdrs, objp)
	XDR *xdrs;
	EAST_WEST *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_SENTENCE_ID(xdrs, objp)
	XDR *xdrs;
	SENTENCE_ID *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_SATELLITE_ID(xdrs, objp)
	XDR *xdrs;
	SATELLITE_ID *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_ErrorCode(xdrs, objp)
	XDR *xdrs;
	ErrorCode *objp;
{
	if (!xdr_int(xdrs, objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TStartupInfo(xdrs, objp)
	XDR *xdrs;
	TStartupInfo *objp;
{
	if (!xdr_int(xdrs, &objp->iEmpty)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TPositionInfo(xdrs, objp)
	XDR *xdrs;
	TPositionInfo *objp;
{
	if (!xdr_double(xdrs, &objp->iLatitude)) {
		return (FALSE);
	}
	if (!xdr_bool(xdrs, &objp->iNS)) {
		return (FALSE);
	}
	if (!xdr_double(xdrs, &objp->iLongitude)) {
		return (FALSE);
	}
	if (!xdr_bool(xdrs, &objp->iEW)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TSatellite(xdrs, objp)
	XDR *xdrs;
	TSatellite *objp;
{
	if (!xdr_int(xdrs, &objp->iElevation)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iAzimuth)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iSNR)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iId)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TField(xdrs, objp)
	XDR *xdrs;
	TField *objp;
{
	if (!xdr_int(xdrs, &objp->iFieldId)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iState)) {
		return (FALSE);
	}
	if (!xdr_vector(xdrs, (char *)objp->iValue, MAXFIELDLENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TAppendSentence(xdrs, objp)
	XDR *xdrs;
	TAppendSentence *objp;
{
	if (!xdr_int(xdrs, &objp->iSentenceId)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iFrequency)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TAppendString(xdrs, objp)
	XDR *xdrs;
	TAppendString *objp;
{
	if (!xdr_vector(xdrs, (char *)objp->iUserString, MAXSTRINGLENGTH, sizeof(char), (void*)xdr_char)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iFrequency)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TAppendUserSentence(xdrs, objp)
	XDR *xdrs;
	TAppendUserSentence *objp;
{
	if (!xdr_APICommandHandler_TAppendSentence(xdrs, &objp->iAppendSentence)) {
		return (FALSE);
	}
	if (!xdr_array(xdrs, (char **)&objp->iUserFields.iUserFields_val, (u_int *)&objp->iUserFields.iUserFields_len, ~0, sizeof(TField), (void*)xdr_APICommandHandler_TField)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TCourse(xdrs, objp)
	XDR *xdrs;
	TCourse *objp;
{
	if (!xdr_double(xdrs, &objp->iSpeed)) {
		return (FALSE);
	}
	if (!xdr_double(xdrs, &objp->iDirection)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_APICommandHandler_TAccuracy(xdrs, objp)
	XDR *xdrs;
	TAccuracy *objp;
{
	if (!xdr_double(xdrs, &objp->iAccuracy)) {
		return (FALSE);
	}
	if (!xdr_double(xdrs, &objp->iHWConstant)) {
		return (FALSE);
	}
	return (TRUE);
}



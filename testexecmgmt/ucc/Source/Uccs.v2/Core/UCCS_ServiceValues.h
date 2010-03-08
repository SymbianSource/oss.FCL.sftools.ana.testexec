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
* Filename: UCCS_ServiceValues.h
*
*/



#ifndef __UCCSSERVICEVALUES_H__
#define __UCCSSERVICEVALUES_H__

typedef enum {
	RPCSVC_INVALID,				// 0
	RPCSVC_NULL,
	RPCSVC_HOMEAGENT,
	RPCSVC_FOREIGNAGENT,
	RPCSVC_MOBILEAGENT,
	RPCSVC_MOBSTER,				// 5
	RPCSVC_UUINTERFACE,
	RPCSVC_PPPCONTROLLER,
	RPCSVC_HOSTEXECUTE,
	RPCSVC_INTERNAL,
	RPCSVC_HOSTEXECUTEASYNC,	// 10
	RPCSVC_TEST,
	RPCSVC_TESTSERVICE,
	RPCSVC_GPSSIMULATOR,
//	RPCSVC_GENERICSTUB,
	RPCSVC_LAST
} TRpcServiceID;

#endif __UCCSSERVICEVALUES_H__

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




#ifndef __CCFACONTROLLER_H__
#define __CCFACONTROLLER_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "facontroller.h"


/****************************************************************************************
 * 
 * Definition: CCFacontroller
 * 
 ***************************************************************************************/
class CCFacontroller
{
public:
	// standard methods
	CCFacontroller();
	~CCFacontroller();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_devices( TComponentList *rv );
	int cstr_createagent( TResult *rv );
	int dstr_removeagent( int aArgs, int *rv );
	int startmobileagent( TStartAgentRequest aArgs, TResult *rv );
	int stopmobileagent( int aArgs, TResult *rv );
	int getmobileagentstatus( int aArgs, TResult *rv );
	int setsingleoption( TOptionDesc aArgs, TResult *rv );
	int removesingleoption( TOptionDesc aArgs, TResult *rv );
	int addlistoption( TOptionDesc aArgs, TResult *rv );
	int removelistoption( TOptionDesc aArgs, TResult *rv );
	int getstatus( int aArgs, TFaStatusInfo *rv );
	int destroytunnelid( TFaTunnelID aArgs, TResult *rv );
	int listtunnels( int aArgs, TFaTunnelList *rv );
	int gettunnelinfo( TGetTunnelRequest aArgs, TFaTunnelInfo *rv );
	int settimeout( TTimeoutRequest aArgs );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

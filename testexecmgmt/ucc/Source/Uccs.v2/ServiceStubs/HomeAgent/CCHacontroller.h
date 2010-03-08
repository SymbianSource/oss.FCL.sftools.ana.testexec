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




#ifndef __CCHACONTROLLER_H__
#define __CCHACONTROLLER_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "hacontroller.h"


/****************************************************************************************
 * 
 * Definition: CCHacontroller
 * 
 ***************************************************************************************/
class CCHacontroller
{
public:
	// standard methods
	CCHacontroller();
	~CCHacontroller();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_devices( TComponentList *rv );
	int cstr_createagent( TResult *rv );
	int dstr_removeagent( int aArgs, TResult *rv );
	int startmobileagent( int aArgs, TResult *rv );
	int stopmobileagent( int aArgs, TResult *rv );
	int getmobileagentstatus( int aArgs, TResult *rv );
	int setsingleoption( TOptionDesc aArgs, TResult *rv );
	int removesingleoption( TOptionDesc aArgs, TResult *rv );
	int addlistoption( TOptionDesc aArgs, TResult *rv );
	int removelistoption( TOptionDesc aArgs, TResult *rv );
	int getstatus( int aArgs, THaStatus *rv );
	int destroytunnelid( THaTunnelID aArgs, TResult *rv );
	int listtunnels( int aArgs, THaTunnelList *rv );
	int gettunnelinfo( TGetTunnelRequest aArgs, THaTunnelInfo *rv );
	int settimeout( TTimeoutRequest aArgs );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

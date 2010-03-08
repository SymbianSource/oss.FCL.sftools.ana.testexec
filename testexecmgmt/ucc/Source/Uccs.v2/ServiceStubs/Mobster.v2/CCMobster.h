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




#ifndef __CCMOBSTER_H__
#define __CCMOBSTER_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "mobster.h"


/****************************************************************************************
 * 
 * Definition: CCMobster
 * 
 ***************************************************************************************/
class CCMobster
{
public:
	// standard methods
	CCMobster();
	~CCMobster();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( struct TChannelAddress aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_devices( TComponentList *rv );
	int cstr_startdevice( TDeviceDesc aArgs, int *rv );
	int dstr_removedevice( int aArgs, int *rv );
	int getdeviceinfo( int aArgs, TDeviceDesc *rv );
	int getdevicelog( int aArgs, TVarData *rv );
	int stopdevice( int aArgs, int *rv );
	int setremoteuuaddress( struct TUUAddress aArgs, int *rv );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

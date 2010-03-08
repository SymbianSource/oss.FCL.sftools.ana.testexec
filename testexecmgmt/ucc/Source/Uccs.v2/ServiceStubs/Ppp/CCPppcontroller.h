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




#ifndef __CCPPPCONTROLLER_H__
#define __CCPPPCONTROLLER_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "pppcontroller.h"


/****************************************************************************************
 * 
 * Definition: CCPppcontroller
 * 
 ***************************************************************************************/
class CCPppcontroller
{
public:
	// standard methods
	CCPppcontroller();
	~CCPppcontroller();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_devices( TComponentList *rv );
	int cstr_startpppsession( TPppSessionConfig aArgs, TResult *rv );
	int dstr_removepppsession( int aArgs, TResult *rv );
	int killsession( int aArgs, TResult *rv );
	int stopsession( int aArgs, TResult *rv );
	int getsessioninfo( int aArgs, TPppSessionDesc *rv );
	int getppplog( int aArgs, TVarData *rv );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

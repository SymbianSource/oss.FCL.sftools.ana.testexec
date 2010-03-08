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




#ifndef __CCHOSTEXECUTEASYNC_H__
#define __CCHOSTEXECUTEASYNC_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "hostexecuteasync.h"


/****************************************************************************************
 * 
 * Definition: CCHostexecuteasync
 * 
 ***************************************************************************************/
class CCHostexecuteasync
{
public:
	// standard methods
	CCHostexecuteasync();
	~CCHostexecuteasync();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_devices( TComponentList *rv );
	int cstr_startprocess( char *aArgs, TResult *rv );
	int dstr_removeprocess( int aArgs, TResult *rv );
	int killprocess( int aArgs, TResult *rv );
	int stopprocess( int aArgs, TResult *rv );
	int getprocessinfo( int aArgs, THostExecuteAsyncProcessInfo *rv );
	int getstandardoutput( int aArgs, TVarData *rv );
	int getstandarderror( int aArgs, TVarData *rv );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

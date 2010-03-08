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




#ifndef __CCTESTSERVICE_H__
#define __CCTESTSERVICE_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "testservice.h"


/****************************************************************************************
 * 
 * Definition: CCTestservice
 * 
 ***************************************************************************************/
class CCTestservice
{
public:
	// standard methods
	CCTestservice();
	~CCTestservice();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_devices( TComponentList *rv );
	int st_discard( int aArgs, TResult *rv );
	int st_block( int aArgs, TResult *rv );
	int st_wait( int aArgs, TResult *rv );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

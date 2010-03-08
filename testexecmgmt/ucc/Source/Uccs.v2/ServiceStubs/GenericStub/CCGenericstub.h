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




#ifndef __CCGENERICSTUB_H__
#define __CCGENERICSTUB_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "GenericStub.h"


/****************************************************************************************
 * 
 * Definition: CCGenericstub
 * 
 ***************************************************************************************/
class CCGenericstub
{
public:
	// standard methods
	CCGenericstub( const int anIID, const int iVersion );
	~CCGenericstub();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_connections( TComponentList *rv );
	int cstr_startprocess( char *aArgs, int *rv );
	int dstr_removeprocess( int aArgs, int *rv );
	int run_command( TCall aArgs, int *rv );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
	int iVersion;
	int iIID;
};

#endif

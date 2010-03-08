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




#ifndef __CCGPSSIMULATOR_H__
#define __CCGPSSIMULATOR_H__


/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <rpc/rpc.h>
#include <string>
using namespace std;
#include "APICommandHandler.h"


/****************************************************************************************
 * 
 * Definition: CCGpssimulator
 * 
 ***************************************************************************************/
class CCGpssimulator
{
public:
	// standard methods
	CCGpssimulator();
	~CCGpssimulator();
	int Connect( string aRemoteHost );
	int Disconnect();
	char *GetLastRPCError( int *aIntErr );

	// service methods
	int ss_startuprpcservice( TStartupInfo aArgs, int *rv );
	int sc_shutdownrpcservice( int aArgs, int *rv );
	int list_connections( TComponentList *rv );
	int cstr_startprocess( char *aArgs, int *rv );
	int dstr_removeprocess( int aArgs, int *rv );
	int startsimulator( int *rv );
	int stopsimulator( int *rv );
	int setfielddefault( TField aArgs, int *rv );
	int setsatellitedefault( TSatellite aArgs, int *rv );
	int positionset( TPositionInfo aArgs, int *rv );
	int courseset( TCourse aArgs, int *rv );
	int accuracyset( TAccuracy aArgs, int *rv );
	int satelliteset( TSatellite aArgs, int *rv );
	int batchappendsentence( TAppendSentence aArgs, int *rv );
	int batchappendusersentence( TAppendUserSentence aArgs, int *rv );
	int batchsetdelay( int aArgs, int *rv );
	int batchreset( int *rv );
	int setcomport( char *aArgs, int *rv );
	int setchunkmode( bool_t aArgs, int *rv );
	int setchunksize( int aArgs, int *rv );
	int setchunkdelay( int aArgs, int *rv );
	int startcomms( int *rv );
	int stopcomms( int *rv );
	int batchappenduserstring( TAppendString aArgs, int *rv );

private:
	struct rpc_err iLastRPCError;
	CLIENT *cl;
};

#endif

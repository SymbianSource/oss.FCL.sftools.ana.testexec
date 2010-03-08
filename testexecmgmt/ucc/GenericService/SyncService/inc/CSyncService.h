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



#ifndef __CSYNCSERVICE_H__
#define __CSYNCSERVICE_H__

// Define _WINSOCK2API_ so windows.h doesn't include it.
// This fixes compilations errors due to winsock.h being
//  included by the ONC RPC static library.
#define _WINSOCK2API_
#include <windows.h>
#include <STATExp.h>
#include <string>

#include "CService.h"

// Define some error codes specific to this service
#define ERR_STAT							-100
#define ERR_STAT_START_SYNC_TEST_CASE		-101
#define ERR_STAT_RETRIEVE_SYNC_TEST_RESULT	-102
#define ERR_STAT_RETRIEVE_SYNC_TEST_STATUS	-103
#define ERR_STAT_SET_SHARED_DATA			-104

class CSyncService : public CService
	{
public:
	CSyncService();
	~CSyncService();

	virtual bool Setup();

	// Handle the parsing/running of particular commands
	virtual int RunCommand( const CCall& aCall );

private:
	HMODULE					ihLib;
	PROC_SENDRAWCOMMAND		iptrSendRawCommand;
	PROC_GETTEFSHAREDDATA	iptrGetTEFSharedData;
	PROC_CONNECT			iptrConnect;
	PROC_DISCONNECT			iptrDisconnect;

	int						iConnection;
	STATConnectType			iConnectionType;
	string					iComPort;
	string					iIPAddress;
	string					iSTATDLLLocation;

	int						iTimeout;
	int						iPollInterval;

	bool	RetrieveCommsInfo();
	int		StartSyncTestCase( const CCall& aCall );
	int		RetrieveSyncTestCaseResult( const CCall& aCall );
	int		SetSharedData( const CCall& aCall );
	};

#endif // __CSYNCSERVICE_H__

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



#ifndef __CREMOTEINTERFACE_H__
#define __CREMOTEINTERFACE_H__

#include <string>
#include "MUCCTransport.h"

#define MAXCOMMANDLINELEN		1024

// Error codes
typedef enum {
	TRI_SUCCESS,
	TRI_ERRINTIALISING,
	TRI_ERRSENDING,
	TRI_ERRRECV,
	TRI_INVALIDREPLY,
	TRI_VARNAMETOOLONG,
	TRI_VARVALTOOLONG,
	TRI_COMMANDLINETOOLONG
} TRIError;

// Definitions
#define CMD_REQ_RUNCOMMAND		6

typedef struct {
	int iUid;
	int iCmdID;
	int iDataLen;
} TPHeader;

typedef struct {
	char iCommandLine[MAXCOMMANDLINELEN] ;
	int iResult;
} TRunCommandRequest;

typedef struct {
	int iResult;
} TRunCommandReply;

class CRemoteInterface
	{
public:
	CRemoteInterface( const string &aDestination );
	~CRemoteInterface();

	void SetupStartService( const string &aServiceName );
	void SetupStopService( const string &aServiceName );
	void SetupCall( const string &aServiceName, const string &aCall );
	void AddParam( const string &aName, const string &aValue );
	int Send();

private:
	void DefaultSetupCall( const string &aServiceName, const string &aMethodID );

	MUCCTransport	*iTransport;
	bool			iConnected;
	string			iDestination;
	bool			iIsSetup;
	string			iCommandLine;

	int				iRand_UID;
	double			iRandomSeed;
	};

#endif // __CREMOTEINTERFACE_H__

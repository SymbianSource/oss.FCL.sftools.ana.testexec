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




#ifndef __CGENERICSTUB_H__
#define __CGENERICSTUB_H__

#include "../../Core/UCCS_Interfaces.h"
#include "../CommonServiceStub/CServiceAgentBase.h"
#include "CUCCIniFile.h"

#define MAXINIFILEPATH 1024

class CCGenericstub;

class CGenericStub : public IService, public CServiceAgentBase
{
public:
	CGenericStub( char* aIniFile, char* aServiceName );
	~CGenericStub();

	int StartUccsService( char *aHostName, int* aLocalErr, int* aRemoteErr );
	int StopUccsService(int* aLocalErr, int* aRemoteErr );
	CDataRecord* IssueCommand(CDataRecord* aCommand);
	int GetStatus();
	char *GetLastRPCError( int *aIntError );

private:
	CCGenericstub*	iGenericStub;
	CUCCIniFile*	iIniFile;
};

#endif //__CGENERICSTUB_H__

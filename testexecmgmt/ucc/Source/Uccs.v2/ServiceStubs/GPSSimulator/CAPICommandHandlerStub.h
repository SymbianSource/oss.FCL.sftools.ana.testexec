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




#ifndef __CAPICOMMANDHANDLERSTUB_H__
#define __CAPICOMMANDHANDLERSTUB_H__

#include "../../Core/UCCS_Interfaces.h"
#include "../CommonServiceStub/CServiceAgentBase.h"
#include "CCGpssimulator.h"

class CCGpssimulator;

class CAPICommandHandlerStub : public IService, public CServiceAgentBase
{
public:
	CAPICommandHandlerStub();
	~CAPICommandHandlerStub();

	int StartUccsService( char *aHostName, int* aLocalErr, int* aRemoteErr );
	int StopUccsService(int* aLocalErr, int* aRemoteErr );
	CDataRecord* IssueCommand(CDataRecord* aCommand);
	int GetStatus();
	char *GetLastRPCError( int *aIntError );

private:
	CCGpssimulator*	iGPSSimulator;
};

#endif //__CAPICOMMANDHANDLERSTUB_H__

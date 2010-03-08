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
* Filename: CTestStub.h
* This class is a simple stub application which is used primarily
* for the UCCS.  It does not do anything complicated and hence is good for 
* isolating the upper levels of the system.
*
*/



#ifndef __CTESTSTUB_H__
#define __CTESTSTUB_H__


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "../../Core/UCCS_Interfaces.h"
#include "../CommonServiceStub/CServiceAgentBase.h"

/*******************************************************************************
 *
 * CTestStub
 *
 ******************************************************************************/
class CTestStub : public IService, public CServiceAgentBase
{
public:
	CTestStub();
	~CTestStub();

	int StartUccsService( char *aHostName, int *aLocalError, int *aRemoteError );
	int StopUccsService( int *aLocalerror, int *aRemoteError );
	CDataRecord* IssueCommand(CDataRecord* aCommand);
	int GetStatus();
	char *GetLastRPCError( int *aIntError );
};

#endif __CTESTSTUB_H__

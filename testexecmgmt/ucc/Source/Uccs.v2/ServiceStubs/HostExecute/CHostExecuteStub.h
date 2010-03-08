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
* CHostExecuteStub -- this class is the 'glue' between a standardised
* UCCS interface (IService) and the custom service interface.
*
*/




#ifndef __CHOSTEXECUTESTUB_H__
#define __CHOSTEXECUTESTUB_H__


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "../../Core/UCCS_Interfaces.h"
#include "../CommonServiceStub/CServiceAgentBase.h"


/*******************************************************************************
 *
 * Forward declaration. 
 *
 ******************************************************************************/
class CCHostexecute;


/*******************************************************************************
 *
 * CHostExecuteStub
 *
 ******************************************************************************/
class CHostExecuteStub : public IService, public CServiceAgentBase
{
public:
	CHostExecuteStub();
	~CHostExecuteStub();

	int StartUccsService( char *aHostName, int* aLocalErr, int* aRemoteErr );
	int StopUccsService(int* aLocalErr, int* aRemoteErr );
	CDataRecord* IssueCommand(CDataRecord* aCommand);
	int GetStatus();
	char *GetLastRPCError( int *aIntError );

private:
	CCHostexecute *iClientHostExecute;
};

#endif //__CHOSTEXECUTESTUB_H__

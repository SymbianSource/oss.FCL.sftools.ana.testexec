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
* CForeignAgentServiceStub -- this class is the 'glue' between a standardised
* UCCS interface (IService) and the custom service interface.
*
*/




#ifndef __CFOREIGNAGENTSERVICESTUB_H__
#define __CFOREIGNAGENTSERVICESTUB_H__


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
class CCFacontroller;


/*******************************************************************************
 *
 * CForeignAgentServiceStub
 *
 ******************************************************************************/
class CForeignAgentServiceStub : public IService, public CServiceAgentBase
{
public:
	CForeignAgentServiceStub();
	~CForeignAgentServiceStub();

	int StartUccsService( char *aHostName, int *aLocalError, int *aRemoteError );
	int StopUccsService( int *aLocalerror, int *aRemoteError );
	CDataRecord* IssueCommand(CDataRecord* aCommand);
	int GetStatus();
	char *GetLastRPCError( int *aIntError );

private:
	CCFacontroller *iClientForeignAgent;
	char* iHostName;
};

#endif //__CFOREIGNAGENTSERVICESTUB_H__

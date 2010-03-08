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
* CPdsnClientServiceStub -- this class is the 'glue' between a standardised
* UCCS interface (IService) and the custom service interface.
*
*/




#ifndef __CPPPCONTROLLERSTUB_H__
#define __CPPPCONTROLLERSTUB_H__


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
class CCPppcontroller;


/*******************************************************************************
 *
 * CPppControllerServiceStub
 *
 ******************************************************************************/
class CPppControllerServiceStub : public IService, public CServiceAgentBase
{
public:
	CPppControllerServiceStub();
	~CPppControllerServiceStub();

	int StartUccsService( char *aHostName, int *aErrorCode, int *aUnused );
	int StopUccsService( int *aErrorCode, int *aUnused );
	CDataRecord* IssueCommand( CDataRecord* aCommand );
	int GetStatus();
	char *GetLastRPCError( int *aIntError );

private:
	CCPppcontroller *iClientPppController;
};

#endif //__CPPPCONTROLLERSTUB_H__

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
* CUuinterfaceStub -- this class is the 'glue' between a standardised
* UCCS interface (IService) and the custom service interface.
*
*/




#ifndef __CUUINTERFACESTUB_H__
#define __CUUINTERFACESTUB_H__


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "../../Core/UCCS_Interfaces.h"
#include "../CommonServiceStub/CServiceAgentBase.h"

/*******************************************************************************
 *
 * Defines 
 *
 ******************************************************************************/
#define TCP	"tcp"
#define UDP "udp"

/*******************************************************************************
 *
 * Forward declaration. 
 *
 ******************************************************************************/
class CCUuinterface;
struct TConfigValue;

/*******************************************************************************
 *
 * CHostExecuteStub
 *
 ******************************************************************************/
class CUuinterfaceStub : public IService, public CServiceAgentBase
{
public:
	CUuinterfaceStub();
	~CUuinterfaceStub();

	int StartUccsService( char *aHostName, int* aLocalErr, int* aRemoteErr );
	int StopUccsService(int* aLocalErr, int* aRemoteErr );
	CDataRecord* IssueCommand(CDataRecord* aCommand);
	int GetStatus();
	char *GetLastRPCError( int *aIntError );

private:
	void CreateConfigValue( TConfigValue* aConfigValue,
							char* aSrc, char* aDst,  
							char* aProtocol, 
							int aSrcPort, int aDstPort,
							int aValueIntegerPart, int aValueFractionPart, 
							int aSigmaIntegerPart, int aSigmaFractionPart,
							int aCorrelationIntegerPart, int aCorrelationFractionPart,
							int aMaximumValueIntegerPart, int aMaximumValueFractionPart,
							int aCongestionIntegerPart, int aCongestionFractionPart );

	CCUuinterface *iUuClient;
};

#endif //__CUUINTERFACESTUB_H__

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
* Filename: UCCSServMgr.h
*
*/



#ifndef __UCCSSERVICEMANAGER_H__
#define __UCCSSERVICEMANAGER_H__

/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_CDataRecord.h"
#include "UCCS_Interfaces.h"
#include "UCCS_ServiceValues.h"

/***********************************************************************************
 *
 * Definition
 *
 **********************************************************************************/
#define MAXSERVICES			128
#define MAXHOSTNAME			256
#define MAXINIPATH			1024
#define MAXSERVICENAME		256

/***********************************************************************************
 *
 * Types
 *
 **********************************************************************************/
typedef struct 
{
	TRpcServiceID	iServiceID;
	char			iHostname[MAXHOSTNAME];
} TServiceTableEntry;

typedef struct 
{
	int		iServiceID;
	char	iServiceName[MAXSERVICENAME];
	char	iIniFile[MAXINIPATH];
	char	iHost[MAXHOSTNAME];
} TGenericService;


//----------------------------------------------------------------------------------
//
// Class used to communicate with the stub interfaces.
// Class uses same methods as IService but methods here also have a service ID to use.
//
class CServiceManager
{
	public:

		//constructor/destructor
		CServiceManager( IOutput *aOutput );
		~CServiceManager();
		int ShutdownAll();
		
		// duplicated interface method - the only one to be seen from outside at this level
		int IssueCommand(CDataRecord* aDataRecord, CDataRecord** aReturnedDataRecord);

		// reset a service
		int ResetService( int aServiceID, char *aHostname );
		int ResetAllServices();

	private:

		// get the local service index
		int GetLocalServiceIndex( int aServiceID, char *aServiceHost );

		// duplicated interface methods used to dumultiplex for the higher layers
		int LocalStartService( int aServiceID, char *aServiceHost );
		int LocalStopService( int aLocalServiceIndex );
		
		//defines interface pointers to the service handlers
		IService *iServiceList[MAXSERVICES];	
		TServiceTableEntry iServiceDescTable[MAXSERVICES];
		IOutput *iOutput;

		// GenericStub Service id's
		int iNumGenericServices;
		TGenericService iGenericServices[MAXSERVICES];
};

//----------------------------------------------------------------------------------
#endif __UCCSSERVICEMANAGER_H__

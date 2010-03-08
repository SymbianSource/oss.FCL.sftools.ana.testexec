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



/**
 @file CUCCServer.cpp
*/

#include "CUCCServer.h"
#include "CUCCTest.h"

_LIT(KServerName,"UCCControlTE");

CUCCServer* CUCCServer::NewL()
/**
 * @return - Instance of the test server
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CUCCServer * server = new (ELeave) CUCCServer();
	CleanupStack::PushL(server);
	
	// Either use a StartL or ConstructL, the latter will permit
	// Server Logging.

	//server->StartL(KServerName); 
	server-> ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

// EKA2 much simpler
// Just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CUCCServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CUCCServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

// Only a DLL on emulator for  and earlier

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on exit
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());
	delete cleanup;
	return KErrNone;
    }

// Create a thread in the calling process
// Emulator  and earlier

CTestStep* CUCCServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep* testStep = NULL;

	if(aStepName == KTestStartUseCase)
		testStep = new CUCCTestStartUseCase();
	if(aStepName == KTestSignal)
		testStep = new CUCCTestSignal();
	if(aStepName == KTestRendezvous)
		testStep = new CUCCTestRendezvous();
	if(aStepName == KTestWaitfor)
		testStep = new CUCCTestWaitfor();
	if(aStepName == KTestEndUseCase)
		testStep = new CUCCTestEndUseCase();
	if(aStepName == KTestGetVariableName)
		testStep = new CUCCTestGetVariableName();
	if(aStepName == KTestSetDeviceID)
		testStep = new CUCCTestSetDeviceID();
	if(aStepName == KTestRunCommand )
		testStep = new CUCCTestRunCommand();

	return testStep;
	}


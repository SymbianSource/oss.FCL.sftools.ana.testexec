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
 @file CUCCServer.h
*/

#ifndef __UCC_SERVER_H__
#define __UCC_SERVER_H__

#include <testexecuteserverbase.h>

class CUCCServer : public CTestServer
	{
public:
	static CUCCServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	RFs& Fs(){return iFs;};

private:
	RFs iFs;
	};

#endif // __UCC_SERVER_H__

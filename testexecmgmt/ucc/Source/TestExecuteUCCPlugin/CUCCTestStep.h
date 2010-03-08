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
 @file CUCCTestStep.h
*/

#ifndef __UCC_TEST_STEP_H__
#define __UCC_TEST_STEP_H__

#include <testexecuteserverbase.h>
#include <networkemulator/cuccsdevicecontrol.h>

class CUCCTestStep : public CTestStep
	{
public:
	CUCCTestStep();
	~CUCCTestStep();
	CUCCSDeviceControl *CreateApplicationControlInstance( int *aUID, TBool aUIDRequired = ETrue );
	CUCCTestStep * iSelfTestSuite;
	};

#endif // __UCC_TEST_STEP_H__

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
 @file CUCCTest.h
*/

#ifndef __UCC_TEST_H__
#define __UCC_TEST_H__

#include <testexecutestepbase.h>
#include "CUCCTestStep.h"
#include "CUCCServer.h"

class CUCCTestStartUseCase : public CUCCTestStep
	{
public:
	CUCCTestStartUseCase();
	~CUCCTestStartUseCase();
	virtual TVerdict doTestStepL();
	};

class CUCCTestSignal : public CUCCTestStep
	{
public:
	CUCCTestSignal();
	~CUCCTestSignal();
	virtual TVerdict doTestStepL();
	};

class CUCCTestRendezvous : public CUCCTestStep
	{
public:
	CUCCTestRendezvous();
	~CUCCTestRendezvous();
	virtual TVerdict doTestStepL();
	};

class CUCCTestWaitfor : public CUCCTestStep
	{
public:
	CUCCTestWaitfor();
	~CUCCTestWaitfor();
	virtual TVerdict doTestStepL();
	};

class CUCCTestEndUseCase : public CUCCTestStep
	{
public:
	CUCCTestEndUseCase();
	~CUCCTestEndUseCase();
	virtual TVerdict doTestStepL();
	};

class CUCCTestGetVariableName : public CUCCTestStep
	{
public:
	CUCCTestGetVariableName();
	~CUCCTestGetVariableName();
	virtual TVerdict doTestStepL();
	};

class CUCCTestRunCommand : public CUCCTestStep
	{
public:
	CUCCTestRunCommand();
	~CUCCTestRunCommand();
	virtual TVerdict doTestStepL();
	};

class CUCCTestSetDeviceID : public CUCCTestStep
	{
public:
	CUCCTestSetDeviceID();
	~CUCCTestSetDeviceID();
	virtual TVerdict doTestStepL();
private:
	TVerdict UpdateCommdbEntry( CUCCSDeviceControl *aApp );
	};

_LIT(KTestStartUseCase,"StartUseCase");
_LIT(KTestSignal,"Signal");
_LIT(KTestRendezvous,"Rendezvous");
_LIT(KTestWaitfor,"Waitfor");
_LIT(KTestEndUseCase,"EndUseCase");
_LIT(KTestGetVariableName,"GetVariableName");
_LIT(KTestRunCommand,"RunCommand");
_LIT(KTestSetDeviceID,"SetDeviceID");

#endif // __UCC_TEST_H__

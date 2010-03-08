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



#ifndef __CTESTDRIVERSERVICE_H__
#define __CTESTDRIVERSERVICE_H__

#include <string>
#include "CService.h"

// Define some error codes specific to this service
#define ERR_TEST_DRIVER_BUILD			-110
#define ERR_TEST_DRIVER_RUN				-111
#define ERR_TEST_DRIVER_RUN_COMPLETE	-112
#define ERR_TEST_DRIVER_ALREADY_RUNNING	-113

class CTestDriverService : public CService
	{
public:
	CTestDriverService();
	~CTestDriverService();

	virtual bool Setup();

	// Handle the parsing/running of particular commands
	virtual int RunCommand( const CCall& aCall );

private:
	int			BuildAndRunTestDriver( const CCall& aCall );
	int			RetrieveTestDriverRunResult();
	void		RetrieveConfig();
	int			AppendCmdLineParams( const CCall& aCall, string &aCmdLine );
	
	static void ListeningServer( void );
	static void TestDriverRun( void );
	static void	Wait( HANDLE aMutexHandle );
	static void	Release( HANDLE aMutexHandle );
	};

#endif // __CTESTDRIVERSERVICE_H__

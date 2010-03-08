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



#ifndef __CTESTSERVICE_H__
#define __CTESTSERVICE_H__

#include "CService.h"

class CTestService : public CService
	{
	public:
		CTestService();
		~CTestService();

		// Handle the parsing/running of particular commands
		virtual int RunCommand( const CCall& aCall );

	private:

	};

CService* Service() { return new CTestService(); }

#endif // __CTESTSERVICE_H__

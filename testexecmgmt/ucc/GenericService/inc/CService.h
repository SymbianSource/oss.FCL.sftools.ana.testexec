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



#ifndef __CSERVICE_H__
#define __CSERVICE_H__

#include "CCall.h"

class CService
	{
	public:
		CService() {}
		virtual ~CService() {}

		// Virtual function can be implemented in the derived class
		//  if any setup is required for the service.
		virtual bool Setup() { return true; }

		// Pure virtual function needs to be implemented
		// Handle the parsing/running of particular commands
		virtual int RunCommand( const CCall& aCall ) = 0;
	};

// This also need to be implemented to return an instance
// of your derived CService class
extern CService* Service();

#endif // __CSERVICE_H__

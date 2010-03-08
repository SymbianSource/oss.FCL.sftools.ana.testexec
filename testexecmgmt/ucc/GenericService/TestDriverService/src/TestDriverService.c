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
* TestDrivercService.c
*
*/



#define TEST_SERVICE_IID		0x34630777
#define TEST_SERVICE_VERSION	1

#include <stdlib.h>
#include "GenericStub.h"

int main()
	{
	return StartUCCService( TEST_SERVICE_IID, TEST_SERVICE_VERSION );
	}

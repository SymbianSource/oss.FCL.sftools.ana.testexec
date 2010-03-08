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
* CIntegerAllocator
*
*/



#ifndef __CINTEGERALLOCATOR_H__
#define __CINTEGERALLOCATOR_H__


/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <vector>
using namespace std;


/*******************************************************************************
 *
 * CIntegerAllocator
 *
 ******************************************************************************/
class CIntegerAllocator
{
public:
	int AddToIntegerPool( int aStart, int aEnd );
	int AllocateInteger();
	int FreeInteger( int aFreeInteger );

private:
	int DoesIntegerAlreadyAppear( int aInteger );

	vector<int> iList;
	int iListSize;
};


#endif //__CINTEGERALLOCATOR_H__

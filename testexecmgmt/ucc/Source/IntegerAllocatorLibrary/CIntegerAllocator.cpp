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
* System Includes
*
*/



#include <assert.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CIntegerAllocator.h"


/*******************************************************************************
 *
 * Implementation
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * PUBLIC METHOD: AddToIntegerPool
 *
 ******************************************************************************/
int CIntegerAllocator::AddToIntegerPool( int aStart, int aEnd )
{
	int i, err;

	// numbers must be positive 
	if( aStart > aEnd ) {
	  return -1;
	}
	if( aStart < 0 ) {
	  return -1;
	}

	// add each integer in the range, as long as it doesn't already
	// exist in the list
	for( i = aStart; i <= aEnd; i++ ) {
		err = DoesIntegerAlreadyAppear( i );
		if( err == 0 ) {
			iList.push_back( i );
		}
	}

	// done
	return 0;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: AllocateInteger
 *
 ******************************************************************************/
int CIntegerAllocator::AllocateInteger()
{
	int rv;
	vector<int>::iterator iter;

	// check that there is a free integer
	if( iList.empty() ) {
		return -1;
	}

	// get the integer and delete it
	iter = iList.begin();
	rv = *iter;
	iList.erase( iter );

	// return the allocated integer
	return rv;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: FreeInteger
 *
 ******************************************************************************/
int CIntegerAllocator::FreeInteger( int aFreeInteger )
{
	// check that the number isn't already there
	assert( DoesIntegerAlreadyAppear(aFreeInteger) == 0 );

	// add the integer back to the list
	iList.push_back( aFreeInteger );
	return 0;
}


/*******************************************************************************
 *
 * PRIVATE METHOD: DoesIntegerAlreadyAppear
 *
 ******************************************************************************/
int CIntegerAllocator::DoesIntegerAlreadyAppear( int aInteger )
{
	vector<int>::iterator iter;

	// search the vector for a match
	for( iter = iList.begin(); iter != iList.end(); iter++ ) {
		if( *iter == aInteger ) {
			return 1;
		}
	}

	// done - not found
	return 0;
}

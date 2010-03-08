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
* CNetworkPartitionManager
*
*/




#ifndef __CNETWORKPARTITIONMANAGER_H__
#define __CNETWORKPARTITIONMANAGER_H__

/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <string>
using namespace std;

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "../IntegerAllocatorLibrary/CIntegerAllocator.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
class CNetworkPartitionManager
{
public:
	CNetworkPartitionManager();
	~CNetworkPartitionManager();

	int SetConfiguration( int aBaseNetworkAddress, int aBaseNetworkBitCount, int aSegmentSize  );
	int AllocateNetworkSegment( int *aNetmaskBitcount, int *aSegmentSize );
	int FreeNetworkSegment( int aSegmentBaseAddress );

private:
	int log2( int aValue );
	int IsPowerOfTwo( int aValue );
	int NetmaskFromBitcount( int aBitCount );

	int iBaseNetworkPartBitcount, iSegmentPartBitcount, iHostPartBitcount; 
	int iBaseNetworkNetmask, iCompleteSegmentNetmask, iSegmentMask;
	int iBaseNetworkAddress;
	int iSegmentSize, iSegmentCount;
	CIntegerAllocator iAddressAllocator;
};

#endif //__CNETWORKPARTITIONMANAGER_H__

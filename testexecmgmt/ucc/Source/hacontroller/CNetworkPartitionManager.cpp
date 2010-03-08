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
* System Includes
*
*/




#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <assert.h>
#include <math.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CNetworkPartitionManager.h"


/*******************************************************************************
 *
 * PUBLIC METHOD: CNetworkPartitionManager
 *
 ******************************************************************************/
CNetworkPartitionManager::CNetworkPartitionManager()
{
}


/*******************************************************************************
 *
 * PUBLIC METHOD: ~CNetworkPartitionManager
 *
 ******************************************************************************/
CNetworkPartitionManager::~CNetworkPartitionManager()
{
}


/*******************************************************************************
 *
 * PUBLIC METHOD: SetConfiguration
 *
 ******************************************************************************/
int CNetworkPartitionManager::SetConfiguration( int aBaseNetworkAddress, int aBaseNetworkBitCount, int aSegmentSize )
{
	int err;
	int i;

	// check the params
	assert( aBaseNetworkBitCount > 0 );
	assert( aBaseNetworkBitCount <= 31 );
	assert( aSegmentSize > 0 );

	// check that the chunk size is a power of two
	err = IsPowerOfTwo( aSegmentSize );
	if( err == 0 ) {
		return -1;
	}

	// calculate the number of bits for each part
	iBaseNetworkPartBitcount = aBaseNetworkBitCount;
	iHostPartBitcount = log2( aSegmentSize );
	iSegmentPartBitcount = 32 - iBaseNetworkPartBitcount - iHostPartBitcount;
	if( iSegmentPartBitcount < 1 ) {
		return -1;
	}
	iSegmentCount = 1<<iSegmentPartBitcount;

	// create the useful netmasks
	iBaseNetworkNetmask = NetmaskFromBitcount( iBaseNetworkPartBitcount );
	iCompleteSegmentNetmask = NetmaskFromBitcount( 32 - iHostPartBitcount );

	// save the other info
	iBaseNetworkAddress = htonl((ntohl(aBaseNetworkAddress) & iBaseNetworkNetmask));
	iSegmentSize = aSegmentSize;

	// now create the list of all available segments
	err = iAddressAllocator.AddToIntegerPool( 0, iSegmentCount-1 );
	if( err == -1 ) {
		return -1;
	}

	// create a mask for the segment bits in position
	for( iSegmentMask = 0, i = 0; i < iSegmentPartBitcount; i++ ) {
	  iSegmentMask |= 1<<(iHostPartBitcount + i);
	}


	// done
	return 0;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: AllocateNetworkChunk
 *
 ******************************************************************************/
int CNetworkPartitionManager::AllocateNetworkSegment( int *aNetmaskBitcount, int *aSegmentSize )
{
	int segment_number, complete_address, shifted_segment_number;

	// check
	assert( aSegmentSize != NULL );

	// allocate a segment number
	segment_number = iAddressAllocator.AllocateInteger();
	if( segment_number == -1 ) {
	  return -1;
	}
	

	// make a complete address for the segment number
	complete_address = ntohl( iBaseNetworkAddress );
	shifted_segment_number = (segment_number << iHostPartBitcount) & (iSegmentMask);
	complete_address |= shifted_segment_number;


	// set the return values
	complete_address = htonl( complete_address );
	*aNetmaskBitcount = iBaseNetworkPartBitcount + iSegmentPartBitcount;
	*aSegmentSize = iSegmentSize;
	assert( (*aNetmaskBitcount + log2(*aSegmentSize)) == 32 );

	// done
	return complete_address;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: FreeNetworkChunk
 *
 ******************************************************************************/
int CNetworkPartitionManager::FreeNetworkSegment( int aSegmentBaseAddress )
{
	int i, err;
	int segment_number;
	int lower_bits_segment_mask;

	// create a mask for the segment bits in the lower part of the work
	for( lower_bits_segment_mask = 0, i = 0; i < iSegmentPartBitcount; i++ ) {
		lower_bits_segment_mask |= 1<<i;
	}

	// extract the segment number
	segment_number = ntohl( aSegmentBaseAddress );
	segment_number = segment_number >> iHostPartBitcount;
	segment_number &= lower_bits_segment_mask;

	// make sure this number is in the expected range
	assert( (segment_number >= 0) && (segment_number < iSegmentCount) );

	// free the segment - this will assert that there are no duplicates
	err = iAddressAllocator.FreeInteger( segment_number );
	assert( err == 0 );

	// done
	return 0;
}


/****************************************************************************************
 * 
 * PRIVATE: log2
 * 
 ***************************************************************************************/
int CNetworkPartitionManager::log2( int aValue )
{
	double base;
	double value;
	double simple_result, integer_result;
	int rv;

	// if the value is less than zero then we have an error which I want to catch NOW
	assert( aValue >= 0 );
	
	// if the value is zero then - this is also an error?
	assert( aValue > 0 );

	// calculate the logarithm using doubles (we have to) and convert back to integer
	base = 2;
	value = (double)aValue;
	simple_result = log(value) / log(base);
	integer_result = ceil(simple_result);
	rv = (int)integer_result;

	// since this is not very nice (the need for ceil) I verify the result
	if( (1<<rv) != aValue ) {
	  fprintf( stderr, "CRITICAL ERROR: log2() made an incorrect calculation - log2(%d) => %d (%g,%g,%g,%g).\n", aValue, rv, base, value, 
		   simple_result, integer_result );
	}
	assert( (1<<rv) == aValue );

	// done - success
	return rv;
}


/****************************************************************************************
 * 
 * PRIVATE: IsPowerOfTwo
 * 
 ***************************************************************************************/
int CNetworkPartitionManager::IsPowerOfTwo( int aValue )
{
	int bit_count = 0, i;
	
	// check how many bits are set
	for( i = 0; i < 32; i++ ) {
		if( (aValue & 0x1) != 0 ) {
			bit_count++;
		}
	}

	// if more than one bit is set then this number is not a power of two
	return ((bit_count > 1) ? 0 : 1);
}


/****************************************************************************************
 * 
 * PRIVATE: NetmaskFromBitcount
 * 
 ***************************************************************************************/
int CNetworkPartitionManager::NetmaskFromBitcount( int aBitCount )
{
	int i, netmask = 0;
	for( netmask = 0, i = 0; i < aBitCount; i++ ) {
	    netmask |= 1<<(31-i);
	}
	return netmask;
}

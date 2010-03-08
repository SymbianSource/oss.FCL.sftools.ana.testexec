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



#include <stdio.h>
#include <assert.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CDatalinkPacketise.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * Constructor - init state vars
 *
 ******************************************************************************/
CDatalinkPacketise::CDatalinkPacketise( TPhoneData *aPhoneData, CLog *aLog ) : iIncomingFrame("CDatalinkPacketise::IncomingFrame"), iOutgoingFrame("CDatalinkPacketise::OutgoingFrame")
{
	// check params
	assert( aPhoneData != NULL );
	assert( aLog != NULL );

	// set state
	iAirInterface = NULL;
	iTEChannel = NULL;
	iPhoneData = aPhoneData;
	iLog = aLog;
}


CDatalinkPacketise::~CDatalinkPacketise()
{
}


/*******************************************************************************
 *
 * Access methods
 *
 ******************************************************************************/
void CDatalinkPacketise::SetAirInterface( IAirInterface *aAirInterface )
{
	iAirInterface = aAirInterface;
}

void CDatalinkPacketise::SetTEChannel( ITEChannel *aTEChannel )
{
	iTEChannel = aTEChannel;
}


/*******************************************************************************
 *
 * Process data from the TEChannel
 *
 ******************************************************************************/
TDataPathError CDatalinkPacketise::ProcessTEData( char *data, int len, int *aErrCode )
{
//	int i;
	int errcode;
//	int frame_size;
//	char *frame_pointer;
//	TFrameError ferr;
//	TFrameStatus fstatus;
	TDataPathError derr = DPE_NONE;

	// check the params
	assert( aErrCode != NULL );
	*aErrCode = 0;
		
	// check that there is an air interface - not an error if not
	if( iAirInterface == NULL ) {
		return DPE_NONE;
	}

	// do not packetise the incoming interface
	derr = iAirInterface->SendPacket( data, len, &errcode );
	if( derr != DPE_NONE ) {
		iLog->WriteLogEntry( SV_WARNING, "CDatalinkPacketise::ProcessTEData", "SendPacket returned error", derr, errcode );
	}
	return DPE_NONE;

#if 0
	// process each byte
	for( i = 0; i < len; i++ ) {

		// add the byte to the frame
		ferr = iOutgoingFrame.AddByteToFrame( data[i] );

		// if this overflows the buffer then we send whatever is in the frame and then add it again
		if( ferr == FE_OVERFLOW ) {
			frame_pointer = iOutgoingFrame.GetFrameBuffer( &frame_size );
			derr = iAirInterface->SendPacket( frame_pointer, frame_size, &errcode );
			if( derr != DPE_NONE ) {
				iLog->WriteLogEntry( SV_WARNING, "CDatalinkPacketise::ProcessTEData", "SendPacket returned error", derr, errcode );
			}
			ferr = iOutgoingFrame.ClearFrameMemoryButNotState();
			assert( ferr == FE_NONE );
			ferr = iOutgoingFrame.AddByteToFrame( data[i] );
			assert( ferr == FE_NONE );
			continue;
		}

		// if the add was successful then check to see if the frame is complete and send it if it is
		fstatus = iOutgoingFrame.GetFrameStatus();
		if( fstatus == FS_COMPLETE ) {
			frame_pointer = iOutgoingFrame.GetFrameBuffer( &frame_size );
			derr = iAirInterface->SendPacket( frame_pointer, frame_size, &errcode );
			if( derr != DPE_NONE ) {
				iLog->WriteLogEntry( SV_WARNING, "CDatalinkPacketise::ProcessTEData", "SendPacket returned error", derr, errcode );
			}
			iOutgoingFrame.ClearFrame();
		}
	}
#endif 
	// done
	return DPE_NONE;
}


/*******************************************************************************
 *
 * Process data from the UU interface
 *
 ******************************************************************************/
TDataPathError CDatalinkPacketise::ProcessUUData( char *data, int len, int *aErrCode )
{
	int i, errcode;
	int frame_size;
	char *frame_pointer;
	TFrameError ferr;
	TFrameStatus fstatus;
	TDataPathError derr = DPE_NONE;

	// check the params
	assert( aErrCode != NULL );
	*aErrCode = 0;
		
	// check that there is an air interface - it is not unexpected or problematic if the
	// channel is not defined so this is not an error.
	if( iTEChannel == NULL ) {
		return DPE_NONE;
	}

	// process each byte 
	for( i = 0; i < len; i++ ) {

		// add the byte to the frame
		ferr = iIncomingFrame.AddByteToFrame( data[i] );

		// if this overflows the buffer then we send whatever is in the frame and then add it again
		if( ferr == FE_OVERFLOW ) {
			frame_pointer = iIncomingFrame.GetFrameBuffer( &frame_size );
			derr = iTEChannel->SendPacket( frame_pointer, frame_size, &errcode );
			if( derr != DPE_NONE ) {
				iLog->WriteLogEntry( SV_WARNING, "CDatalinkPacketise::ProcessTEData", "SendPacket returned error", derr, errcode );
			}
			ferr = iIncomingFrame.ClearFrameMemoryButNotState();
			assert( ferr == FE_NONE );
			ferr = iIncomingFrame.AddByteToFrame( data[i] );
			assert( ferr == FE_NONE );
			continue;
		}

		// get the frame status -- if the frame is complete then send it
		fstatus = iIncomingFrame.GetFrameStatus();
		if( fstatus == FS_COMPLETE ) {
			frame_pointer = iIncomingFrame.GetFrameBuffer( &frame_size );
			derr = iTEChannel->SendPacket( frame_pointer, frame_size, &errcode );
			if( derr != DPE_NONE ) {
				iLog->WriteLogEntry( SV_WARNING, "CDatalinkPacketise::ProcessUUData", "SendPacket returned an error", derr, errcode );
			}
			iIncomingFrame.ClearFrame();
		}
	}

	// done
	return DPE_NONE;
}

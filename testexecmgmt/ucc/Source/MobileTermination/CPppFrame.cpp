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



#include <string.h>
#include <assert.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CPppFrame.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * PUBLIC METHODS: Constructor / Destructor
 *
 ******************************************************************************/
CPppFrame::CPppFrame( char *aFrameName, int aRemoveHDLCFraming )
{	
	int slen;

	// check params
	assert( aFrameName != NULL );
	slen = strlen(aFrameName);
	assert( slen < MAXFRAMENAME );

	// init members
	strcpy( iFrameName, aFrameName );
	iCurrentFrameSize = 0;
	iCurrentBufferSize = 0;
	iFrameBufferStatus = FS_EMPTY;
	iRemoveHDLCFraming = aRemoveHDLCFraming;
	iEscapedChar = 0;
}


CPppFrame::~CPppFrame()
{
}


/*******************************************************************************
 *
 * PUBLIC METHODS: AddByteToFrame
 *
 ******************************************************************************/
TFrameError CPppFrame::AddByteToFrame( char c )
{
	int escaped_char_flag_on_entry;

	// check state
	assert( iFrameBufferStatus != FS_COMPLETE );

	// save the escaped char flag in case we have to restore it 
	escaped_char_flag_on_entry = iEscapedChar;

	// (1)  undo hdlc frameing if configured to do so
	if( iRemoveHDLCFraming != 0 ) {

		// check to see if this is the escape char, if so we set a flag and stop processing this byte
		if( (c == 0x7D) && (iEscapedChar == 0) ) {
			iEscapedChar = 1;
			return FE_NONE;
		}

		// if this byte was escaped then we have to XOR it with 0x20, then continue processesing
		if( iEscapedChar == 1 ) {
			iEscapedChar = 0;
			c = c ^ 0x20;
		}
	}

	// sanity checks between hdlc and non
	assert( iEscapedChar == 0 );

	// (2) process the byte as normal

	// if this is the start of the frame then set the length, state, and copy the char. Note that PPP
	// only requires one frame sequence (0x7e) between two adjacent frames. We take this flag as the
	// end of the previous frame, which means that a frame may not have an openning frame sequence. The
	// upshot of all this is that any byte may start a new frame.
	if( iFrameBufferStatus == FS_EMPTY ) {
	  assert( iCurrentFrameSize == 0 );
	  assert( iCurrentBufferSize == 0 );
	  (iFrameBuffer)[0] = c;
	  iCurrentFrameSize = iCurrentBufferSize = 1;
	  iFrameBufferStatus = FS_IN_PROGRESS;
	  return FE_NONE;
	} 

	// If we are in a frame then buffer the byte. If there is no room then discard the byte and return 
	// a warning. One byte is always kept for the end of frame sequence.
	if( (iFrameBufferStatus == FS_IN_PROGRESS) && (c != 0x7E) ) {
	  if( iCurrentBufferSize < (MAXFRAMESIZE-1) ) {
	    (iFrameBuffer)[iCurrentBufferSize] = c;
		iCurrentFrameSize++;
		iCurrentBufferSize++;
		return FE_NONE;
	  } else {
	    iEscapedChar = escaped_char_flag_on_entry;
		return FE_OVERFLOW;
	  }
	}

	// If we are in a frame and this is then buffer it and update the state
	if( (iFrameBufferStatus == FS_IN_PROGRESS) && (c == 0x7E) ) {

		// save the final byte
		assert( iCurrentBufferSize < MAXFRAMESIZE );
		(iFrameBuffer)[iCurrentBufferSize] = c;
		iCurrentFrameSize++;
		iCurrentBufferSize++;

		// update the state
		iFrameBufferStatus = FS_COMPLETE;
		return FE_NONE;
	}

	// done (just to keep compiler happy)
	return FE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHODS: ClearFrame
 *
 ******************************************************************************/
TFrameError CPppFrame::ClearFrame()
{
	iCurrentBufferSize = 0;
	iCurrentFrameSize = 0;
	iFrameBufferStatus = FS_EMPTY;
	return FE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC METHODS: ClearFrameMemoryButNotState
 *
 ******************************************************************************/
TFrameError CPppFrame::ClearFrameMemoryButNotState()
{
	iCurrentBufferSize = 0;
	return FE_NONE;
}

/*******************************************************************************
 *
 * PUBLIC METHODS: GetFrameStatus
 *
 ******************************************************************************/
TFrameStatus CPppFrame::GetFrameStatus()
{
	return iFrameBufferStatus;
}


/*******************************************************************************
 *
 * PUBLIC METHODS: GetOverflowCount
 *
 ******************************************************************************/
int CPppFrame::GetOverflowCount()
{
	return 0;
}


/*******************************************************************************
 *
 * PUBLIC METHODS: GetFrameBuffer
 *
 ******************************************************************************/
char *CPppFrame::GetFrameBuffer( int *aLen )
{
	assert( aLen != NULL );
	*aLen = iCurrentBufferSize;
	return (char*)iFrameBuffer;
}


/*******************************************************************************
 *
 * PUBLIC METHODS: GetFrameName
 *
 ******************************************************************************/
char *CPppFrame::GetFrameName()
{
	return iFrameName;
}

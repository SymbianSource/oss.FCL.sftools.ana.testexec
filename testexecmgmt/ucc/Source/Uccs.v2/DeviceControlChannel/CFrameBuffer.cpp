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
#include <string.h>

/**********************************************************************************************
 *
 * Local Includes
 *
 *********************************************************************************************/
#include "CFrameBuffer.h"

/**********************************************************************************************
 *
 * Defines
 *
 *********************************************************************************************/

/**********************************************************************************************
 *
 * Static Method
 *
 *********************************************************************************************/
TFBError CFrameBuffer::createSendBuffer(int aUID, TPCommand aSendCommand, int aDataLength, char* aData, char* aFrame, int* aFrameSize)
{
	int totalFrameSize, i, data_base_index;
	TPHeader *header;

	// Check params
	assert ( aData != NULL);
	assert ( aFrame != NULL);
	assert ( aFrameSize != NULL);

	// Calculate the size of the resultant frame buffer
	totalFrameSize = ( sizeof(TPHeader) + aDataLength);

	// Check that the frame data will fit into the allocated frame size 
	if ( totalFrameSize > *aFrameSize )
	{
		return TFB_BUFFERTOOSMALL;
	}

	header = (TPHeader*)aFrame;

	// Now create the packet											
	header->iUid = aUID;					// Add the UID
	header->iCmdID = aSendCommand;			// Add the command id
	header->iDataLen = aDataLength;			// Add the length

	for( i = 0, data_base_index = sizeof(TPHeader); i < aDataLength; i++ ) 
	{
		aFrame[data_base_index + i] = aData[i];
	}

	// Set the final frame size to the actual size of the frame we have created.
	*aFrameSize = totalFrameSize;

	return TFB_SUCCESS;
}


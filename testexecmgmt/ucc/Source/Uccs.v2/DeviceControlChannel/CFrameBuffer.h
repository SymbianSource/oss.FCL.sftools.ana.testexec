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
* CFrameBuffer Class
*
*/



#ifndef _CFrameBuffer_H
#define _CFrameBuffer_H

/*****************************************************************************
 *
 * System Includes 
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/
#include "CProtocolTypes.h"

/*****************************************************************************
 *
 *  Types
 *
 ****************************************************************************/
typedef enum {
	TFB_SUCCESS,
	TFB_INVALIDARGS,
	TFB_BUFFERTOOSMALL
} TFBError;

/*****************************************************************************
 *
 *  Defines
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * Class Definition
 *
 ****************************************************************************/
class CFrameBuffer
{
public:

	static TFBError createSendBuffer (int aUID, TPCommand aSendCommand, int aDataLength, char* aData, char* aFrame, int* aFrameSize);
};

#endif

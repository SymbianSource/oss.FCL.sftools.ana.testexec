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



#ifndef __CPPPFRAME_H__
#define __CPPPFRAME_H__

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAXFRAMESIZE (1024*4)
#define MAXFRAMENAME (128)

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum {
	FS_EMPTY,
	FS_IN_PROGRESS,
	FS_COMPLETE
} TFrameStatus;

typedef enum {
	FE_NONE,
	FE_OVERFLOW
} TFrameError;

/*******************************************************************************
 *
 * class CPppFrame
 *
 ******************************************************************************/
class CPppFrame
{
public:
	CPppFrame( char *aFrameName, int aRemoveHDLCFraming = 0 );
	~CPppFrame();

	TFrameError AddByteToFrame( char c );
	TFrameError ClearFrame();
	TFrameError ClearFrameMemoryButNotState();

	TFrameStatus GetFrameStatus();
	int GetOverflowCount();
	char *GetFrameBuffer( int *aLen );
	char *GetFrameName();

private:
	char iFrameName[MAXFRAMENAME];
	int iRemoveHDLCFraming;

	int iEscapedChar;

	char iFrameBuffer[MAXFRAMESIZE];
	int iCurrentFrameSize;
	int iOverflowByteCount;
	int iCurrentBufferSize;
	TFrameStatus iFrameBufferStatus;
};

#endif //__CPPPFRAME_H__

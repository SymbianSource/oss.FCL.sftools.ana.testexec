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
* Switches
*
*/



#ifndef __FILE_UTILITIES_H__
#define __FILE_UTILITIES_H__


/******************************************************************************************************
 *
 * System Includes
 *
 *****************************************************************************************************/
#include <string>
using namespace std;


/******************************************************************************************************
 *
 * Types
 *
 *****************************************************************************************************/
typedef enum {
  FUE_NONE,
  FUE_READ_ERROR,
  FUE_WRITE_ERROR,
  FUE_OPEN_ERROR,
  FUE_OPEN_TEMP_ERROR,
  FUE_OPEN_SOURCE_ERROR,
  FUE_RENAME_ERROR,
  FUE_END_OF_FILE,
} TFUError;


/******************************************************************************************************
 *
 * Prototypes
 *
 *****************************************************************************************************/
FILE *OpenTempfile( char *aFilenamePrefix, char *aFilename, int aLen, char *aMode, int *aSystemError );
TFUError CopyFileByPtr( FILE *aSource, FILE *aDest, int *aSystemError );
TFUError CopyFileByName( char *aSource, char *aDest, int *aSystemError );
TFUError GetLine( char *aFilename, int aLineNumber, char *aBuffer, int aBufferSize, int *aSystemError );
TFUError InsertLine( char *aFilename, int aLineNumber, char *aLine, int aLineSize, int aReplaceFlag, int *aSystemError );
TFUError FindMatchingLines( char *aFilename, char *aToken, int *aLineList, int *aListSize, int *aSystemError );
TFUError FindMatchingLinesByRawPrefix( char *aFilename, char *aToken, int *aLineList, int *aListSize, int *aSystemError );
TFUError GetFileLineCount( char *aFilename, int *aLineCount, int *aSystemError );

#endif

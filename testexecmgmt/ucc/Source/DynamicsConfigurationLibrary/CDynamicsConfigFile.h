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



#ifndef __CDYNAMICSCONFIGFILE_H__
#define __CDYNAMICSCONFIGFILE_H__

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
  // These values must precisely match FUE_ errors
  DCE_NONE,
  DCE_READ_ERROR,
  DCE_WRITE_ERROR,
  DCE_OPEN_ERROR,
  DCE_OPEN_TEMP_ERROR,
  DCE_OPEN_SOURCE_ERROR,			// 5
  DCE_RENAME_ERROR,
  DCE_END_OF_FILE,

  // These are specific errors
  DCE_INVALID_STATE,
  DCE_COPY_ERROR,
  DCE_MULTIPLE_MATCHES,				// 10
  DCE_START_TAG_NOT_FOUND,
  DCE_MULTIPLE_START_TAGS_FOUND,
  DCE_END_TAG_NOT_FOUND,
  DCE_MULTIPLE_END_TAGS_FOUND,		
  DCE_ENTRY_NOT_FOUND,                          // 15
} TDCFError;

typedef enum {
  DCS_INIT,
  DCS_REFERENCE_FILE_SET,
  DCS_ACTIVE_LOCAL_COPY,
} TDCFStatus;


/******************************************************************************************************
 *
 * class CDynamicsConfigFile
 *
 *****************************************************************************************************/
class CDynamicsConfigFile
{
public:
  CDynamicsConfigFile();
  ~CDynamicsConfigFile();

  // Management
  TDCFError SetReferenceFile( string aReferenceFile );
  TDCFError CreateLocalCopy( int *aSystemError );
  TDCFError RemoveLocalCopy( int *aSystemError );

  // Les fonctionnaires
  TDCFError SetSingleOption( string aLabel, string aValue, int *aSystemError );
  TDCFError RemoveSingleOption( string aLabel, int *aSystemError );
  TDCFError AddListOption( string aBlockStartTag, string aBlockEndTag, string aLineItem, int *aSystemError );
  TDCFError RemoveListOption( string aBlockStartTag, string aBlockEndTag, string aFirstToken, int *aSystemError );
  const char *GetLocalFilename();

private:

  // Stateless helpers
  TDCFError GenerateLocalFilenameTemplate( char *aBuffer, int aSize );

  // Members
  string iReferenceFilename;
  string iLocalFilename;
  TDCFStatus iStatus;
};

#endif //__CDYNAMICSCONFIGFILE_H__

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
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <errno.h>
#include <assert.h>


/******************************************************************************************************
 *
 * Local Includes
 *
 *****************************************************************************************************/
#include "file_utilities.h"
#include "CDynamicsConfigFile.h"


/******************************************************************************************************
 *
 * Definition
 *
 *****************************************************************************************************/
#define MAXFILENAMESIZE      128
#define MAXLINEPOSITION      99999
#define MAXTOKENMATCHES      16
#define MAXLINESIZE          2048
#ifdef WIN32
#define snprintf             _snprintf
#endif

/******************************************************************************************************
 *
 * PUBLIC METHOD: Constructors
 *
 *****************************************************************************************************/
CDynamicsConfigFile::CDynamicsConfigFile()
{
  iStatus = DCS_INIT;
  iReferenceFilename = "";
  iLocalFilename = "";
}


CDynamicsConfigFile::~CDynamicsConfigFile()
{
  assert( iStatus != DCS_ACTIVE_LOCAL_COPY );
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: SetReferenceFile
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::SetReferenceFile( string aReferenceFile )
{
  // check that we don't have an active local copy
  if( iStatus == DCS_ACTIVE_LOCAL_COPY ) {
    return DCE_INVALID_STATE;
  }

  // set the reference file
  iReferenceFilename = aReferenceFile;
  iStatus = DCS_REFERENCE_FILE_SET;
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: CreateLocalCopy
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::CreateLocalCopy( int *aSystemError )
{
  TFUError ferr;
  FILE *fp_dest, *fp_source;
  char copy_filename[MAXFILENAMESIZE];

  // check params
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // check the state
  if( iStatus != DCS_REFERENCE_FILE_SET ) {
    return DCE_INVALID_STATE;
  }

  // create a tempfile to hold the local copy
  fp_dest = OpenTempfile( (char*)iReferenceFilename.c_str(), copy_filename, MAXFILENAMESIZE, "w", aSystemError );
  if( fp_dest == NULL ) {
    return DCE_OPEN_TEMP_ERROR;
  }

  // open the source file
  fp_source = fopen( (char*)iReferenceFilename.c_str(), "r" );
  if( fp_source == NULL ) {
    fclose( fp_dest );
	unlink( copy_filename );
    *aSystemError = errno;
    return DCE_OPEN_ERROR;
  }

  // copy the files, close them, and look for errors
  ferr = CopyFileByPtr( fp_source, fp_dest, aSystemError );
  fflush( fp_source );
  fflush( fp_dest );
  fclose( fp_source );
  fclose( fp_dest );
  if( ferr != FUE_NONE ) {
  	unlink( copy_filename );
    return (TDCFError)ferr;
  }

  // we have been successful - update the state
  iLocalFilename = copy_filename;
  iStatus = DCS_ACTIVE_LOCAL_COPY;
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: RemoveLocalCopy
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::RemoveLocalCopy( int *aSystemError )
{
  // check params
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // check the state
  if( iStatus != DCS_ACTIVE_LOCAL_COPY ) {
    return DCE_INVALID_STATE;
  }

  // remove the local copy
  unlink( iLocalFilename.c_str() );
  iStatus = DCS_REFERENCE_FILE_SET;
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: SetSingleOption
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::SetSingleOption( string aLabel, string aValue, int *aSystemError )
{
  TFUError ferr;
  int line_numbers[MAXTOKENMATCHES], match_count, insert_line, replace_flag = 1;

  // check params
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // check the state
  if( iStatus != DCS_ACTIVE_LOCAL_COPY ) {
    return DCE_INVALID_STATE;
  }

  // get the list of occurences of the label
  match_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aLabel.c_str(), line_numbers, &match_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // make sure there is one or zero occurences
  if( match_count > 1 ) {
    return DCE_MULTIPLE_MATCHES;
  }

  // set the line to insert at -- if zero occurences then add at the start of the file and DONT REPLACE
  insert_line = line_numbers[0];
  if( match_count == 0 ) {
    insert_line = 1;
    replace_flag = 0;
  }

  // insert the line
  ferr = InsertLine( (char*)iLocalFilename.c_str(), insert_line, (char*)aValue.c_str(), aValue.length(), replace_flag, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }
  
  // done
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: AddListOption
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::AddListOption( string aBlockStartTag, string aBlockEndTag, string aLineItem, int *aSystemError )
{
  TFUError ferr;
  int start_block_tag[MAXTOKENMATCHES], end_block_tag[MAXTOKENMATCHES], start_block_count, end_block_count;

  // check params
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // check the state
  if( iStatus != DCS_ACTIVE_LOCAL_COPY ) {
    return DCE_INVALID_STATE;
  }

  // count the number of times the block start label appears in the file -- must 1 here
  start_block_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aBlockStartTag.c_str(), start_block_tag, &start_block_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // count the number of times the block end label appears in the file -- must 1 here
  end_block_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aBlockEndTag.c_str(), end_block_tag, &end_block_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // make sure there is exactly one of each tag
  if( start_block_count == 0 ) {
    return DCE_START_TAG_NOT_FOUND;
  }
  if( start_block_count > 1 ) {
    return DCE_MULTIPLE_START_TAGS_FOUND;
  }
  if( end_block_count == 0 ) {
    return DCE_END_TAG_NOT_FOUND;
  }
  if( end_block_count > 1 ) {
    return DCE_MULTIPLE_END_TAGS_FOUND;
  }

  // ok - insert the line before the end tag  
  ferr = InsertLine( (char*)iLocalFilename.c_str(), end_block_tag[0], (char*)aLineItem.c_str(), aLineItem.length(), 0, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }
  
  // done
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: RemoveListOption
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::RemoveListOption( string aBlockStartTag, string aBlockEndTag, string aFirstToken, int *aSystemError )
{
  TFUError ferr;
  int start_block_tag[MAXTOKENMATCHES], end_block_tag[MAXTOKENMATCHES], entries[MAXTOKENMATCHES], start_block_count, end_block_count, entry_count;
  int i;

  // check params
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // check the state
  if( iStatus != DCS_ACTIVE_LOCAL_COPY ) {
    return DCE_INVALID_STATE;
  }

  // count the number of times the block start label appears in the file -- must 1 here
  start_block_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aBlockStartTag.c_str(), start_block_tag, &start_block_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // count the number of times the block end label appears in the file -- must 1 here
  end_block_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aBlockEndTag.c_str(), end_block_tag, &end_block_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // make sure there is exactly one of each tag
  if( start_block_count <= 0 ) {
    return DCE_START_TAG_NOT_FOUND;
  } else if( end_block_count <= 0 ) {
    return DCE_END_TAG_NOT_FOUND;
  } else if( start_block_count > 1 ) {
    return DCE_MULTIPLE_START_TAGS_FOUND;
  } else if( end_block_count > 1 ) {
    return DCE_MULTIPLE_END_TAGS_FOUND;
  }
  assert( (start_block_count == 1) && (end_block_count == 1) );

  // count the number of lines matching the token
  entry_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aFirstToken.c_str(), entries, &entry_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // now look for the first entry that is between the tags
  for( i = 0; i < entry_count; i++ ) {
    if( (entries[i] >= start_block_tag[0]) && (entries[i] <= end_block_tag[0]) )
      break;
  }
			
  // if no match was found between the tags then exit
  if( i == entry_count ) {
    return DCE_ENTRY_NOT_FOUND;
  }

  // remove the line
  ferr = InsertLine( (char*)iLocalFilename.c_str(), entries[i], NULL, 0, 1, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }
  
  // done
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: RemoveListOption
 *
 *****************************************************************************************************/
TDCFError CDynamicsConfigFile::RemoveSingleOption( string aLabel, int *aSystemError )
{
  TFUError ferr;
  int line_numbers[MAXTOKENMATCHES], match_count, insert_line;
  char nline[MAXLINESIZE];

  // check params
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // check the state
  if( iStatus != DCS_ACTIVE_LOCAL_COPY ) {
    return DCE_INVALID_STATE;
  }

  // get the list of occurences of the label
  match_count = MAXTOKENMATCHES;
  ferr = FindMatchingLines( (char*)iLocalFilename.c_str(), (char*)aLabel.c_str(), line_numbers, &match_count, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }

  // make sure there is one or zero occurences
  if( match_count > 1 ) {
    return DCE_MULTIPLE_MATCHES;
  }

  // if there is zero then there is nothing to set
  if( match_count == 0 ) {
	  return DCE_NONE;
  }

  // otherwise replace the line with the string '# aLabel'
  assert( match_count == 1 );
  insert_line = line_numbers[0];
  snprintf( nline, MAXLINESIZE, "# %s", aLabel.c_str() );

  // insert the line
  ferr = InsertLine( (char*)iLocalFilename.c_str(), insert_line, nline, strlen(nline), 1, aSystemError );
  if( ferr != FUE_NONE ) {
    return (TDCFError)ferr;
  }
  
  // done
  return DCE_NONE;
}


/******************************************************************************************************
 *
 * PUBLIC METHOD: GetLocalFilename
 *
 *****************************************************************************************************/
const char *CDynamicsConfigFile::GetLocalFilename()
{
	return iLocalFilename.c_str();
}

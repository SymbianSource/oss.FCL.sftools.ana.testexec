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
#include <errno.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <assert.h>
 

/******************************************************************************************************
 *
 * Local Includes
 *
 *****************************************************************************************************/
#include "file_utilities.h"


/******************************************************************************************************
 *
 * Definitions
 *
 *****************************************************************************************************/
#define TEMPFILE_SUFFIX               "XXXXXX"
#define MAXLINESIZE                    2048
#define COPYBUFFER                     2048
#define TEMPFILENAMESIZE               128
#define INSERT_TEMPFILE_PREFIX         "insert_tempfile."

#define MIN(a,b)                       (((a)<(b))?a:b)


/******************************************************************************************************
 *
 * Prototypes
 *
 *****************************************************************************************************/
#ifdef WIN32
static int mkstemp( char *aString );
#endif


/******************************************************************************************************
 *
 * PRIVATE METHOD: OpenTempfile
 *
 *****************************************************************************************************/
FILE *OpenTempfile( char *aFilenamePrefix, char *aFilename, int aLen, char *aMode, int *aSystemError )
{
  int err, prefix_len;
  FILE *fp;

  // check params
  assert( aFilenamePrefix != NULL );
  assert( aFilename != NULL );
  assert( aSystemError != NULL );
  assert( aLen > (strlen(TEMPFILE_SUFFIX) + 1) );
  *aSystemError = 0;

  // copy the prefix into the buffer
  prefix_len = MIN( strlen(aFilenamePrefix), (aLen - strlen(TEMPFILE_SUFFIX) - 1) );
  memcpy( aFilename, aFilenamePrefix, prefix_len );

  // copy the template into the buffer and NULL terminate
  memcpy( &(aFilename[prefix_len]), TEMPFILE_SUFFIX, strlen(TEMPFILE_SUFFIX) );
  aFilename[prefix_len + strlen(TEMPFILE_SUFFIX)] = 0;
  
  // get the filename
  err = mkstemp( aFilename );
  if( err == -1 ) { 
    *aSystemError = errno;
    return NULL;
  }

  // now open the file
  fp = fopen( aFilename, aMode );
  if( fp == NULL ) {
    *aSystemError = errno;
  }
  return fp;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: CopyFileByPtr
 *
 *****************************************************************************************************/
TFUError CopyFileByPtr( FILE *aSource, FILE *aDest, int *aSystemError )
{
  char buff[COPYBUFFER];
  int bytes_read, bytes_written;

  // check params
  assert( aSource != NULL );
  assert( aDest != NULL );
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // now copy all the bytes
  while( 1 ) {

    // read from the source
    bytes_read = fread( buff, 1, COPYBUFFER, aSource );
    assert( bytes_read >= 0 );

    // check for read errors 
    if( ferror(aSource) != 0 ) {
      *aSystemError = errno;
      return FUE_READ_ERROR;
    }

    // if some bytes were read then write to the destination
    bytes_written = fwrite( buff, 1, bytes_read, aDest );
    assert( bytes_written >= 0 );

    // check for write errors
    if( bytes_written != bytes_read ) {
      *aSystemError = errno;
      return FUE_WRITE_ERROR;
    }

    // check for eof 
    if( feof(aSource) != 0 ) {
      return FUE_NONE;
    }
  }

  // done
  assert( !"INVALID CODE PATH" );
  return FUE_NONE;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: CopyFileByName
 *
 *****************************************************************************************************/
TFUError CopyFileByName( char *aSource, char *aDest, int *aSystemError )
{
  FILE *fpsrc, *fpdst;
  TFUError err;

  // check params
  assert( aSource != NULL );
  assert( aDest != NULL );
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // open the source filename
  fpsrc = fopen( aSource, "r" );
  if( fpsrc == NULL ) {
    *aSystemError = errno;
    return FUE_OPEN_ERROR;
  }

  // open the dest filename
  fpdst = fopen( aDest, "w" );
  if( fpdst == NULL ) {
    fclose( fpsrc );
    *aSystemError = errno;
    return FUE_OPEN_ERROR;
  }
  
  // copy the files
  err = CopyFileByPtr( fpsrc, fpdst, aSystemError );

  // close the files
  fflush( fpsrc );
  fflush( fpdst );
  fclose( fpsrc );
  fclose( fpdst );

  // if the copy failed then remove the new file
  if( err != FUE_NONE ) {
    unlink( aDest );
  }

  // done
  return err;      
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: InsertLine
 *
 *****************************************************************************************************/
TFUError InsertLine( char *aFilename, int aLineNumber, char *aLine, int aLineSize, int aReplaceFlag, int *aSystemError )
{
  char buff[COPYBUFFER], tempfilename[TEMPFILENAMESIZE];
  int i, end_of_file, err, insert_line_logic, line_inserted = 0;
  TFUError rv;
  FILE *fp_dest, *fp_source;

  // check params
  assert( aFilename != NULL );
  assert( (aLine != NULL) || (aLineSize == 0) );
  assert( aSystemError != NULL );
  assert( aLineNumber > 0 );
  *aSystemError = 0;

  // open the file
  fp_source = fopen( aFilename, "r" );
  if( fp_source == NULL ) {
    *aSystemError = errno;
    return FUE_OPEN_SOURCE_ERROR;
  }

  // now open the tempfile
  fp_dest = OpenTempfile( INSERT_TEMPFILE_PREFIX, tempfilename, TEMPFILENAMESIZE, "w", aSystemError );
  if( fp_dest == NULL ) {
    fclose( fp_source );
    return FUE_OPEN_TEMP_ERROR;
  }

  // now copy all files until we get to the target line - add the new line - then keep going
  for( i = 1; ; i++ ) {
    
    // read the next line from the source file 
    fgets( buff, COPYBUFFER, fp_source );

    // check for errors
    if( ferror(fp_source) != 0 ) {
      *aSystemError = errno;
      rv = FUE_READ_ERROR;
      break;
    }

    // see if we want to insert the newline at this point: we do if the user specified to insert before 
    // this line OR if we have hit the end of the file. In both cases there must be something to insert.
    end_of_file = feof( fp_source );
    insert_line_logic = ((aLineNumber == i) || ((line_inserted == 0)&&(end_of_file != 0)));
    if( (insert_line_logic != 0) && (aLineSize > 0) ) {
      fwrite( aLine, 1, aLineSize, fp_dest );
      if( ferror(fp_dest) != 0 ) {
	*aSystemError = errno;
	rv = FUE_WRITE_ERROR;
	break;
      }
      fputc( '\n', fp_dest );
      line_inserted = 1;
    }
    
    // if we have hit the end of the file then we are done
    if( end_of_file != 0 ) {
      rv = FUE_NONE;
      break;
    }

    // write the line just read -- unless we just inserted a line and the replace flag is set
    if( (insert_line_logic == 0) || (aReplaceFlag == 0) ) {
      fwrite( buff, 1, strlen(buff), fp_dest );
      if( ferror(fp_dest) != 0 ) {
		*aSystemError = errno;
		rv = FUE_WRITE_ERROR;
		break;
      }
    }
    
    // go to next line
  }

  // close both files
  fflush( fp_source );
  fflush( fp_dest );
  fclose( fp_source );
  fclose( fp_dest );

  // if we were successful then remove the original file and rename the new file to the original 
  // filename -- otherwise just remove the tempfile
  if( rv == FUE_NONE ) {
    unlink( aFilename );
    err = rename( tempfilename, aFilename );
    if( err != 0 ) {
      *aSystemError = errno;
      rv = FUE_RENAME_ERROR;
      unlink( tempfilename );
    }
  } else {
    unlink( tempfilename );
  }
      
  // done
  return rv;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: GetLine
 *
 *****************************************************************************************************/
TFUError GetLine( char *aFilename, int aLineNumber, char *aBuffer, int aBufferSize, int *aSystemError )
{
  FILE *fp;
  char *cline;
  int i;

  // check params
  assert( aLineNumber >= 0 );
  assert( aBuffer != NULL );
  assert( aSystemError != NULL );
  *aSystemError = 0;

  // open the file
  fp = fopen( aFilename, "r" );
  if( fp == NULL ) {
    *aSystemError = errno;
    return FUE_OPEN_ERROR;
  }

  // read through the file until finding the appropriate line
  for( i = 0; i < aLineNumber; i++ ) {
    cline = fgets( aBuffer, aBufferSize, fp );
    if( cline == NULL )
      break;
  }

  // done with the file
  fflush( fp );
  fclose( fp );

  // if cline is null then we reached the end-of-file
  if( cline == NULL ) {
    return FUE_END_OF_FILE;
  }

  // otherwise just return -- the line is in the buffer
  return FUE_NONE;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: FindMatchingLines
 *
 *****************************************************************************************************/
TFUError FindMatchingLines( char *aFilename, char *aToken, int *aLineList, int *aListSize, int *aSystemError )
{
  int list_capacity, i, match;
  char cline[MAXLINESIZE], *cptr, *token_start, *token_end;
  FILE *fp;
  TFUError rv;

  // check params
  assert( aFilename != NULL );
  assert( aToken != NULL );
  assert( aLineList != NULL );
  assert( aListSize != NULL );
  assert( aSystemError != NULL );
  list_capacity = *aListSize;
  *aListSize = 0;
  *aSystemError = 0;

  // open the file
  fp = fopen( aFilename, "r" );
  if( fp == NULL ) {
    *aSystemError = errno;
    return FUE_OPEN_ERROR;
  }

  // now read through the line and look for a line starting with the passed token (after whitespace and '#')
  for( i = 1; ; i++ ) {
    
    // read the next line
    cptr = fgets( cline, MAXLINESIZE, fp );
    
    // check for errors
    if( ferror(fp) != 0 ) {
      *aSystemError = errno;
      rv = FUE_READ_ERROR;
      break;
    }

    // check for end of file
    if( cptr == NULL ) {
      rv =  FUE_NONE;
      break;
    }

    // remove any whitespace (' ' + '\t') and hashes to find the start of the token
    for( token_start = cline; (*token_start == ' ') || (*token_start == '\t') || (*token_start == '#'); token_start++ )
      ;

    // now find the end of the token
    for( token_end = token_start; (*token_end != ' ') && (*token_end != '\t') && (*token_end != '\n') && (*token_end != 0); token_end++ ) 
      ;

    // null terminate the token - no need to save the old value
    *token_end = 0;

    // now compare the tokens and add the match
    match = strcmp( token_start, aToken );
    if( (match == 0) && ((*aListSize) < list_capacity) ) {
		aLineList[(*aListSize)++] = i;
    }
	
    // and do it all again
  }

  // close up the file and return
  fflush( fp );
  fclose( fp );

  // done
  return rv;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: FindMatchingLinesByRawPrefix
 *
 *****************************************************************************************************/
TFUError FindMatchingLinesByRawPrefix( char *aFilename, char *aToken, int *aLineList, int *aListSize, int *aSystemError )
{
  int list_capacity, i, match, token_length;
  char cline[MAXLINESIZE], *cptr;
  FILE *fp;
  TFUError rv;

  // check params
  assert( aFilename != NULL );
  assert( aToken != NULL );
  assert( aLineList != NULL );
  assert( aListSize != NULL );
  assert( aSystemError != NULL );
  list_capacity = *aListSize;
  *aListSize = 0;
  *aSystemError = 0;

  // get the length of the passed token
  token_length = strlen( aToken );
  assert( token_length > 0 );

  // open the file
  fp = fopen( aFilename, "r" );
  if( fp == NULL ) {
    *aSystemError = errno;
    return FUE_OPEN_ERROR;
  }

  // now read through the line and look for a line starting with the passed token
  for( i = 1; ; i++ ) {
    
    // read the next line
    cptr = fgets( cline, MAXLINESIZE, fp );
    
    // check for errors
    if( ferror(fp) != 0 ) {
      *aSystemError = errno;
      rv = FUE_READ_ERROR;
      break;
    }

    // check for end of file
    if( cptr == NULL ) {
      rv =  FUE_NONE;
      break;
    }


    // now compare the tokens and add the match
    match = strncmp( cptr, aToken, token_length );
    if( (match == 0) && ((*aListSize) < list_capacity) ) {
		aLineList[(*aListSize)++] = i;
    }
	
    // and do it all again
  }

  // close up the file and return
  fflush( fp );
  fclose( fp );

  // done
  return rv;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: GetFileLineCount
 *
 *****************************************************************************************************/
TFUError GetFileLineCount( char *aFilename, int *aLineCount, int *aSystemError )
{
  FILE *fp;
  char cline[MAXLINESIZE], *cptr;

  // check params
  assert( aFilename != NULL );
  assert( aLineCount != NULL );
  assert( aSystemError != NULL );
  *aLineCount = *aSystemError = 0;

  // open the file
  fp = fopen( aFilename, "r" );
  if( fp == NULL ) {
    *aSystemError = errno;
    return FUE_OPEN_ERROR;
  }

  // read through the file and count the number of lines
  while( 1 ) {
    cptr = fgets( cline, MAXLINESIZE, fp );
    if( cptr == NULL )
      break;
    (*aLineCount)++;
  }

  // done with the file
  fclose( fp );

  // otherwise just return -- the line is in the buffer
  return FUE_NONE;
}


/******************************************************************************************************
 *
 * PRIVATE METHOD: mkstemp
 *
 *****************************************************************************************************/
#ifdef WIN32
int mkstemp( char *aString )
{
	char *ptr;
	ptr = _mktemp( aString );
	if( ptr == NULL ) {
		return -1;
	}
	return 0;
}
#endif

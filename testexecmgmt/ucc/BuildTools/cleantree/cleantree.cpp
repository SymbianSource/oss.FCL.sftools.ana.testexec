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
* System Include
*
*/



#include <stdio.h>
#include <windows.h>
#include <direct.h>
#include <assert.h>

/*******************************************************************************
 *
 * Local Include
 *
 ******************************************************************************/
#include "cleantree.h"

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAXPATHLEN	1024

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef void	(*TProcessFunction)(char*,int); 

/*******************************************************************************
 *
 * Prototypes
 *
 ******************************************************************************/
int ProcessDirectoryTree( char *path, int depth, TProcessFunction func );
int UpdateWorkingDirectory( char *previous, char *next, char *path, int op );
void PrintPath( char *str, int depth );
void ProcessFile( char *filename, int depth );

/*******************************************************************************
 *
 * Filescope statics
 *
 ******************************************************************************/
static int debug_flag = 0;
static char *dirname = NULL;
static int deleted_file_count;
static int deleted_directory_count;

/*******************************************************************************
 *
 * Main
 *
 ******************************************************************************/
int main( int argc, char *argv[] )
{
	char fname[1024];
	int slen;

	// check params
	if( argc < 3 ) {
		fprintf( stderr, "usage: cleantree path dirname\n" );
		return -1;
	}
	dirname = argv[2];

	// get the name into the correct format
	strcpy( fname, argv[1] );
	slen = strlen(fname);
	if( (fname[slen-1] != '*') || (fname[slen-2] != '\\') ) {
		fprintf( stderr, "ERROR: pathname must end in '\\*'.\n" );
		return -1;
	}

	// process the directory tree 
	ProcessDirectoryTree( argv[1], 0, ProcessFile );

	// print stats
	printf( "%d files deleted from %d directories\n", deleted_file_count, deleted_directory_count );

	// done
	fflush( stdout );
	return 0;
}

/*******************************************************************************
 *
 * ProcessFile
 *
 ******************************************************************************/
void ProcessFile( char *filename, int depth )
{
	if( debug_flag > 0 ) {
		unlink( filename );
		deleted_file_count++;
	}
}

/*******************************************************************************
 *
 * ProcessDirectoryTree
 *
 ******************************************************************************/
int ProcessDirectoryTree( char *path, int depth, TProcessFunction func )
{
	WIN32_FIND_DATA finfo;
	HANDLE list;
	int plen;
	int err = 1;
	int match;
	char previous_working_directory[MAXPATHLEN];
	char new_working_directory[MAXPATHLEN];
	char fullname[MAXPATHLEN];

	// ignore . and ..
	if( path[0] == '.' ) {
		return 0;
	}

	// ignore any dir beginning with _
	if( path[0] == '_' ) {
		return 0;
	}

	// print the path
//	PrintPath( path, (depth - 1));

	// check if we are in a debug directory
	plen = strlen( path );
	assert( path[plen-1] == '*' );
	assert( path[plen-2] == '\\' );
	path[plen-2] = 0;
	match = strcmp( path, dirname );
	if( match == 0 ) {
		debug_flag++;
	}
	path[plen-2] = '\\';
	
	// get the first file in the directory
	list = FindFirstFile( path, &finfo );
	if( list == INVALID_HANDLE_VALUE ) {
		return 0;
	}

	// update the current working directory
	UpdateWorkingDirectory( previous_working_directory, new_working_directory, path, ((depth == 0) ? 1 : 0) );

	// loop through the rest of the files
	for( err = 1; err != 0; err ) {

		// process the file
		if( finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			strcat( finfo.cFileName, "\\*" );
			ProcessDirectoryTree( finfo.cFileName, depth + 1, func );
		} else {
			strcpy( fullname, new_working_directory );
			strcat( fullname, "\\" );
			strcat( fullname, finfo.cFileName );
			func( fullname, depth );
		}

		// get the info for the next file
		err = FindNextFile( list, &finfo );
	}

	// update the working directory and close the directory
	UpdateWorkingDirectory( previous_working_directory, new_working_directory, path, 2 );
	FindClose( list );

	// now delete the directory
	if( match == 0 ) {
		err = RemoveDirectory( new_working_directory );
		if( err == 0 ) {
			fprintf( stderr, "ERROR: failed to remove directory (%d)\n", GetLastError() );
		}
		deleted_directory_count++;
		debug_flag--;
	}

	return 0;
}


/*******************************************************************************
 *
 * UpdateWorkingDirectory
 *
 ******************************************************************************/
int UpdateWorkingDirectory( char *previous, char *next, char *path, int op )
{
	int len;

	// op zero means normal PUSH of the working directory
	if( op == 0 ) {
		getcwd( previous, MAXPATHLEN );
		getcwd( next, MAXPATHLEN );
		len = strlen( next );
		next[len] = '\\';
		next[len+1] = 0;
		strcat( next, path );
		len = strlen(next);
		next[len-2] = 0;
		chdir( next );
	} 

	// op one is initial set of the dirs
	if( op == 1 ) {
		getcwd( previous, MAXPATHLEN );
		strcpy( next, path );
		len = strlen(next);
		next[len-1] = 0;
		chdir( next );
	}

	// op two is pop
	if( op == 2 ) {
		chdir( previous );
	}

	// done
	return 0;
}


/*******************************************************************************
 *
 * Print Path
 *
 ******************************************************************************/
void PrintPath( char *str, int depth )
{
	for( int i = 0; i < (depth - 1); i++ )
		printf( "\t" );
	printf( "%s\n", str );
}



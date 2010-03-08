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
* Filename: UCCS_Main.cpp
* System Includes
*
*/



#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <direct.h>


/********************************************************************************
 *
 * Local Includes
 *
 ********************************************************************************/
#include "strncpynt.h"
#include "UCCS_CBatchEngine.h"
#include "HTMLOutput.h"
#include "CRetrieveCommandFromFile.h"
#include "CRetrieveCommandFromConsole.h"
#include "../DeviceControlChannel/CUCCSCommandControl.h"
#include "../DeviceControlChannel/CApplicationControlNull.h"
#include "../DeviceControlChannel/CommandControlErrors.h"


/********************************************************************************
 *
 * Definitions
 *
 ********************************************************************************/
#define MAXCONTROLMODULEARGS 256
#define MAXPATHLENGTH		 1024
#define WAIT_TIME_RECEIVE_BYTES_ERROR 100


/********************************************************************************
 *
 * Prototypes
 *
 ********************************************************************************/
int ParseParameters( int argc, char *argv[] );
int ProcessParameters();
void PrintUsage();


/********************************************************************************
 *
 * Types
 *
 ********************************************************************************/
typedef enum {
	OUTPUT_MODULE_HTML
} TOutputModule;

typedef enum {
	INPUT_MODULE_FILE,
} TInputModule;

typedef enum {
	CONTROL_MODULE_SERIAL,
	CONTROL_MODULE_CONSOLE,
	CONTROL_MODULE_TCP,
	CONTROL_MODULE_AUTO
} TControlModule;

typedef struct {
	TRemoteControlTransport iRemoteControlTransport;
	TOutputModule	iOutputModule;
	TInputModule	iInputModule;
	TControlModule	iControlModule;
	char			iControlModuleArgs[MAXCONTROLMODULEARGS];
	int				iUseAlternateScriptDirectory;
	char			iAlternateScriptDirectory[MAXPATHLENGTH];
} TControllerArgs;

typedef enum {
	OPT_HELP,

	// Control moduless
	OPT_SERIALCONTROL,
	OPT_CONSOLECONTROL,
	OPT_TCPCONTROL,
	OPT_AUTOCONTROL,

	// Input modules
	OPT_FILEINPUT,

	// Output modules
	OPT_HTMLOUTPUT,

	// Options
	OPT_SCRIPTDIR
} TCommandOptions;


/********************************************************************************
 *
 * File-scope variables
 *
 ********************************************************************************/
static TControllerArgs gControllerArgs;
static char *gOptionStrings[] = {	"--help", 
									"--serialcontrol", 
									"--consolecontrol", 
									"--tcpcontrol",
									"--autocontrol", 
									"--fileinput",  
									"--htmloutput", 
									"--scriptdir", 
									NULL };

// Module Classes
static CRetrieveCommandFromFile gRetrieveCommandFromFile;
static CHTMLOutput gHtmlOutput;
static CUCCSCommandControl *gApplicationControlPC;
static CApplicationControlNull *gApplicationControlNull;
static CBatchEngine *gBatchEngine;

// Module Interfaces
static IOutput *gOutput;
static IRetrieveCommand *gInput;
static IRemoteControl *gRemoteControl;


/********************************************************************************
 *
 * Main() program entry point
 *
 ********************************************************************************/
int main(int argc, char* argv[])
{
	int err, errcode, rv = 0, script_error;
	TCommandControlError channel_execution_error;

	// parse the parameters
	err = ParseParameters( argc, argv );
	if( err != 0 ) {
		PrintUsage();
		return -1;
	}

	// process the parameters 
	err = ProcessParameters();
	if( err != 0 ) {
		return -1;
	}

	// print a welcome banner
	fprintf( stdout, "------------------------------------------------------------------\n" );
	fprintf( stdout, "\n" );
	fprintf( stdout, " Network Emulator - Use Case Controller\n" );
	fprintf( stdout, "\n" );
	fprintf( stdout, " eg. runcmd svcname=\"TestService\"  methodid=\"1\"\n");
	fprintf( stdout, " eg. runcmd svcname=\"TestService\"  methodid=\"10\" call=\"TestCallOne\" param1=\"one\" param2=\"two\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"SyncService\"  methodid=\"1\"\n");
	fprintf( stdout, " eg. runcmd svcname=\"SyncService\"  methodid=\"10\" call=\"StartSyncTestCase\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"ExeService\"   methodid=\"1\"\n");
	fprintf( stdout, " eg. runcmd svcname=\"ExeService\"   methodid=\"2\"\n");
	fprintf( stdout, " eg. runcmd svcname=\"ExeService\"   methodid=\"10\" call=\"Start\"       cmd=\"cmd\" args=\"\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"ExeService\"   methodid=\"10\" call=\"Start\"       cmd=\"cmd\" args=\"\" isDefault=\"true\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"ExeService\"   methodid=\"10\" call=\"Stop\"        handle=\"0\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"IperfService\" methodid=\"10\" call=\"StartServer\" isDefault=\"true\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"PcapService\"  methodid=\"10\" call=\"StartCapture\" isDefault=\"true\" outputFileName=\"test10\"\n" );
	fprintf( stdout, " eg. runcmd svcname=\"PcapService\"  methodid=\"10\" call=\"StopCapture\"  isDefault=\"true\"\n" );
	
	fprintf( stdout, "------------------------------------------------------------------\n\n" );

	// start the remote control module listening for commands
	while( 1 ) {
		channel_execution_error = gRemoteControl->Start( gControllerArgs.iRemoteControlTransport, gControllerArgs.iControlModuleArgs, &errcode, &script_error );
		if( channel_execution_error == EAC_RECEIVEBYTESERROR ) {
			Sleep( WAIT_TIME_RECEIVE_BYTES_ERROR );
		}

		if( (script_error != 0) && (gRemoteControl == gApplicationControlNull) ) {
			fprintf( stderr, "WARNING: Script error occured, last error was '%s' (%d)\n", GetUccsErrorString((TUCCSErrors)script_error), script_error );
		}
	}

	// if err is not quit then print an error
	if( channel_execution_error != EAC_QUIT ) {
		fprintf( stdout, "WARNING: Controller completed with errors: '%s' (%d, %d)\n", GetCommandControlErrorString(channel_execution_error), channel_execution_error, errcode );
		rv = -2;
	}


	// now cleanup all the objects (just delete)
	gOutput = NULL;
	gInput = NULL;
	gRemoteControl = NULL;
	delete gBatchEngine;
	gBatchEngine = NULL;
	if( gApplicationControlPC != NULL ) {
		delete gApplicationControlPC;
		gApplicationControlPC = NULL;
	}
	if( gApplicationControlNull != NULL ) {
		delete gApplicationControlNull;
		gApplicationControlNull = NULL;
	}

	// done
	return rv;
}


/********************************************************************************
 *
 * Parse parameters
 *
 ********************************************************************************/
int ParseParameters( int argc, char *argv[] )
{
	int match, i, j;

	// set the arguments to default values
	gControllerArgs.iControlModule = CONTROL_MODULE_CONSOLE;
	gControllerArgs.iControlModuleArgs[0] = 0;
	gControllerArgs.iInputModule = INPUT_MODULE_FILE;
	gControllerArgs.iOutputModule = OUTPUT_MODULE_HTML;
	gControllerArgs.iUseAlternateScriptDirectory = 0;
	gControllerArgs.iAlternateScriptDirectory[0] = 0;
	gControllerArgs.iRemoteControlTransport = RCI_CONSOLE;

	// Parse the parameters to get the configuration information
	for( i = 1; i < argc; i++ ) {

		// search for a matching option string
		for( j = 0; gOptionStrings[j] != NULL; j++ ) {
			match = strcmp( argv[i], gOptionStrings[j] );
			if( match == 0 )
				break;
		}

		// if no option found then continue with the next token
		if( match != 0 ) {
			fprintf( stderr, "WARNING: unknown option '%s' (ignoring).\n", argv[i] );
			continue;
		}

		// otherwise parse each option
		switch( j ) {

			// just print the usage
		case OPT_HELP:
			return -1;

			// use an alternate script directory
		case OPT_SCRIPTDIR:
			if( i == (argc-1) ) {
				fprintf( stderr, "ERROR: --scriptdir requires a directory name to be specified.\n" );
				return -1;
			}
			gControllerArgs.iUseAlternateScriptDirectory = 1;
			STRNCPY_NULL_TERMINATE( gControllerArgs.iAlternateScriptDirectory, argv[++i], MAXPATHLENGTH );
			break;

			// use the serial control
		case OPT_SERIALCONTROL:
			if( i == (argc-1) ) {
				fprintf( stderr, "ERROR: --serialcontrol requires a portname to be specified (e.g. --serialcontrol COM2).\n" );
				return -1;
			}
			gControllerArgs.iRemoteControlTransport = RCI_SERIAL;
			gControllerArgs.iControlModule = CONTROL_MODULE_SERIAL;
			STRNCPY_NULL_TERMINATE( gControllerArgs.iControlModuleArgs, argv[++i], MAXCONTROLMODULEARGS );
			break;

			// use the control control
		case OPT_CONSOLECONTROL:
			gControllerArgs.iRemoteControlTransport = RCI_CONSOLE;
			gControllerArgs.iControlModule = CONTROL_MODULE_CONSOLE;
			break;

			// use the wintunnel control
		case OPT_TCPCONTROL:
			if( i == (argc-1) ) {
				fprintf( stderr, "ERROR: --tcpcontrol requires a portname to be specified (e.g. --tcpcontrol 110).\n" );
				return -1;
			}
			gControllerArgs.iRemoteControlTransport = RCI_TCP;
			gControllerArgs.iControlModule = CONTROL_MODULE_TCP;
			STRNCPY_NULL_TERMINATE( gControllerArgs.iControlModuleArgs, argv[++i], MAXCONTROLMODULEARGS );
			break;

			// use the auto control
		case OPT_AUTOCONTROL:
			if( i == (argc-1) ) {
				fprintf( stderr, "ERROR: --autocontrol requires a config string (i.e. <sid>[:<rc>[:<int>]]).\n" );
				return -1;
			}
			gControllerArgs.iRemoteControlTransport = RCI_NONE;
			gControllerArgs.iControlModule = CONTROL_MODULE_AUTO;
			STRNCPY_NULL_TERMINATE( gControllerArgs.iControlModuleArgs, argv[++i], MAXCONTROLMODULEARGS );
			break;

			// input from a script file
		case OPT_FILEINPUT:
			gControllerArgs.iInputModule = INPUT_MODULE_FILE;
			break;

			// output to html
		case OPT_HTMLOUTPUT:
			gControllerArgs.iOutputModule = OUTPUT_MODULE_HTML;
			break;
		}
	}

	// done - success
	return 0;
}


/********************************************************************************
 *
 * Process parameters
 *
 ********************************************************************************/
int ProcessParameters()
{
	int err;

	// use alternate directory
	if( gControllerArgs.iUseAlternateScriptDirectory != 0 ) {
		err = _chdir( gControllerArgs.iAlternateScriptDirectory );
		if( err != 0 ) {
			fprintf( stderr, "ERROR: failed to set directory '%s' - %s.\n", gControllerArgs.iAlternateScriptDirectory, strerror(errno) );
			return -1;
		}
	}

	// set the output module pointer
	gOutput = &gHtmlOutput;

	// set the script command input module 
	gInput = &gRetrieveCommandFromFile;

	// create the batchengine
	gBatchEngine = new CBatchEngine( gInput, gOutput );
	assert( gBatchEngine != NULL );

	// create the remote channel
	if( gControllerArgs.iControlModule == CONTROL_MODULE_SERIAL ) {
		gApplicationControlPC = new CUCCSCommandControl( gBatchEngine, gOutput );
		assert( gApplicationControlPC != NULL );
		gRemoteControl = gApplicationControlPC;
	}
	if( gControllerArgs.iControlModule == CONTROL_MODULE_CONSOLE ) {
		gApplicationControlPC = new CUCCSCommandControl( gBatchEngine, gOutput );
		assert( gApplicationControlPC != NULL );
		gRemoteControl = gApplicationControlPC;
	}
	if( gControllerArgs.iControlModule == CONTROL_MODULE_TCP ) {
		gApplicationControlPC = new CUCCSCommandControl( gBatchEngine, gOutput );
		assert( gApplicationControlPC != NULL );
		gRemoteControl = gApplicationControlPC;
	}
	if( gControllerArgs.iControlModule == CONTROL_MODULE_AUTO ) {
		gApplicationControlNull = new CApplicationControlNull( gBatchEngine, gOutput );
		assert( gApplicationControlNull != NULL );
		gRemoteControl = gApplicationControlNull;
	}

	// done - success
	return 0;
}


/********************************************************************************
 *
 * PrintUsage
 *
 ********************************************************************************/
void PrintUsage()
{
	fprintf( stderr, "\n" );
	fprintf( stderr, "usage: uccs [options]\n" );
	fprintf( stderr, "\n" );

	fprintf( stderr, "CONTROL COMMAND OPTIONS:\n" );
	fprintf( stderr, "\t--serialcontrol <comport>\n" );
	fprintf( stderr, "\t\tControl commands are sent over a serial cable.\n" );
	fprintf( stderr, "\t--consolecontrol\n" );
	fprintf( stderr, "\t\tControl commands are entered into the console.\n" );
	fprintf( stderr, "\t--tcpcontrol <port number>\n" );
	fprintf( stderr, "\t\tControl commands are sent over tcp/ip using WinTunnel.\n" );
	fprintf( stderr, "\t--autocontrol <use-case-id>[:<rendezvous-count>[:<interactive_flag>]]\n" );
	fprintf( stderr, "\t\tControl commands are automatically entered.\n" );
	fprintf( stderr, "\n" );

	fprintf( stderr, "GENERAL OPTIONS:\n" );
	fprintf( stderr, "\t--scriptdir <pathname>\n" );
	fprintf( stderr, "\t\tAlternate directory to look for script files and put output files.\n" );
	fprintf( stderr, "\n" );
}

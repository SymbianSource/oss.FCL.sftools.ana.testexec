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
* CTestDriverService.h
*
*/



#include <stdlib.h>

#include "CAThread.h"
#include "CListeningServer.h"
#include "CTestDriverService.h"
#include "CUCCIniFile.h"

// Parameters names
const char	KPlatform[]				= { "PLATFORM" };
const char	KBuild[]				= { "BUILD" };
const char	KSuite[]				= { "SUITE" };
const char	KTransport[]			= { "TRANSPORT" };
const char	KLogging[]				= { "LOGGING" };

// Ini file names
const char	KTDServiceIni[]			= { ".\\TestDriverService.ini" };
const char	KIniSectionName[]		= { "TestDriverService" };
const char	KIniPort[]				= { "Port" };
const char	KIniExeLocation[]		= { "Exe" };
const char	KIniPollInterval[]		= { "Poll" };

// TestDriver parameters and flags
const char	KTDConnection[]			= { "localhost" };
const char	KDefaultExe[]			= { "C:\\apps\\engtools\\testdriver2\\testdriver" };
const char	KTDBuild[]				= { " build " };
const char	KTDRun[]				= { " run " };
const char	KPlatformFlag[]			= { " -p " };
const char	KBuildFlag[]			= { " -b " };
const char	KSuiteFlag[]			= { " -s " };
const char	KTransportFlag[]		= { " -t " };
const char	KLoggingFlag[]			= { " -l " };
const char	KSocketFlag[]			= { " -f " };

// TCP Port for TestDriver to use to contact the TestDriverService listening socket
const int	KDefaultPort			= 3002;
const int	KContinuePollingValue	= 1;

// Default polling timeout for the Listening Server
const int	KDefaultPoll			= 5; // 5 seconds

// Static values to control the thread(s) execution
static bool			iServerRunning;
static bool			iTestDriverRunning;
static int			iTDStartResult;
static int			iTDCompleteResult;
static bool			iTDContinuePolling;
static bool			iTDRunSetupComplete;
static char*		iRunCommandLine;
static CAThread*	iServer;
static CAThread*	iTestDriverRun;
static HANDLE		iServerMutex;
static HANDLE		iTDRunMutex;

static string		iExe;
static int			iPort;
static int			iPollInterval;

CService* Service() { return new CTestDriverService(); }

/*
 *	Constructor
 */
CTestDriverService::CTestDriverService()
	{
	iServerRunning = false;
	iTestDriverRunning = false;
	iTDStartResult = 0;
	iTDCompleteResult = 0;
	iTDContinuePolling = false;
	iTDRunSetupComplete = false;
	iServer = NULL;
	iRunCommandLine = NULL;
	iServerMutex = NULL;
	iTDRunMutex = NULL;
	iExe = KDefaultExe;
	iPort = KDefaultPort;
	iPollInterval = KDefaultPoll;
	}

/*
 *	Destructor
 */
CTestDriverService::~CTestDriverService()
	{
	// Let the Listening Server clean itself up
	Wait( iServerMutex );
	if( iServerRunning )
		{
		iServerRunning = false;
		}
	iServer->WaitForThread( -1 );
	if( iServer != NULL )
		{
		delete iServer;
		iServer = NULL;
		}
	Release( iServerMutex );
	ReleaseMutex( iServerMutex );
	CloseHandle( iServerMutex );

	// Clean up the TestDriver thread
	Wait( iTDRunMutex );
	if( iTestDriverRunning )
		{
		iTestDriverRunning = false;
		}
	iTestDriverRun->WaitForThread( -1 );
	if( iTestDriverRun != NULL )
		{
		delete iTestDriverRun;
		iTestDriverRun = NULL;
		}

	// Cleanup the shared command line
	if( iRunCommandLine != NULL )
		{
		delete [] iRunCommandLine;
		iRunCommandLine = NULL;
		}
	Release( iTDRunMutex );
	ReleaseMutex( iTDRunMutex );
	CloseHandle( iTDRunMutex );
	}


/*
 *	Setup
 *
 *   Creates the mutex's requires by this service
 */
bool CTestDriverService::Setup()
	{
	bool ret = false;

	// Read the ini file configuration if available, otherwise default
	RetrieveConfig();

	// Create the Listening Server and TestDrive run mutex
	iServerMutex = CreateMutex( NULL, false, NULL );
	iTDRunMutex = CreateMutex( NULL, false, NULL );

	if( iServerMutex != NULL && iTDRunMutex != NULL )
		{
		ret = true;
		}

	return ret;
	}

/*
 *	RunCommand
 *
 *   Extracts the call id and delegates to the required functionality
 */
int CTestDriverService::RunCommand( const CCall& aCall )
	{
	int		ret		= ERR_INVALID_CALL;
	bool	callRet	= false;

	// Retrieve the call id
	int callID = -1;
	callRet = aCall.CallID( callID );
	if( callRet )
		{
		switch( callID )
			{
			case 1: // Build and initiate TestDriver (TestExecute) execution
				{
				ret = BuildAndRunTestDriver( aCall );
				}
				break;
			case 2: // Retrieve the TestDriver run execution result
				{
				ret = RetrieveTestDriverRunResult();
				}
				break;
			default:
				{
				ret = ERR_INVALID_CALL;
				}
				break;
			}
		}

	return ret;
	}

/*
 *	BuildAndRunTestDriver
 *
 *   Builds all of the dependancies and starts the execution via TestDriver
 */
int	CTestDriverService::BuildAndRunTestDriver( const CCall& aCall )
	{
	int ret = ERR_NONE;

	// Test to see if the Listening Server is already running
	// If running, we wait for it exit.
	Wait( iServerMutex );
	if( iServerRunning )
		{
		Release( iServerMutex );
		RetrieveTestDriverRunResult();
		//ret = ERR_TEST_DRIVER_ALREADY_RUNNING;
		}
	else 
		{
		Release( iServerMutex );
		}

	// Retrieve all the parameters
	
	// The build command line
	string buildCommandLine;
	buildCommandLine += iExe;
	buildCommandLine += KTDBuild;
	if( !ret )
		{
		ret = AppendCmdLineParams( aCall, buildCommandLine );
		}

	// The run command line
	string runCommandLine;
	runCommandLine += iExe;
	runCommandLine += KTDRun;
	if( !ret )
		{
		ret = AppendCmdLineParams( aCall, runCommandLine );
		}

	// Make the TestDriver build call
	if( !ret )
		{
		int tdRet = system( buildCommandLine.c_str() );
		if( tdRet )
			{
			ret = ERR_TEST_DRIVER_BUILD;
			}
		}

	if( !ret )
		{
		// Append the details for the listening socket 
		runCommandLine += KSocketFlag;
		runCommandLine += KTDConnection;
		runCommandLine += ":";
		char	portBuf[20];
		sprintf( portBuf, "%d", iPort );
		runCommandLine += portBuf;

		if( iRunCommandLine != NULL )
			{
			delete [] iRunCommandLine;
			iRunCommandLine = NULL;
			}

		// Setup the command line for the execution thread
		iRunCommandLine = new char[runCommandLine.length()+1];
		strcpy( iRunCommandLine, runCommandLine.c_str() );
		}

	// Start the listening server in its own thread
	if( !ret )
		{
		// Set the server running flag to true
		iServerRunning = true;
		// Kick off the Listening Server in its own thread
		iServer = new CAThread( "CTestDriverService::ListeningServer" );
		int err = 1;
		TThreadError terr = iServer->StartThread( (void*)CTestDriverService::ListeningServer, NULL, &err ); 
		if( terr != TE_NONE )
			{
			delete iServer;
			iServer = NULL;
			ret = ERR_TEST_DRIVER_RUN;
			}
		}

	// Launch TestDriver in run mode in its own thread
	if( !ret )
		{
		// Set the TestDriver running flag to true
		iTestDriverRunning = true;
		// Kick off TestDriver in its own thread
		iTestDriverRun = new CAThread( "CTestDriverService::TestDriverRun" );
		int err = 1;
		TThreadError terr = iTestDriverRun->StartThread( (void*)CTestDriverService::TestDriverRun, NULL, &err ); 
		if( terr != TE_NONE )
			{
			delete iTestDriverRun;
			iTestDriverRun = NULL;
			ret = ERR_TEST_DRIVER_RUN;
			}
		}

	// Retrieve the TestDriver run startup result
	if( !ret )
		{
		bool cont = false;
		while( !cont && iTestDriverRunning )
			{
			Sleep( iPollInterval * 1000 );
			Wait( iServerMutex );
			iTDRunSetupComplete?cont=true:cont=false;
			Release( iServerMutex );
			}
		Wait( iServerMutex );
		iTDStartResult>0?ret=0:ret=ERR_TEST_DRIVER_RUN;
		iTDRunSetupComplete = false;
		Release( iServerMutex );
		}

	return ret;
	}

/*
 *	RetrieveTestDriverRunResult
 *
 *   Retrieves the result upon the TestDriver run completion 
 */
int CTestDriverService::RetrieveTestDriverRunResult()
	{
	int ret = ERR_NONE;
	
	Wait( iServerMutex );
	// Set the continue polling flag
	iTDContinuePolling = true;
	Release( iServerMutex );

	// The ListeningServer thread will now issue TestDriver with the command
	// to complete and cleanup and the return code will be set from the TestDriverRun
	// thread when the executable completes.

	// Wait for the TestDriverRun thread to complete
	bool cont = false;
	while( !cont )
		{
		Sleep( iPollInterval * 1000 );
		Wait( iTDRunMutex );
		iTestDriverRunning?cont=false:cont=true;
		Release( iTDRunMutex );
		}

	// Cleanup the threads as they have now completed
	Wait( iServerMutex );
	iServerRunning = false;
	Release( iServerMutex );

	Wait( iTDRunMutex );
	iTestDriverRunning = false;
	Release( iTDRunMutex );

	// Now the TestDriver executable has complete return the 
	// appropriate error code
	if( iTDCompleteResult < 0 )
		{
		ret = ERR_TEST_DRIVER_RUN_COMPLETE;
		}

	return ret;
	}

/*
 *	ListeningServer
 *
 *    Runs the Listening Server
 *      Controlled via the main/TestDriver run thread
 */
void CTestDriverService::ListeningServer( void )
	{
	// Create a new CListeningServer
	CListeningServer* server = new CListeningServer();
	if( server != NULL )
		{
		// Open it an wait until TestDriver has connected
		int err = server->Open( iPort );

		// Continue if TestDriver connected successfully and the server is set to running
		// Retrieve the TestDriver execution startup result
		// Loop until told to stop
		if( !err )
			{
			int recvInt = 0;
			iTDStartResult = 0;
			bool cont = true;
			while( cont )
				{
				Wait( iServerMutex );

				// Attempt to recieve the TestDriver startup code
				// This recieve call can timeout and we continue retrying
				err = server->Recieve( recvInt );
				if( !err )
					{
					// Set the TestDriver run startup result
					iTDStartResult = recvInt;
					iTDRunSetupComplete = true;
					Release( iServerMutex );
					break;
					}
				iServerRunning?cont=true:false;
				Release( iServerMutex );
				}
			}

		// Check to see that TestDriver has returned a run number.
		// If it hasn't then the startup has failed.
		if( iTDStartResult > 0 )
			{
			// Wait until the continue polling flag has been set
			bool cont = true;
			while( !err && cont )
				{
				Wait( iServerMutex );
				if( iTDContinuePolling )
					{
					err = server->Send( KContinuePollingValue );
					// Reset the polling flag
					iTDContinuePolling = false;
					Release( iServerMutex );
					break;
					}
				Release( iServerMutex );
				Sleep( iPollInterval * 1000 );
				Wait( iServerMutex );
				iServerRunning?cont=true:cont=false;
				Release( iServerMutex );
				}
			}
		server->Close();
		}

	delete server;
	server = NULL;
	Wait( iServerMutex );
	iServerRunning = false;
	Release( iServerMutex );
	}

/*
 *	TestDriverRun
 *
 *    Executes the run command for TestDriver
 */
void CTestDriverService::TestDriverRun( void )
	{
	Wait( iTDRunMutex );
	// Launch TestDriver
	if( iRunCommandLine != NULL )
		{
		iTDCompleteResult = system( iRunCommandLine );
		}
	else
		{
		iTDCompleteResult = -1;
		}

	// Reset the status to not running
	iTestDriverRunning = false;
	Release( iTDRunMutex );
	}

/*
 *	Wait
 *
 *    For synchronisation
 */
void CTestDriverService::Wait( HANDLE aMutexHandle )
	{
	WaitForSingleObject( aMutexHandle, INFINITE );
	}

/*
 *	Release
 *
 *    For synchronisation
 */
void CTestDriverService::Release( HANDLE aMutexHandle )
	{
	ReleaseMutex( aMutexHandle );
	}


void CTestDriverService::RetrieveConfig()
	{
	bool ret = false;

	// Load the configuration information file
	CUCCIniFile iniFile( KTDServiceIni );

	// Retrieve the port to be used
	int port;
	if( iniFile.KeyValue(KIniPort, KIniSectionName, port) )
		{
		iPort = port;
		}

	// Retrieve the exe location/filename
	string exe;
	if( iniFile.KeyValue(KIniExeLocation, KIniSectionName, exe) )
		{
		iExe = exe;
		}

	// Retrieve the poll interval
	int pollInterval;
	if( iniFile.KeyValue(KIniPollInterval, KIniSectionName, pollInterval) )
		{
		iPollInterval = pollInterval;
		}
	}


int CTestDriverService::AppendCmdLineParams( const CCall& aCall, string &aCmdLine )
	{
	int ret = ERR_NONE;

	int numParams = 0;
	bool callRet = aCall.Params( numParams );
	if( !ret && callRet && numParams >= 3 )
		{
		for( int index=0 ; index<numParams && callRet && !ret ; index++ )
			{
			// Retrieve the PLATFORM parameters
			string name;
			string value;
			callRet = aCall.Name( index, name );
			if( callRet )
				{
				callRet = aCall.Value( index, value );
				}
			
			if( callRet )
				{
				if( name == KPlatform )
					{
					aCmdLine += KPlatformFlag;
					aCmdLine += value;
					}
				else if( name == KBuild )
					{
					aCmdLine += KBuildFlag;
					aCmdLine += value;
					}
				else if( name == KSuite )
					{
					aCmdLine += KSuiteFlag;
					aCmdLine += value;
					}
				else if( name == KTransport )
					{
					aCmdLine += KTransportFlag;
					aCmdLine += value;
					}
				else if( name == KLogging )
					{
					aCmdLine += KLoggingFlag;
					aCmdLine += value;
					}
				else
					{
					ret = ERR_INVALIDARG;
					}
				}
			else
				{
					ret = ERR_INVALIDARG;
				}
			}
		}
	else
		{
			ret = ERR_INVALIDARG;
		}
	
	return ret;
	}

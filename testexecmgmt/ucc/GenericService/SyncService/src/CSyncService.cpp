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
* CSyncService.h
*
*/



#include <stdio.h>
#include <crtdbg.h>
#include <time.h>

#include "CSyncService.h"
#include "CUCCIniFile.h"

// Parameters names
const char	KSharedDataName[]				= { "NAME" };
const char	KSharedDataValue[]				= { "VALUE" };


// Sync Service ini file
const char	KSyncServiceIni[]				= { ".\\SyncService.ini" };
const char	KIniSectionName[]				= { "SyncService" };
const char	KIniConnection[]				= { "Connection" };
const char	KIniSerialPort[]				= { "SerialPort" };
const char	KIniIPAddress[]					= { "IPAddress" };
const char	KSerial[]						= { "serial" };
const char	KInfrared[]						= { "ir" };
const char	KBluetooth[]					= { "bt" };
const char	KSocket[]						= { "tcp" };
const char	KTimeout[]						= { "Timeout" };
const char	KPoll[]							= { "Poll" };
const char	KSTATLocation[]					= { "STAT" };

// Timeout defaults (in seconds)
const int	KDefaultTimeout					= 120;
const int	KDefaultPollInterval			= 10;

// STAT DLL Location
#ifdef _DEBUG
const char	KDefaultSTATDLLLocation[]		= { "\\epoc32\\tools\\stat\\statd.dll" };
#else
const char	KDefaultSTATDLLLocation[]		= { "\\epoc32\\tools\\stat\\stat.dll" };
#endif

// STAT Sync calls
const char	KStartSyncTestCase[]			= { "<B><OTEFStatus,4><E>" };
const char	KStartSyncComplete[]			= { "<B><OTEFStatus,2><E>" };
const char	KRetrieveSyncTestCaseResult[]	= { "<B><NTEFResult><E>" };
const char	KRetrieveSyncTestCaseStatus[]	= { "<B><NTEFStatus><E>" };
const char	KSetSharedData[]				= { "<B><O" };
const char	KCommandTerminate[]				= { "><E>" };
const char	KComma[]						= { "," };

const int	KMaxSharedDataLength			= 2048;

// Possible TEF synchronised test case error codes
const int	EPass							= 0;		
const int	EFail							= 106;
const int	EInconclusive					= 107;
const int	ETestSuiteError					= 108;
const int	EAbort							= 109;
const int	EIgnore							= 110;

// TEF Sync Status codes
enum TSyncStatus
	{
	ETEFSyncUnknown		= 0,
	ETEFSyncRunning		= 1,
	ETEFSyncComplete	= 2,
	ETEFSyncWaiting		= 3,
	ETEFSyncContinue	= 4,
	ETEFRetrieveResult	= 5
	};

CService* Service() { return new CSyncService(); }

CSyncService::CSyncService()
	: ihLib(NULL), iptrConnect(NULL), iptrDisconnect(NULL), iptrSendRawCommand(NULL),
		iptrGetTEFSharedData(NULL), iConnection(0), iTimeout(KDefaultTimeout),
		iPollInterval(KDefaultPollInterval), iSTATDLLLocation(KDefaultSTATDLLLocation)
	{
	}

CSyncService::~CSyncService()
	{
	// Free the STAT DLL library
	if( ihLib != NULL )
		{
		::FreeLibrary( ihLib );
		ihLib =	NULL;
		}
	}

bool CSyncService::Setup()
	{
	bool ret = true;

	ret = RetrieveCommsInfo();
	if(!ret)
		return ret;

	// Load the STAT DLL library.
	ihLib					= ::LoadLibrary( iSTATDLLLocation.c_str() );
	iptrConnect				= reinterpret_cast<PROC_CONNECT>(::GetProcAddress( ihLib, ProcConnect ));
	iptrDisconnect			= reinterpret_cast<PROC_DISCONNECT>(::GetProcAddress( ihLib, ProcDisconnect ));
	iptrSendRawCommand		= reinterpret_cast<PROC_SENDRAWCOMMAND>(::GetProcAddress( ihLib, ProcSendRawCommand ));
	iptrGetTEFSharedData	= reinterpret_cast<PROC_GETTEFSHAREDDATA>(::GetProcAddress( ihLib, ProcGetTEFSharedData ));

	if( ihLib == NULL ||
		iptrConnect == NULL || 
		iptrDisconnect == NULL ||
		iptrSendRawCommand == NULL ||
		iptrGetTEFSharedData == NULL )
		{
		ret = false;
		}
	return ret;
	}

int CSyncService::RunCommand( const CCall& aCall )
	{
	int ret = ERR_NONE;

	// Connect to the device via STAT
	if( !iConnection )
		{
		if( iConnectionType == SymbianSerial ||
			iConnectionType == SymbianInfrared ||
			iConnectionType == SymbianBluetooth )
			{
			iConnection = (iptrConnect)(iConnectionType,iComPort.c_str(),NULL,NULL);
			}
		else if( iConnectionType == SymbianSocket )
			{
			iConnection = (iptrConnect)(iConnectionType,iIPAddress.c_str(),NULL,NULL);
			}
		}

	if( !iConnection )
		{
		ret = ERR_STAT;
		}

	int callID = -1;
	if( aCall.CallID( callID ) && ret == ERR_NONE )
		{
		switch( callID )
			{
			case 1: // StartSyncTestCase
				{
				ret = StartSyncTestCase( aCall );
				}
				break;
			case 2: // RetrieveSyncTestCaseResult
				{
				ret = RetrieveSyncTestCaseResult( aCall );
				}
				break;
			case 3: // SetSharedData
				{
				ret = SetSharedData( aCall );
				}
				break;
			default:
				{
				#ifdef _DEBUG
				printf( "Unknown CallID\n" );
				#endif
				ret = ERR_INVALID_CALL;
				}
			}
		}
	else
		{
		ret = ERR_GENERAL;
		}

	// Disconnect the device via STAT
	if( iConnection )
		{
		int disconnectOK;
		disconnectOK = (iptrDisconnect)(iConnection);
		if( !disconnectOK )
			{
			ret = ERR_STAT;
			}
		else
			{
			iConnection = 0;
			}
		}

	return ret;
}

bool CSyncService::RetrieveCommsInfo()
	{
	bool ret = false;

	// Load the configuration information file
	CUCCIniFile iniFile( KSyncServiceIni );

	// Retrieve the connection type
	string connectionType;
	if( iniFile.KeyValue(KIniConnection, KIniSectionName, connectionType) )
		{
			if( connectionType == KSerial )
				{
				iConnectionType = SymbianSerial;
				ret = true;
				}
			else if( connectionType == KInfrared )
				{
				iConnectionType = SymbianInfrared;
				ret = true;
				}
			else if( connectionType == KBluetooth )
				{
				iConnectionType = SymbianBluetooth;
				ret = true;
				}
			else if( connectionType == KSocket )
				{
				iConnectionType = SymbianSocket;
				ret = true;
				}
			else
				{
				iConnectionType = SymbianInvalid;
				}
		}
	else
		{
		ret = false;
		}

	// Retrieve either the com port or the ip address
	if( ret )
		{
		if( iConnectionType == SymbianSerial ||
			iConnectionType == SymbianInfrared ||
			iConnectionType == SymbianBluetooth )
			{
			if( !iniFile.KeyValue(KIniSerialPort, KIniSectionName, iComPort) )
				{
				ret = false;
				}
			}
		else if( iConnectionType == SymbianSocket )
			{
			if( !iniFile.KeyValue(KIniIPAddress, KIniSectionName, iIPAddress) )
				{
				ret = false;
				}
			}
		}

	// Finally retrieve the timeout, poll interval and STAT DLL location values (all optional)
	if( ret )
		{
		// Timeout
		int timeout = 0;
		if( iniFile.KeyValue(KTimeout, KIniSectionName, timeout) )
			{
			if( timeout > 0 )
				{
				iTimeout = timeout;
				}
			}

		// Poll Interval
		int pollInterval = 0;
		if( iniFile.KeyValue(KPoll, KIniSectionName, pollInterval) )
			{
			if( pollInterval > 0 )
				{
				iPollInterval = pollInterval;
				}
			}

		// STAT DLL Location
		string statDLLLocation;
		if( iniFile.KeyValue(KSTATLocation, KIniSectionName, statDLLLocation) )
			{
			if( statDLLLocation.length() > 0 )
				{
				iSTATDLLLocation.erase();
				iSTATDLLLocation = statDLLLocation;
				}
			}
		}

	return ret;
	}

int	CSyncService::StartSyncTestCase( const CCall& aCall )
	{
	int ret = ERR_NONE;

	printf( "StartSyncTestCase call\n" );

	// Wait until the status is set to WAITING
	// ie. The next test case is waiting
	// Make the RetrieveSyncTestCaseStatus call

	// Poll for the TEF Sync Status
	bool tefContinue = false;
	int sendRawOK =	(iptrSendRawCommand)(	iConnection,
											KRetrieveSyncTestCaseStatus,
											NULL);

	TSyncStatus TEFstatus = ETEFSyncUnknown;
	if( sendRawOK == ITS_OK )
		{
		const char* result = (iptrGetTEFSharedData)( iConnection );
		if( result != NULL )
			{
			TEFstatus = (TSyncStatus)atoi( result );
			if( TEFstatus == ETEFSyncWaiting )
				{
				tefContinue = true;
				}
			}
		}

	// Poll for the test case status
	time_t end_time, cur_time;
	time(&cur_time);
	end_time = cur_time + iTimeout;
	while(	!tefContinue &&	end_time-cur_time > 0 )
		{
		// Delay before next attempt
		Sleep( iPollInterval * 1000 );

		// Make the RetrieveSyncTestCaseStatus call (poll)
		sendRawOK =	(iptrSendRawCommand)(	iConnection,
											KRetrieveSyncTestCaseStatus,
											NULL);
		if( sendRawOK == ITS_OK )
			{
			const char* result = (iptrGetTEFSharedData)( iConnection );
			if( result != NULL )
				{
				TEFstatus = (TSyncStatus)atoi( result );
				if( TEFstatus == ETEFSyncWaiting )
					{
					tefContinue = true;
					}
				}
			}

		// Update the current time
		time(&cur_time);
		}
	
	if( sendRawOK == ITS_OK && TEFstatus == ETEFSyncWaiting )
		{
		// Make the StartSyncTestCase call
		int sendRawOK = (iptrSendRawCommand)(	iConnection,
												KStartSyncTestCase,
												NULL);
		if( sendRawOK != ITS_OK )
			{
			ret = ERR_STAT_START_SYNC_TEST_CASE;
			}
		}
	else
		{
			ret = ERR_STAT_RETRIEVE_SYNC_TEST_STATUS;
		}

	return ret;
	}

int	CSyncService::RetrieveSyncTestCaseResult( const CCall& aCall )
	{
	int ret = ERR_NONE;

	printf( "RetrieveSyncTestCaseResult call\n" );

	// Wait until the status is set to either COMPLETE or WAITING
	// ie. The sync test case has completed or the next one is waiting
	// Make the RetrieveSyncTestCaseStatus call

	// Poll for the TEF Sync Status
	bool tefContinue = false;
	int sendRawOK =	(iptrSendRawCommand)(	iConnection,
											KRetrieveSyncTestCaseStatus,
											NULL);

	TSyncStatus TEFstatus = ETEFSyncUnknown;
	if( sendRawOK == ITS_OK )
		{
		const char* result = (iptrGetTEFSharedData)( iConnection );
		if( result != NULL )
			{
			TEFstatus = (TSyncStatus)atoi( result );
			if( TEFstatus == ETEFRetrieveResult )
				{
				tefContinue = true;
				}
			}
		}

	// Poll for the test case status
	time_t end_time, cur_time;
	time(&cur_time);
	end_time = cur_time + iTimeout;
	while(	!tefContinue &&	(end_time-cur_time > 0) )
		{
		// Delay before next attempt
		Sleep( iPollInterval * 1000 );

		// Make the RetrieveSyncTestCaseStatus call (poll)
		sendRawOK =	(iptrSendRawCommand)(	iConnection,
											KRetrieveSyncTestCaseStatus,
											NULL);
		if( sendRawOK == ITS_OK )
			{
			const char* result = (iptrGetTEFSharedData)( iConnection );
			if( result != NULL )
				{
				TEFstatus = (TSyncStatus)atoi( result );
				if( TEFstatus == ETEFRetrieveResult )
					{
					tefContinue = true;
					}
				}
			}

		// Update the current time
		time(&cur_time);
		}

	if( sendRawOK == ITS_OK && TEFstatus == ETEFRetrieveResult )
		{
		// Make the RetrieveSyncTestCaseResult call
		int sendRawOK =	(iptrSendRawCommand)(	iConnection,
												KRetrieveSyncTestCaseResult,
												NULL);

		// Check what was returned from TEF
		if( sendRawOK == ITS_OK )
			{
			// Now retrieve the TEF Shared data from STAT
			// In the is case the shared data stores the synchronised
			//  test case result as an integer.
			// Set the ret value to equal to this.
			const char* result = (iptrGetTEFSharedData)( iConnection );
			int TEFret = EInconclusive;
			if( result != NULL )
				{
				TEFret = atoi( result );
				}

			// Map the code onto the return value
			switch( TEFret )
				{
				case EPass:
				case EFail:
				case EInconclusive:
				case ETestSuiteError:
				case EAbort:
				case EIgnore:
					{
					ret = TEFret;
					}
					break;
				default:
					{
					ret = EFail;
					}
					break;
				}
			
			// Update the status so TEF can continue
			int sendRawOK = (iptrSendRawCommand)(	iConnection,
													KStartSyncComplete,
													NULL);
			if( sendRawOK != ITS_OK )
				{
				ret = ERR_STAT_START_SYNC_TEST_CASE;
				}
			}
		else
			{
			ret = ERR_STAT_RETRIEVE_SYNC_TEST_RESULT;			
			}
		}
	else
		{
		ret = ERR_STAT_RETRIEVE_SYNC_TEST_STATUS;			
		}

	return ret;
	}

int	CSyncService::SetSharedData( const CCall& aCall )
	{
	int ret = ERR_NONE;

	#ifdef _DEBUG
	printf( "SetSharedData call\n" );
	#endif

	int numParams = 0;
	bool callRet = aCall.Params( numParams );
	if( numParams == 2 )
		{
		string paramName;
		string sharedDataName;
		string paramValue;
		string sharedDataValue;

		// Retrieve the shared data name
		if( aCall.Name(0, paramName) && aCall.Value(0, sharedDataName) )
			{
			if( paramName != KSharedDataName )
				{
				ret = ERR_INVALIDARG;
				}
			}
		else
			{
			ret = ERR_INVALIDARG;
			}

		// Retrieve the shared data value
		if( ret == 0 )
			{
			if( aCall.Name(1, paramValue) && aCall.Value(1, sharedDataValue) )
				{
				if( paramValue != KSharedDataValue )
					{
					ret = ERR_INVALIDARG;
					}
				}
			else
				{
				ret = ERR_INVALIDARG;
				}
			}

		// Send the shared data (name and value) via STAT
		if( ret == 0 )
			{
			if( sharedDataValue.size() > 0 )
				{
				// Construct the call
				string sharedDataCall;
				sharedDataCall += KSetSharedData;
				sharedDataCall += sharedDataName;
				sharedDataCall += KComma;
				sharedDataCall += sharedDataValue;
				sharedDataCall += KCommandTerminate;

				// Make the SetSharedData call
				int sendRawOK = (iptrSendRawCommand)(	iConnection,
														sharedDataCall.c_str(),
														NULL);
				if( sendRawOK != ITS_OK )
					{
					ret = ERR_STAT_SET_SHARED_DATA;
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

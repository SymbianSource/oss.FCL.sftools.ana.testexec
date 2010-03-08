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



/**
 @file CUCCTest.cpp
*/

#include <stdlib.h>
#include <string.h>
#include <testexecutelog.h>
#include "CUCCTest.h"
#include "CNetworkEmulatorSetupCommdb.h"
#include "uccs_errorcodes.h"

#define RECORD_ID			"RECORD_ID"
#define COMMDB_VALUE		"VALUETOSET"
#define BUFFERSIZE			256	
#define MAXLOGINSCRIPT		512
#define MAXVARLEN			64

const TInt KMaxCommandLineLength	= 4096;
const TInt KMaxParamNameLength		= 128;
const TInt KMaxParamValueLength		= 1024;


CUCCTestStartUseCase::CUCCTestStartUseCase()
	{
	SetTestStepName(KTestStartUseCase);
	}

CUCCTestStartUseCase::~CUCCTestStartUseCase()
	{
	}

TVerdict CUCCTestStartUseCase::doTestStepL()
	{	
	int ret, uid;
	int err = 0;
	CUCCSDeviceControl *app;

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	// start the usecase
	ret = app->startUseCase(uid, &err);

	// cleanup the app control instance
	delete app;

	// return success / failure
	SetTestStepResult( (ret == UCCS_OK) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestSignal::CUCCTestSignal()
	{
	SetTestStepName(KTestSignal);
	}

CUCCTestSignal::~CUCCTestSignal()
	{
	}

TVerdict CUCCTestSignal::doTestStepL()
	{
	int ret, uid;
	int err = 0;
	CUCCSDeviceControl *app;

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();	
		}

	// call signal
	ret = app->signal(uid, &err);

	// cleanup the app control instance
	delete app;

	// return success / failure
	SetTestStepResult( (ret == UCCS_OK) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestRendezvous::CUCCTestRendezvous()
	{
	SetTestStepName(KTestRendezvous);
	}

CUCCTestRendezvous::~CUCCTestRendezvous()
	{
	}

TVerdict CUCCTestRendezvous::doTestStepL()
	{
	int ret, uid;
	int err = 0;
	CUCCSDeviceControl *app;

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	// call rendezvous
	ret = app->rendezvous(uid, &err);

	// cleanup the app control instance
	delete app;

	// return success / failure
	SetTestStepResult( (ret == UCCS_OK) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestEndUseCase::CUCCTestEndUseCase()
	{
	SetTestStepName(KTestEndUseCase);
	}

CUCCTestEndUseCase::~CUCCTestEndUseCase()
	{
	}

TVerdict CUCCTestEndUseCase::doTestStepL()
	{
	int ret, uid;
	int err = 0;
	CUCCSDeviceControl *app;

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	
	// call endusecase
	TVerdict tmpVerdict = EFail;
	ret = app->endUseCase(uid, tmpVerdict, &err);
	SetTestStepResult( tmpVerdict );

	// cleanup the app control instance
	delete app;

	// return success / failure
	SetTestStepResult( (ret == UCCS_OK) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestWaitfor::CUCCTestWaitfor()
	{
	SetTestStepName(KTestWaitfor);
	}

CUCCTestWaitfor::~CUCCTestWaitfor()
	{
	}

TVerdict CUCCTestWaitfor::doTestStepL()
	{
	int ret, uid;
	int err = 0;
	CUCCSDeviceControl *app;

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	// call waitfor
	ret = app->waitfor(uid, &err);

	// cleanup the app control instance
	delete app;

	// return success / failure
	SetTestStepResult( (ret == UCCS_OK) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestGetVariableName::CUCCTestGetVariableName()
	{
	SetTestStepName(KTestGetVariableName);
	}

CUCCTestGetVariableName::~CUCCTestGetVariableName()
	{
	}

TVerdict CUCCTestGetVariableName::doTestStepL()
	{
	int ret, uid;
	int err = 0;
	CUCCSDeviceControl *app;
	char variableValue[64];
	char* variableName = "ENV";

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	// call getVariableName
	ret = app->getVariableName( variableName, variableValue, 64, &err );

	// cleanup the app control instance
	delete app;

	// return success / failure
	SetTestStepResult( (ret == UCCS_OK) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestRunCommand::CUCCTestRunCommand()
	{
	SetTestStepName(KTestRunCommand);
	}

CUCCTestRunCommand::~CUCCTestRunCommand()
	{
	}

// Ini file parameter names and standard values
_LIT(KEquals, "=");
_LIT(KSpace, " ");
_LIT(KQuote, "\"");
_LIT(KRunCmd, "runcmd");
_LIT(KServiceName, "svcname");
_LIT(KCall, "call");
_LIT(KMethodID, "methodid");
_LIT(KDefaultMethodID, "10");

TVerdict CUCCTestRunCommand::doTestStepL()
	{
	TInt ret, uid;
	TInt err = 0;
	CUCCSDeviceControl *app;
	HBufC* commandLine = HBufC::NewLC(KMaxCommandLineLength);
	TPtr commandLinePtr(commandLine->Des());
	
	// Specific ini file parameter names and standard values
	_LIT(KServiceID, "svcid");
	_LIT(KServiceHost, "svchost");
	_LIT(KNumParams, "num_params" );
	_LIT(KParamNameStr,"param_name_");
	_LIT(KParamValueStr,"param_value_");
	_LIT(KExpectPositiveReturnCodeStr,"expect_positive_return_code");

	commandLinePtr.Append( KRunCmd );

	// Retrieve the service name
	TPtrC svcname;
	TInt rv = GetStringFromConfig( ConfigSection(), KServiceName, svcname );
	if( rv )
		{
		// Append the service_name to the command line
		commandLinePtr.Append( KSpace );
		commandLinePtr.Append( KServiceName );
		commandLinePtr.Append( KEquals );
		commandLinePtr.Append( KQuote );
		commandLinePtr.Append( svcname );
		commandLinePtr.Append( KQuote );
		}
	else
		{
		// Otherwise revert to the legacy svcid and svchost
		TPtrC svcid;
		TPtrC svchost;
		rv = GetStringFromConfig( ConfigSection(), KServiceID, svcid );
		if( rv )
			{
			rv = GetStringFromConfig( ConfigSection(), KServiceHost, svchost );
			}
		if( rv )
			{
			// Append the svcid command line
			commandLinePtr.Append( KSpace );
			commandLinePtr.Append( KServiceID );
			commandLinePtr.Append( KEquals );
			commandLinePtr.Append( svcid );
			// Append the svchost command line
			commandLinePtr.Append( KSpace );
			commandLinePtr.Append( KServiceHost );
			commandLinePtr.Append( KEquals );
			commandLinePtr.Append( KQuote );
			commandLinePtr.Append( svchost );
			commandLinePtr.Append( KQuote );
			}
		}

	if( rv )
		{
		// Look up the call name
		TPtrC call;
		rv = GetStringFromConfig( ConfigSection(), KCall, call );
		if( rv )
			{
			// Append the default methodid (10) to the command line
			commandLinePtr.Append( KSpace );
			commandLinePtr.Append( KMethodID );
			commandLinePtr.Append( KEquals );
			commandLinePtr.Append( KDefaultMethodID );
			// Append the call name to the command line
			commandLinePtr.Append( KSpace );
			commandLinePtr.Append( KCall );
			commandLinePtr.Append( KEquals );
			commandLinePtr.Append( KQuote );
			commandLinePtr.Append( call );
			commandLinePtr.Append( KQuote );
			}
		else
			{
			// If it is not there look up the method id
			TPtrC methodid;
			rv = GetStringFromConfig( ConfigSection(), KMethodID, methodid );
			if( rv )
				{
				// Append the methodid command line
				commandLinePtr.Append( KSpace );
				commandLinePtr.Append( KMethodID );
				commandLinePtr.Append( KEquals );
				commandLinePtr.Append( methodid );
				}
			}
		}

	// Iterate through the parameters adding them to the command line as we go
	TInt num_params = 0;
	rv = GetIntFromConfig( ConfigSection(), KNumParams, num_params );
	if( num_params > 0 )
		{
		HBufC* param_name_str = HBufC::NewLC(KMaxParamNameLength);
		HBufC* param_value_str = HBufC::NewLC(KMaxParamValueLength);
		TPtr param_name_str_ptr(param_name_str->Des());
		TPtr param_value_str_ptr(param_value_str->Des());

		for( int i=1 ; i<=num_params && rv ; i++ )
			{
			TPtrC param_name;
			TPtrC param_value;

			param_name_str_ptr.Copy( KParamNameStr );
			// Append the parameter number to the end of the ini string
			param_name_str_ptr.AppendNum( i );
			rv = GetStringFromConfig( ConfigSection(), param_name_str_ptr, param_name );
			if( rv )
				{
				param_value_str_ptr.Copy( KParamValueStr );
				// Append the parameter number to the end of the ini string
				param_value_str_ptr.AppendNum( i );
				rv = GetStringFromConfig( ConfigSection(), param_value_str_ptr, param_value );
				}
			if( rv )
				{
				// Append the parameter name/value to the command line
				commandLinePtr.Append( KSpace );
				commandLinePtr.Append( param_name );
				commandLinePtr.Append( KEquals );
				commandLinePtr.Append( KQuote );
				commandLinePtr.Append( param_value );
				commandLinePtr.Append( KQuote );
				}
			}

		// Cleanup
		CleanupStack::PopAndDestroy(param_value_str);
		CleanupStack::PopAndDestroy(param_name_str);
		}

	// get the application control instance
	app = CreateApplicationControlInstance( &uid, EFalse );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		CleanupStack::PopAndDestroy(commandLine);
		return TestStepResult();
		}

	// Call runCommand
	HBufC8* commandLine8 = HBufC8::NewLC(commandLinePtr.Length()+1);
	TPtr8 ptr8(commandLine8->Des());
	ptr8.Copy(commandLinePtr);
	ret = app->runCommandL( (char*)ptr8.PtrZ(), &err );

	// Log the UCC/Service call result
	_LIT( KUCCCallResult, "UCC Call Result = %d" );
	INFO_PRINTF2( KUCCCallResult, ret );

	// Cleanup
	delete app;
	CleanupStack::PopAndDestroy(commandLine8);
	CleanupStack::PopAndDestroy(commandLine);

	// the following section represents an ultra lightweight way of tolerating handles returned by services, without assuming
	//  that any nonzero return code is a problem.
	//  in future this could be extended to store the specific handle values in a map to refer back to them in later test steps..
	//  something like "store_result_as_handle_called=Test1"
	TBool expectPositiveReturnCode(EFalse);
	rv = GetBoolFromConfig( ConfigSection(), KExpectPositiveReturnCodeStr, expectPositiveReturnCode );
	if(expectPositiveReturnCode && ret > 0)
		{
		if(ret>0) // a handle value was returned = success
			{
			ret=KErrNone;
			}
		else if(ret==0) // null handle means there was a problem
			{
			ret=KErrUnknown;
			}
		}

	// Return success / failure
	SetTestStepResult( (ret == KErrNone) ? EPass : EFail );
	return TestStepResult();
	}

CUCCTestSetDeviceID::CUCCTestSetDeviceID()
	{
	SetTestStepName(KTestSetDeviceID);
	}

CUCCTestSetDeviceID::~CUCCTestSetDeviceID()
	{
	}

TVerdict CUCCTestSetDeviceID::doTestStepL()
	{
	int uid;
	CUCCSDeviceControl *app;

	// get the application control instance and use-case id
	app = CreateApplicationControlInstance( &uid );
	if( app == NULL )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	
	// do the set
	SetTestStepResult( UpdateCommdbEntry( app ) );

	// cleanup and exit
	delete app;
	app = NULL;
	return TestStepResult();
	}

TVerdict CUCCTestSetDeviceID::UpdateCommdbEntry( CUCCSDeviceControl *aApp )
	{
	int ret, err = 0, isp_index, device_id;
	TCommDBError cdberr;
	CSetUpCommDb commdb_handle;
	TBuf<MAXLOGINSCRIPT> login_script_string;
	char device_id_string[MAXVARLEN];
	char isp_index_string[MAXVARLEN];

	// get the device id from the use-case script
	ret = aApp->getVariableName( "DEVICEID", device_id_string, MAXVARLEN, &err );
	if( ret != UCCS_OK )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	device_id = atoi(device_id_string);

	// get the ISP index from the use-case script
	ret = aApp->getVariableName( "COMMDB_ISP_INDEX", isp_index_string, MAXVARLEN, &err );
	if( ret != UCCS_OK )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	isp_index = atoi( isp_index_string );

	// create the entry string
	login_script_string.FillZ();
	login_script_string.Append( _L("CHARMAP [windows-1252]\n") );
	login_script_string.Append( _L("LOOP 10\n{\nSEND \"ID=") );
	login_script_string.AppendNum( device_id );
	login_script_string.Append( _L("\"+<0x0d>\nWAIT 3\n{\n\"SERVER(") );
	login_script_string.AppendNum( device_id );
	login_script_string.Append( _L(")\" OK\n}\n}\nEXIT KErrNoAnswer$\n\nOK:\nEXIT\n") );

	// set the value in COMMDB
	cdberr = commdb_handle.initialise();
	if( cdberr != TCommDB_Success )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	cdberr = commdb_handle.updateEntry( DIAL_OUT_ISP, isp_index, ISP_LOGIN_SCRIPT, (char*)login_script_string.PtrZ(), &err ); 
	if( cdberr != TCommDB_Success )
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	// done ik
	SetTestStepResult(EPass);
	return TestStepResult();
	}

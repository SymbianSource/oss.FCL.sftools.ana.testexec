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
* UCCS_ErrorCodes.cpp -- provides a function that translates error codes into
* a string description.
* System Includes
* Local Includes
*
*/



#include "UCCS_ErrorCodes.h"
#include "../../include/penstd.h"


/********************************************************************************
 *
 * GetPenstdErrorString()
 *
 ********************************************************************************/
char *GetPenstdErrorString( int aErrorCode )

{
	switch( aErrorCode ) {
	case ERR_NONE:
		return "success";
	case ERR_INVALID_ERROR_CODE:
		return "THIS IS AN INVALID ERROR CODE";
	case ERR_MISSING_PARAMETER:
		return "there is a missing parameter";
	case ERR_SERVICE_ALREADY_STARTED:
		return "service already started";
	case ERR_SERVICE_NOT_RUNNING:
		return "service not running";
	case ERR_ACTIVE_USER_SESSIONS:
		return "active user sessions exist";
	case ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS:
		return "failed to remove active user sessions";
	case ERR_FAILED_TO_CREATE_COMPONENT_MANAGER:
		return "failed to create the component manager";
	case ERR_FAILED_TO_RETRIEVE_KEY:
		return "failed to retrieve the instance key from the arguments";
	case ERR_INSTANCE_DOES_NOT_EXIST:
		return "instance does not exist";
	case ERR_INVALID_METHOD:
		return "invalid method";
	case ERR_CANNOT_CREATE_NEW_INSTANCE:
		return "can't create a new instance";
	case ERR_STUB_ALREADY_CONNECTED:
		return "stub already connected to the service";
	case ERR_FAILED_TO_CONNECT:
		return "failed to connect to the service";
	case ERR_STUB_NOT_CONNECTED:
		return "stub not connected to a service";
	case ERR_INVALID_RV_POINTER:
		return "invalid rv pointer";
	case ERR_RPC_ERROR:
		return "rpc error";
	case ERR_INVALID_SERVICE:
		return "invalid service";
	case ERR_INVALIDARG:
		return "invalid argument";
	case ERR_INVALIDSTATE:
		return "invalid state";
	case ERR_GENERAL:
		return "general error";
	case ERR_INVALID_CALL:
		return "invalid call";
	default:
		return "(unknown error)";
	}
	return "(unknown error)";
}


/********************************************************************************
 *
 * GetUccsErrorString()
 *
 ********************************************************************************/
char *GetUccsErrorString( TUCCSErrors aErrorCode )
{
	switch( aErrorCode ) {

	// Script and BatchEngine Errors
	case UCCS_OK:
		return "Success";
	case UCCS_SERVICELISTFULL:
		return "The maximum number of services have already been started";
    case UCCS_NO_MATCHING_GENERIC_SERVICE_FOUND:
		return "No matching generic service found.  Please check spelling against service defined in ucc.ini";
	case UCCS_NOSERVICEID:
		return "The command must specify the target service (SVCID)";
	case UCCS_INVALIDSERVICEID:
		return "The specified target service (SVCID) is invalid";
	case UCCS_CANTSTARTSERVICE:
		return "Failed to connect to the service";
	case UCCS_CANTSTOPSERVICE:
		return "Failed to stop the service";
	case UCCS_NOSERVICEHOST:
		return "The command must specify the hostname of the target service (SVCHOST)";
	case UCCS_INVALIDSERVICEINDEX:
		return "INTERNAL_ERROR_CODE: UCCS_INVALIDSERVICEINDEX";
	case UCCS_PARSERFAILEDTOSTOREVALUE:
		return "An error occured while parsing the script command";
	case UCCS_UNKNOWNCOMMAND:
		return "This is not a known command";
	case UCCS_REPLAYCOMMAND:
		return "INTERNAL_ERROR_CODE: UCCS_REPLAYCOMMAND";
	case UCCS_NORESULT:
		return "No commands (runcmd) have been completed, therefore there is no result to check/waitfor";
	case UCCS_QUIT:
		return "INTERNAL_ERROR_CODE: UCCS_QUIT";
	case UCCS_REQUIREDVALUEERROR:
		return "Some of the required fields do not exist";
	case UCCS_REQUIREDVALUEINCORRECT:
		return "Some of the required field values do not match those returned";
	case UCCS_REQUIREDNOTVALUEERROR:
		return "Some of the required (not) fields do not exist";
	case UCCS_REQUIREDNOTVALUEMATCH:
		return "Some of the required (not) field values match those returned";
	case UCCS_CHECKVALUEERROR:
		return "Some of the check fields do not exist";
	case UCCS_CHECKVALUEINCORRECT:
		return "Some of the check field values do not match those returned";
	case UCCS_CHECKNOTVALUEERROR:
		return "Some of the check (not) fields do not exist";
	case UCCS_CHECKNOTVALUEMATCH:
		return "Some of the check (not) field values match those returned";
	case UCCS_FIELDNOTFOUND:
		return "An expected record field was not found";
	case UCCS_SYSTEMERROR:
		return "An system call has failed";
	case UCCS_RECORDFULL:
		return "The record is full";
	case UCCS_NOMORECOMMANDS:
		return "INTERNAL_ERROR: UCCS_NOMORECOMMANDS";
	case UCCS_UNKNOWNCONTROLCOMMAND:
		return "An unknown control command was received";
	case UCCS_CLIENT_ERROR:
		return "The client failed to execute the command";

	// Control Command Errors
	case UCCS_ALREADYSTARTEDUSECASE:
		return "There is already an active use-case";
	case UCCS_NOUSECASERUNNING:
		return "There is no use-case active";
	case UCCS_FAILEDTOCREATEEXECUTETHREAD:
		return "SYSTEM_ERROR: The controller couldn't create a thread to execute the script";
	case UCCS_COMMANDEXECUTIONNOTSTARTEDYET:
		return "A use-case is being initialised";
	case UCCS_SCRIPTFINISHED:
		return "The active use-case script has completed";
	case UCCS_CANTOPENSCRIPTFILE:
		return "Failed to open the requested use-case script file";
	case UCCS_CANTOPENOUTPUTFILE:
		return "Failed to open the output files";
	case UCCS_VARIABLEVALTOOLONG:
		return "The buffer supplied to GetEnvironmentVariable is too small to hold the requested value";
	case UCCS_VARIABLEDOESNOTEXIST:
		return "The requested field does not exist in the current environment";
	case UCCS_NOENVIRONMENT:
		return "INTERNAL_ERROR: UCCS_NOENVIRONMENT";

	// Other
	case UCCS_LAST:
		return "INTERNAL_ERROR: UCCS_LAST";
	default:
		// check for penstd error values
		// - not sure if they are intended to come through as UCCS errors, but alas they do anyway
		//   eg. -17 = missing param --> re-producible by having a mismatch in paramaeter names
		return GetPenstdErrorString(aErrorCode);
	}	
}

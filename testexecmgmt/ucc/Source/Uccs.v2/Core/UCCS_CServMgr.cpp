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
* Filename: UCCSServMgr.cpp
* System Includes
*
*/



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "strncpynt.h"
#include "UCCS_CServMgr.h"
#include "UCCS_ErrorCodes.h"
#include "UCCS_ServiceValues.h"
#include "../ServiceStubs/Mobster.v2/CMobsterStub.h"
#include "../ServiceStubs/Internal/CInternalStub.h"
#include "../ServiceStubs/Ppp/CPppControllerStub.h"
#include "../ServiceStubs/HostExecute/CHostExecuteStub.h"
#include "../ServiceStubs/UuInterface/CUuinterfaceStub.h"
#include "../ServiceStubs/MobileAgent/CMobileAgentServiceStub.h"
#include "../ServiceStubs/ForeignAgent/CForeignAgentServiceStub.h"
#include "../ServiceStubs/HomeAgent/CHomeAgentServiceStub.h"
#include "../ServiceStubs/HostExecuteAsync/CHostExecuteAsyncStub.h"
#include "../ServiceStubs/Test/CTestStub.h"
//#include "..\ServiceStubs\TestService\CTestServiceStub.h"
#include "../ServiceStubs/GPSSimulator/CAPICommandHandlerStub.h"
#include "../ServiceStubs/GenericStub/CGenericStub.h"
//#include "..\..\include\penstd.h"
#include "CUCCIniFile.h"

/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define UCC_INI_FILE	".\\UCC.ini"
#define SERVICE_INI		"ini"
#define SERVICE_HOST	"host"

#define STARTUP_GENERIC_SERVICE_STUB(rpcid,ini,servicename) \
			service = iServiceList[localserviceindex] = new CGenericStub(ini,servicename);	\
			assert( service != NULL );														\
			localerror = remoteerror = 0;													\
			rv = service->StartUccsService( aServiceHost, &localerror, &remoteerror );		\
			if( rv != 0 ) {																	\
				delete iServiceList[localserviceindex];										\
				iServiceList[localserviceindex] = NULL;										\
			} else {																		\
				strcpy( iServiceDescTable[localserviceindex].iHostname, aServiceHost );		\
				iServiceDescTable[localserviceindex].iServiceID = rpcid;					\
			}																				\
			break;																

#define STARTUP_SERVICE_STUB(rpcid,classname) \
			service = iServiceList[localserviceindex] = new classname();	\
			assert( service != NULL );													\
			localerror = remoteerror = 0;												\
			rv = service->StartUccsService( aServiceHost, &localerror, &remoteerror );	\
			if( rv != 0 ) {																\
				delete iServiceList[localserviceindex];									\
				iServiceList[localserviceindex] = NULL;									\
			} else {																	\
				strcpy( iServiceDescTable[localserviceindex].iHostname, aServiceHost );	\
				iServiceDescTable[localserviceindex].iServiceID = rpcid;				\
			}

#define STARTUP_SERVICE_STUB_CASE(rpcid,classname)	case rpcid: \
			STARTUP_SERVICE_STUB(rpcid, classname);				\
			break;																

#define SHUTDOWN_SERVICE_STUB(rpcid,classname) \
			delete ((classname*)(iServiceList[aLocalServiceID]));		\
			iServiceList[aLocalServiceID] = NULL;						\
			iServiceDescTable[aLocalServiceID].iHostname[0] = 0;		\
			iServiceDescTable[aLocalServiceID].iServiceID = RPCSVC_INVALID;

#define SHUTDOWN_SERVICE_STUB_CASE(rpcid,classname)		case rpcid:			\
			SHUTDOWN_SERVICE_STUB(rpcid, classname);						\
			break;

/***********************************************************************************
 *
 * Construction 
 *
 **********************************************************************************/
CServiceManager::CServiceManager( IOutput *aOutput )
	: iNumGenericServices(0)
{
	// verify the params
	assert( aOutput != NULL );
	iOutput = aOutput;

	// initialise the service list
	for( int i = 0; i <= MAXSERVICES; i++ ) {
		iServiceList[i] = NULL;
	}

	// *** This need refactoring into a private method ***
	// Open the ucc.ini file
	CUCCIniFile iniFile(UCC_INI_FILE);
	vector<string> sections = iniFile.SectionNames();

	// Check to see if there are any sections defined
	iNumGenericServices = sections.size();
	if( iNumGenericServices > 0 && iNumGenericServices<(MAXSERVICES-RPCSVC_LAST) )
	{
		for( int i=0 ; i<iNumGenericServices ; i++ )
		{
			// *** No error checking currently ***

			// Set the service IID to the next available one
			iGenericServices[i].iServiceID = RPCSVC_LAST+i;

			// Set the service name to the section name
			strcpy( iGenericServices[i].iServiceName, sections[i].c_str() );

			// Retrieve the ini file location
			string service_ini;
			bool ret = iniFile.KeyValue(SERVICE_INI, sections[i].c_str(), service_ini);
			assert( ret == true );
			assert( service_ini.size() > 0 );
			strcpy( iGenericServices[i].iIniFile, service_ini.c_str() );

			// Retrieve the host name/ip address
			string host;
			ret = iniFile.KeyValue(SERVICE_HOST, sections[i].c_str(), host);
			assert( ret == true );
			assert( host.size() > 0 );
			strcpy( iGenericServices[i].iHost, host.c_str() );
		}
	}
}


/***********************************************************************************
 *
 * Destruction
 *
 **********************************************************************************/
CServiceManager::~CServiceManager()
{
	// shutdown all loaded services -- log any errors
	ShutdownAll();

	// verify all services are stopped (or trying to connect)
	for( int i = 0; i < MAXSERVICES; i++ ) {
		assert( (iServiceList[i] == NULL) || (iServiceDescTable[i].iServiceID == RPCSVC_INVALID) );
	}
}


/***********************************************************************************
 *
 * ShutdownAll - Close the connections to all services
 *
 **********************************************************************************/
int CServiceManager::ShutdownAll()
{
	int localserviceindex = 0;

	//check each service and if it is alive then stop it
	for( localserviceindex = 0; localserviceindex < MAXSERVICES; localserviceindex++ ) {
		if( iServiceList[localserviceindex] != NULL ) {
			LocalStopService( localserviceindex );
		}
	}
	return UCCS_OK;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: ResetService -- resets a service by starting then stopping it
 *
 **********************************************************************************/
int CServiceManager::ResetService( int aServiceID, char *aHostname )
{
	assert( !"Method withdrawn" );
	return 0;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: ResetAllServices -- resets all the services in the oldservices
 * list
 *
 **********************************************************************************/
int CServiceManager::ResetAllServices()
{
	assert( !"Method withdrawn" );
	return 0;
}


/***********************************************************************************
 *
 * PUBLIC METHOD: IssueCommand -- sends the command to the appropriate service
 *
 **********************************************************************************/
int CServiceManager::IssueCommand( CDataRecord* aRequestRecord, CDataRecord** aReturnedDataRecord )
{
	int returncode = ERR_NONE;
	int service_id;
	int err;
	int service_index;
	char *service_name;
	char *service_host;
	IService *service = NULL;

	// ensure that the passed buffer is set to NULL
	assert( *aReturnedDataRecord == NULL );


	// Look for the service name initially
	//   if the name doesn't exist then revert to the legacy method
	err = aRequestRecord->GetFieldAsString( "SVCNAME", &service_name );
	if( err == UCCS_OK ) {
		// If a generic service is being used then retrieve the 
		// service id and service name from the generic services list.
		bool found = false;
		for( int i=0; i<iNumGenericServices; i++ )
		{
			if( strcmp(iGenericServices[i].iServiceName, service_name) == 0 )
			{
				service_id = iGenericServices[i].iServiceID;
				service_host = iGenericServices[i].iHost;
				found = true;
				break;
			}
		}
        if (!found)
            return UCCS_NO_MATCHING_GENERIC_SERVICE_FOUND;
	}
	else
	{
		// retrieve the service id for the request 
		err = aRequestRecord->GetFieldAsInt( "SVCID", &service_id );
		if( err != UCCS_OK ) {
			return UCCS_NOSERVICEID;
		}

		// verify that this is a valid service id 
		if( (service_id <= RPCSVC_INVALID) || (service_id >= RPCSVC_LAST+iNumGenericServices) ) {
			return UCCS_INVALIDSERVICEID;
		}

		// retrieve the hostname for the request
		err = aRequestRecord->GetFieldAsString( "SVCHOST", &service_host ); 
		if( err != UCCS_OK ) {
			return UCCS_NOSERVICEHOST;
		}
	}

	// execute command
    // - performed within a loop to provide for a retry mechanism (eg. after new RPC connetion established)
    for (int i=0; i < 2; i++)
    {
	    service_index = GetLocalServiceIndex( service_id, service_host );
	    if( service_index == -1 )
        {
            // start service (RPC connect)
		    err = LocalStartService(service_id, service_host);
		    if (err != UCCS_OK) 
			    return err;
    	    service_index = GetLocalServiceIndex( service_id, service_host );
	    }

	    // get a pointer to the service 
	    assert( service_index != -1 );	
	    service = iServiceList[service_index];
	    assert( service != NULL );	

	    // now issue the command to the service
	    *aReturnedDataRecord = service->IssueCommand( aRequestRecord );

	    // Check the completion code and result
	    int completion_code = ERR_NONE;
	    err = (*aReturnedDataRecord)->GetFieldAsInt( STD_REPLY_FIELD_REQUESTCOMPLETIONCODE, &completion_code );
	    if( err == UCCS_OK )
	    {
		    // If the completion code is ok, check the call result
		    if( completion_code == ERR_NONE )
		    {
			    int call_result = ERR_NONE;
			    err = (*aReturnedDataRecord)->GetFieldAsInt( "RESULT", &call_result );
			    if( err == UCCS_OK )
			    {
				    returncode = call_result;
                    break;  // no need to retry since it worked (either first or second time)
			    }
		    }
		    else
		    {
			    returncode = completion_code;
                err = LocalStopService(service_index);
		    }
	    }
    } // for loop

	return returncode;
}


/***********************************************************************************
 *
 * PRIVATE METHOD: StartService - creates and starts up a service. The passed id
 * must map to an entry in the service table which stores the RPC service and 
 * the host that the service is targetted at. 
 *
 **********************************************************************************/
int CServiceManager::LocalStartService( int aServiceID, char *aServiceHost )
{
	int rv;
	int i;
	int localerror = 0;
	int remoteerror = 0;
	int localserviceindex = -1;
	IService *service;

	// look for a free index in the service table
	for( i = 0; i < MAXSERVICES; i++ ) {
		if( iServiceList[i] == NULL ) {
			localserviceindex = i;
			break;
		}
	}

	// check that a slot was found
	if( localserviceindex == -1 ) {
		return UCCS_SERVICELISTFULL;
	}
	
	// startup each legacy service
	switch( aServiceID ) {

	STARTUP_SERVICE_STUB_CASE( RPCSVC_HOMEAGENT,		CHomeAgentServiceStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_FOREIGNAGENT,		CForeignAgentServiceStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_MOBILEAGENT,		CMobileAgentServiceStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_MOBSTER,			CMobsterServiceStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_UUINTERFACE,		CUuinterfaceStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_HOSTEXECUTE,		CHostExecuteStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_PPPCONTROLLER,	CPppControllerServiceStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_INTERNAL,			CInternalStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_HOSTEXECUTEASYNC,	CHostExecuteAsyncStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_TEST,				CTestStub );
	STARTUP_SERVICE_STUB_CASE( RPCSVC_GPSSIMULATOR,		CAPICommandHandlerStub );

	default:
		{
			// Check to see if it is a generic service stub
			if( (aServiceID >= RPCSVC_LAST) &&
				(aServiceID <= RPCSVC_LAST+iNumGenericServices) )
			{
				STARTUP_GENERIC_SERVICE_STUB(	(TRpcServiceID)aServiceID,
												iGenericServices[aServiceID-RPCSVC_LAST].iIniFile,
												iGenericServices[aServiceID-RPCSVC_LAST].iServiceName);
			}
			else
			{
				rv = ERR_INVALID_SERVICE;
			}
		}
		break;
	}

	// update the status
	iOutput->StartServiceResult( aServiceID, aServiceHost, rv, localerror, 0 );
	return ((rv == ERR_NONE) ? UCCS_OK : UCCS_CANTSTARTSERVICE );
}


/***********************************************************************************
 *
 * PRIVATE METHOD: StopService - destroys the service object
 *
 **********************************************************************************/
int CServiceManager::LocalStopService( int aLocalServiceID )
{
	int err;
	int localerror;
	int remoteerror;
	TServiceTableEntry this_entry;

	// check parameters
	assert( (aLocalServiceID >= 0) && (aLocalServiceID < MAXSERVICES) );
	localerror = remoteerror = 0;


	// get the service
	if( iServiceList[aLocalServiceID] == NULL ) {
		return UCCS_INVALIDSERVICEINDEX;
	}

	// save this entry in-case 
	this_entry.iServiceID = iServiceDescTable[aLocalServiceID].iServiceID;
	STRNCPY_NULL_TERMINATE( this_entry.iHostname, iServiceDescTable[aLocalServiceID].iHostname, MAXHOSTNAME );

	// stop the service and log the result
	err = (iServiceList[aLocalServiceID])->StopUccsService( &localerror, &remoteerror );

	// regardless of success we still want to destroy the object
	switch( iServiceDescTable[aLocalServiceID].iServiceID ) {

	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_HOMEAGENT,			CHomeAgentServiceStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_FOREIGNAGENT,		CForeignAgentServiceStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_MOBILEAGENT,			CMobileAgentServiceStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_MOBSTER,				CMobsterServiceStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_UUINTERFACE,			CUuinterfaceStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_HOSTEXECUTE,			CHostExecuteStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_PPPCONTROLLER,		CPppControllerServiceStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_INTERNAL,			CInternalStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_HOSTEXECUTEASYNC,	CHostExecuteAsyncStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_TEST,				CTestStub );
	SHUTDOWN_SERVICE_STUB_CASE( RPCSVC_GPSSIMULATOR,		CAPICommandHandlerStub );

	// this means that the other thread is trying to connect to the service as 
	// we speak - if we just ignore this then it should be ok?
	case RPCSVC_INVALID:
		break;

	default:
		{
			// Check to see if it is a generic service stub
			if( (iServiceDescTable[aLocalServiceID].iServiceID >= RPCSVC_LAST) &&
				(iServiceDescTable[aLocalServiceID].iServiceID <= RPCSVC_LAST+iNumGenericServices) )
			{
				SHUTDOWN_SERVICE_STUB( iServiceDescTable[aLocalServiceID].iServiceID, CGenericStub );
			}
			else
			{
				assert( !"INVALID CODE PATH" );
			}
		}
		break;
	}

	// update the status
	iOutput->StopServiceResult( this_entry.iServiceID, this_entry.iHostname, err, localerror, remoteerror );
	return ((err == ERR_NONE) ? UCCS_OK : UCCS_CANTSTOPSERVICE );
}


/***********************************************************************************
 *
 * PRIVATE METHOD: GetLocalServiceIndex 
 *
 **********************************************************************************/
int CServiceManager::GetLocalServiceIndex( int aServiceID, char *aServiceHost )
{
	int i;
	int match;

	// check params
	assert( aServiceHost != NULL );
	assert( (aServiceID > RPCSVC_INVALID) && (aServiceID < RPCSVC_LAST+iNumGenericServices) );

	// now look for a match
	for( i = 0; i < MAXSERVICES; i++ ) {

		// make sure this index is active
		if( iServiceList[i] == NULL ) {
			continue;
		}

		// try and match
		if( aServiceID != iServiceDescTable[i].iServiceID ) {
			continue;
		}
		match = strcmp( iServiceDescTable[i].iHostname, aServiceHost );
		if( match != 0 ) {
			continue;
		}

		// we have a match
		return i;
	}

	// done -- failed
	return -1;
}





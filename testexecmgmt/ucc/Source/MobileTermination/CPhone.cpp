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
* System Includes
*
*/



#include <stdio.h>
#include <assert.h>
#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#else
#include <winsock2.h>
#include <windows.h>
#endif

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CPhone.h"

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define POLLINTERVAL 1000 

#define CLEANUP_SOURCE_AIR_INTERFACE_START_FAILED	1
#define CLEANUP_SOURCE_TE_CHANNEL_START_FAILED		2
#define CLEANUP_SOURCE_INTERNAL_RUN_PHONE			3
#define CLEANUP_SOURCE_MAIN_START_FAILED			4


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/
#ifndef WIN32
#define closesocket(x) (shutdown(x,SHUT_RDWR),close(x))
#endif

/*******************************************************************************
 *
 * Macro functions
 *
 ******************************************************************************/
#ifndef WIN32
#define Sleep(x) sleep(x/1000)
#endif

/*******************************************************************************
 *
 * Prototypes
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * Construction
 *
 ******************************************************************************/
CPhone::CPhone() : iUdpAirInterface(&iPhoneData,&iLog), iTcpTeChannel(&iPhoneData,&iLog), iFilterPpp(&iPhoneData,&iLog), iMainThread("MainThread"), iAirInterfaceThread("AirInterfaceThread"), iTEChannelThread("TeChannelThread")
{
	// just need to set all the pointers to null
	iFilter = NULL;
	iDatalinkPacketise = NULL;
	iDatalinkNull = NULL;
	iProcessData = NULL;
	iExitFlag = 0;
	iStatus = MTS_INIT;

	// clear the phone state
	memset( &iPhoneData, 0, sizeof(iPhoneData) );
}

CPhone::~CPhone()
{
	// just need to check that all the pointers are freed
	assert( iFilter == NULL );
	assert( iDatalinkPacketise == NULL );
	assert( iDatalinkNull == NULL );
	assert( iProcessData == NULL );
	assert( iStatus != MTS_RUNNING );
}


/*******************************************************************************
 *
 * PRIVATE METHOD: MAIN-THREAD: InternalInitialisePhone - setup everything - if 
 * this returns an error all resource MUST be cleaned up.
 *
 ******************************************************************************/
MTError CPhone::InternalInitialisePhone( int aPhoneID, int aDatalinkConfig, int aFilterConfig, int *aErrCode )
{
	MTError merr;
	TThreadError terr;

	// check the params
	assert( aPhoneID >= 0 );
	assert( aErrCode != NULL );

	// init the errors
	*aErrCode = 0;

	// setup the phone state 
	iPhoneData.iPhoneID = aPhoneID;

	// create the appropriate filters
	merr = CreateFilters( aFilterConfig );
	if( merr != MTL_SUCCESS ) {
		return merr;
	}

	// create the data link object
	merr = CreateDatalinkLayer( aDatalinkConfig );
	if( merr != MTL_SUCCESS ) {
		DeleteFilters();
		return merr;
	}

	// give the datalink object pointers to the air interface and the uu interface
	iProcessData->SetAirInterface( &iUdpAirInterface );
	iProcessData->SetTEChannel( &iTcpTeChannel );

	// give each channel a pointer to the datalink
	iUdpAirInterface.SetDatalink( iProcessData );
	iTcpTeChannel.SetDatalink( iProcessData );
	iUdpAirInterface.SetFilter( iFilter );
	iTcpTeChannel.SetFilter( iFilter );

	// create a thread for the air interface to listen
	terr = iAirInterfaceThread.StartThread( (void*)AirInterfaceThreadProc, this, aErrCode );
	if( terr != TE_NONE ) {
		CleanupState( CLEANUP_SOURCE_AIR_INTERFACE_START_FAILED );
		return MTL_FAILED_TO_CREATE_AIR_INTERFACE_THREAD;
	}

	// create a thread for the te channel to listen
	terr = iTEChannelThread.StartThread( (void*)TEChannelThreadProc, this, aErrCode );
	if( terr != TE_NONE ) {
		CleanupState( CLEANUP_SOURCE_TE_CHANNEL_START_FAILED );
		return MTL_FAILED_TO_CREATE_TE_CHANNEL_THREAD;
	}

	// cool, we are setup and ready to go
	return MTL_SUCCESS;
}


/*******************************************************************************
 *
 * PRIVATE METHOD: MAIN-THREAD: InternalRunPhone - the main execution loop for 
 * the main thread of the phone - if this returns error then it MUST be cleaned 
 * up.
 *
 ******************************************************************************/
MTError CPhone::InternalRunPhone( int *aErrCode )
{
	TThreadError terr;
	
	// wait for the threads to complete or for a command to stop them
	while( 1 ) {

		// perform checks every X milliseconds
		Sleep( POLLINTERVAL );

		// check the state of the air_interface thread
		terr = iAirInterfaceThread.WaitForThread( 0 );
		if( terr == TE_NONE ) {
		  break;
		}
		assert( terr == TE_TIMEOUT );

		// check the state of the channel thread
		terr = iTEChannelThread.WaitForThread( 0 );
		if( terr == TE_NONE ) {
			break;
		}
		assert( terr == TE_TIMEOUT );

		// check whether the external program has requested that we shutdown
		if( iExitFlag != 0 ) {
			break;
		}
	}

	// cleanup everything
	CleanupState( CLEANUP_SOURCE_INTERNAL_RUN_PHONE );

	// done
	return MTL_SUCCESS;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: StartPhone - this wraps up init phone and run phone
 *
 ******************************************************************************/
MTError CPhone::StartPhone( int aPhoneID, int aDatalinkConfig, int aFilterConfig, int *aErrCode )
{
	MTError merr;
	TThreadError terr;

	// check params
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// check the state
	if( iStatus != MTS_INIT ) {
		return MTL_INVALID_STATE;
	}

	// initialise all the sub-components
	merr = InternalInitialisePhone( aPhoneID, aDatalinkConfig, aFilterConfig, aErrCode );
	if( merr != 0 ) {
		return merr;
	}

	// set the state
	iStatus = MTS_RUNNING;

	// start up a new thread that calls InternalRunPhone
	terr = iMainThread.StartThread( (void*)MainThreadProc, this, aErrCode );
	if( terr != TE_NONE ) {
		CleanupState( CLEANUP_SOURCE_MAIN_START_FAILED );
		return MTL_FAILED_TO_CREATE_MAIN_THREAD;
	}

	// ok everything is running -- return ok
	return MTL_SUCCESS;
}	


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: StopPhone - signal the main handler to stop 
 * everything
 *
 ******************************************************************************/
MTError CPhone::StopPhone()
{
	MTStatus mstatus;

	// set the exit flag
	iExitFlag = 1;

	// depends on our state
	switch( iStatus ) {
	case MTS_INIT:
		return MTL_SUCCESS;
	
	case MTS_RUNNING:
	case MTS_SHUTDOWN_ALL_BUT_MAIN:

		while( 1 ) {
			mstatus = GetStatus();
			if( mstatus == MTS_SHUTDOWN_ALL ) {
				return MTL_SUCCESS;
			}
			Sleep( 500 );
		}
		break;

	case MTS_SHUTDOWN_ALL:
		return MTL_SUCCESS;
	}

	// should never get here
	assert( !"INVALID CODE PATH" );
	return MTL_SUCCESS;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: Sets the remote address for the uu interface
 *
 ******************************************************************************/
MTError CPhone::SetRemoteUUAddress( struct sockaddr_in sockaddr )
{
	iUdpAirInterface.SetRemoteAddress( sockaddr );
	return MTL_SUCCESS;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: Gets the remote address for the uu interface
 *
 ******************************************************************************/
MTError CPhone::GetRemoteUUAddress( struct sockaddr_in *sockaddr )
{
	iUdpAirInterface.GetRemoteAddress( sockaddr );
	return MTL_SUCCESS;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: Gets the local address for the uu interface.
 *
 ******************************************************************************/
MTError CPhone::GetLocalUUAddress( struct sockaddr_in *sockaddr )
{
	assert( sockaddr != NULL );
	iUdpAirInterface.GetLocalAddress( sockaddr );
	return MTL_SUCCESS;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: SERVER-THREAD: Sets the socket to be used by the TE channel.
 *
 ******************************************************************************/
MTError CPhone::SetTeSocket( int aSock )
{
	TChannelError cerr;
	cerr = iTcpTeChannel.SetSocket( aSock );
	assert( (cerr == CE_NONE) || (cerr == CE_SOCKET_ALREADY_SET) );
	return ((cerr == CE_NONE) ? MTL_SUCCESS : MTL_TE_CHANNEL_SOCKET_ALREADY_SET);
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: Returns a pointer to the log.
 *
 ******************************************************************************/
CLog *CPhone::GetLog()
{
	return &iLog;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: RPC-THREAD: Gets the current status of the MT.
 *
 ******************************************************************************/
MTStatus CPhone::GetStatus()
{
	TThreadError terr;

	// if the status is MTS_SHUTDOWN_ALL_BUT_MAIN then we check to see if the 
	// main thread has exited and then we update it
	if( iStatus == MTS_SHUTDOWN_ALL_BUT_MAIN ) {
		terr = iMainThread.WaitForThread( 0 );
		if( terr == TE_NONE ) {
			iStatus = MTS_SHUTDOWN_ALL;
		}
	}

	// return the status
	return iStatus;
}


/*******************************************************************************
 *
 * Cleanupstate - cleans up everything
 *
 ******************************************************************************/
void CPhone::CleanupState( int aRequestSource )
{
	int err;
	TThreadState thread_state;
	TThreadError terr;

	// if the air interface thread is still listening then stop it
	thread_state = iAirInterfaceThread.GetThreadState();
	if( thread_state == TS_ACTIVE ) {
		err = iUdpAirInterface.StopInterface();
		assert( err == 0 );
		terr = iAirInterfaceThread.WaitForThread( INFINITE );
		assert( terr == TE_NONE );
	}

	// if the te channel thread is still listening then stop it
	thread_state = iTEChannelThread.GetThreadState();
	if( thread_state == TS_ACTIVE ) {
		iTcpTeChannel.StopChannel();
		terr = iTEChannelThread.WaitForThread( INFINITE );
		assert( terr == TE_NONE );
	}

	// Remove the datalink layer
	DeleteDatalinkLayer();

	// Remove the fitler
	DeleteFilters();

	// update the status
	thread_state = iMainThread.GetThreadState();
	iStatus = ((thread_state == TS_ACTIVE) ? MTS_SHUTDOWN_ALL_BUT_MAIN : MTS_SHUTDOWN_ALL);

	// verification
	assert( (iStatus == MTS_SHUTDOWN_ALL) || (aRequestSource == CLEANUP_SOURCE_INTERNAL_RUN_PHONE) );
}


/*******************************************************************************
 *
 * SECTION: Helpers
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * Create And Delete filters layers 
 *
 ******************************************************************************/
MTError CPhone::CreateFilters( int aFilterConfig )
{
	// if the config isn't zero or one then we have an error
	if( (aFilterConfig != 0) && (aFilterConfig != 1) ) {
		return MTL_INVALID_FILTER_CONFIG;
	}

	// the only filter is the ppp logger filter
	if( aFilterConfig == FILTER_PPP ) {
		iFilter = &iFilterPpp;
	}

	// done
	return MTL_SUCCESS;
}

void CPhone::DeleteFilters()
{
	// just set the pointer to NULL
	iFilter = NULL;
}


/*******************************************************************************
 *
 * CreateAndDelete Datalink layers - these are the methods who know the class 
 * (rather than the interface) of the datalink layer.
 *
 ******************************************************************************/
MTError CPhone::CreateDatalinkLayer( int aDatalinkConfig )
{
	// create the appropriate datalink layer object and set the process data pointer
	if( aDatalinkConfig == DL_NULL ) {
		iDatalinkNull = new CDatalinkNull( &iPhoneData, &iLog );
		assert( iDatalinkNull != NULL );
		iProcessData = iDatalinkNull;
	} else if( aDatalinkConfig == DL_PACKETISE ) {
		iDatalinkPacketise = new CDatalinkPacketise( &iPhoneData, &iLog );
		assert( iDatalinkPacketise != NULL );
		iProcessData = iDatalinkPacketise;
	} else {
		return MTL_INVALID_DATALINK_LAYER;
	}

	// success
	return MTL_SUCCESS;
}

void CPhone::DeleteDatalinkLayer()
{
	// delete whichever datalink layers are active 
	if( iDatalinkNull != NULL ) {
		delete iDatalinkNull;
		iDatalinkNull = NULL;
		iProcessData = NULL;
	}

	if( iDatalinkPacketise != NULL ) {
		delete iDatalinkPacketise;
		iDatalinkPacketise = NULL;
		iProcessData = NULL;
	}
}


/*******************************************************************************
 *
 * SECTION: Thread entry procedures
 *
 ******************************************************************************/
	
/*******************************************************************************
 *
 * Thread Entry Procedures
 *
 ******************************************************************************/
int MainThreadProc( CPhone *aPhone )
{
	MTError merr;
	CLog *log;
	int errcode;

	// check the param
	assert( aPhone != NULL );

	// log
	log = &(aPhone->iLog);
	log->WriteLogEntry( SV_INFO, "MainThreadProc", "Started" );

	// now call startup
	merr = aPhone->InternalRunPhone( &errcode );

	// the result is logged
	if( merr != MTL_SUCCESS ) {
		log->WriteLogEntry( SV_WARNING, "MainThreadProc", "InternalRunPhone returned error", merr, errcode );
	}

	// log
	log->WriteLogEntry( SV_INFO, "MainThreadProc", "Stopped" );
	return MTL_SUCCESS;
}


int AirInterfaceThreadProc( CPhone *aPhone )
{
	TAirInterfaceError aerr;
	int errcode;
	CUDPAirInterface *air_interface;
	CLog *log;

	// check the param
	assert( aPhone != NULL );

	// log
	log = &(aPhone->iLog);
	log->WriteLogEntry( SV_INFO, "AirInterfaceThreadProc", "Started" );

	// now recv from the air interface
	air_interface = &(aPhone->iUdpAirInterface);
	aerr = air_interface->ListenOnInterface( &errcode );

	// the result is logged
	if( aerr != AIE_NONE ) {
		log->WriteLogEntry( SV_WARNING, "AirInterfaceThreadProc", "ListenOnInterface returned error", aerr, errcode );
	}

	// log
	log->WriteLogEntry( SV_INFO, "AirInterfaceThreadProc", "Stopped" );
	return MTL_SUCCESS;
}

int TEChannelThreadProc( CPhone *aPhone )
{
	TChannelError cerr;
	int errcode;
	CTcpTeChannel *te_channel;
	CLog *log;

	// check the param
	assert( aPhone != NULL );

	// log
	log = &(aPhone->iLog);
	log->WriteLogEntry( SV_INFO, "TEChannelThreadProc", "Started" );

	// now recv from the channel
	te_channel = &(aPhone->iTcpTeChannel);
	cerr = te_channel->ListenOnChannel( &errcode );

	// the result is logged
	if( cerr != CE_NONE ) {
		log->WriteLogEntry( SV_WARNING, "TEChannelThreadProc", "ListenOnChannel returned error", cerr, errcode );
	}

	// log
	log->WriteLogEntry( SV_INFO, "TEChannelThreadProc", "Stopped" );
	return MTL_SUCCESS;
}

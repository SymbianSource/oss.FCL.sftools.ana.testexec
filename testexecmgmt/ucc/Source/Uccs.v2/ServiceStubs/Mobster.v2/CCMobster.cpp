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
#include <rpc/types.h>


/****************************************************************************************
 * 
 * Local Includes
 * 
 ***************************************************************************************/
#include "CCMobster.h"


/****************************************************************************************
 * 
 * Implementation
 * 
 ***************************************************************************************/
CCMobster::CCMobster()
{
	cl = NULL;
	iLastRPCError.re_status = RPC_SUCCESS;
}

CCMobster::~CCMobster()
{
	assert( cl == NULL );
}

char *CCMobster::GetLastRPCError( int *aIntErr )
{
	struct rpc_err rpcerr;

	// check that the handle is valid
	if( cl == NULL ) {
		return NULL;
	}

	// pass the aIntErr
	if( aIntErr != NULL ) {
		clnt_geterr( cl, &rpcerr );
		*aIntErr = rpcerr.re_status;
	}

	// return the errorstring
	return clnt_sperror( cl, NULL );
}

int CCMobster::Connect( string aRemoteHost )
{
	// check that we are not already connected
	if( cl != NULL ) {
		return ERR_STUB_ALREADY_CONNECTED;
	}

	// start the rpc library
	rpc_nt_init();

	// connect to the service
	cl = clnt_create( aRemoteHost.c_str(), MOBSTER, MOBSTER_VERSION, "tcp" );
	if( cl == NULL ) {
		rpc_nt_exit();
		return ERR_FAILED_TO_CONNECT;
	}

	// done
	return ERR_NONE;
}

int CCMobster::Disconnect( )
{
	// check that we are connected
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// cleanup the client handle
	clnt_destroy( cl );
	cl = NULL;
	rpc_nt_exit();

	// done
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: ss_startuprpcservice
 * 
 ***************************************************************************************/
int CCMobster::ss_startuprpcservice( struct TChannelAddress aArgs, int *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *ss_startuprpcservice_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: sc_shutdownrpcservice
 * 
 ***************************************************************************************/
int CCMobster::sc_shutdownrpcservice( int aArgs, int *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *sc_shutdownrpcservice_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: list_devices
 * 
 ***************************************************************************************/
int CCMobster::list_devices( TComponentList *rv )
{
	struct rpc_err rerr;
	int aArgs = 0;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *list_devices_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: cstr_startdevice
 * 
 ***************************************************************************************/
int CCMobster::cstr_startdevice( TDeviceDesc aArgs, int *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *cstr_startdevice_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: dstr_removedevice
 * 
 ***************************************************************************************/
int CCMobster::dstr_removedevice( int aArgs, int *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *dstr_removedevice_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: getdeviceinfo
 * 
 ***************************************************************************************/
int CCMobster::getdeviceinfo( int aArgs, TDeviceDesc *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *getdeviceinfo_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: getdevicelog
 * 
 ***************************************************************************************/
int CCMobster::getdevicelog( int aArgs, TVarData *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *getdevicelog_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: stopdevice
 * 
 ***************************************************************************************/
int CCMobster::stopdevice( int aArgs, int *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *stopdevice_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}


/****************************************************************************************
 * 
 * PUBLIC FUNCTION: setremoteuuaddress
 * 
 ***************************************************************************************/
int CCMobster::setremoteuuaddress( struct TUUAddress aArgs, int *rv )
{
	struct rpc_err rerr;

	// check the rv pointer
	if( rv == NULL ) {
		return ERR_INVALID_RV_POINTER;
	}

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	*rv = *setremoteuuaddress_1( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}

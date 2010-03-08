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
#include "CCUuinterface.h"


/****************************************************************************************
 * 
 * Implementation
 * 
 ***************************************************************************************/
CCUuinterface::CCUuinterface()
{
	cl = NULL;
	iLastRPCError.re_status = RPC_SUCCESS;
}

CCUuinterface::~CCUuinterface()
{
	assert( cl == NULL );
}

char *CCUuinterface::GetLastRPCError( int *aIntErr )
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

int CCUuinterface::Connect( string aRemoteHost )
{
	// check that we are not already connected
	if( cl != NULL ) {
		return ERR_STUB_ALREADY_CONNECTED;
	}

	// start the rpc library
	rpc_nt_init();

	// connect to the service
	cl = clnt_create( aRemoteHost.c_str(), UUINTERFACE, UUINTERFACEVER, "tcp" );
	if( cl == NULL ) {
		rpc_nt_exit();
		return ERR_FAILED_TO_CONNECT;
	}

	// done
	return ERR_NONE;
}

int CCUuinterface::Disconnect( )
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
int CCUuinterface::ss_startuprpcservice( TStartupInfo aArgs, int *rv )
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
	*rv = *ss_startuprpcservice_2( &aArgs, cl );

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
int CCUuinterface::sc_shutdownrpcservice( int aArgs, int *rv )
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
	*rv = *sc_shutdownrpcservice_2( &aArgs, cl );

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
int CCUuinterface::list_devices( TComponentList *rv )
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
	*rv = *list_devices_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_setdelay
 * 
 ***************************************************************************************/
int CCUuinterface::st_setdelay( TConfigValue aArgs, TResult *rv )
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
	*rv = *st_setdelay_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_setdroppercentage
 * 
 ***************************************************************************************/
int CCUuinterface::st_setdroppercentage( TConfigValue aArgs, TResult *rv )
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
	*rv = *st_setdroppercentage_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_setduplicatepercentage
 * 
 ***************************************************************************************/
int CCUuinterface::st_setduplicatepercentage( TConfigValue aArgs, TResult *rv )
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
	*rv = *st_setduplicatepercentage_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_setbandwidth
 * 
 ***************************************************************************************/
int CCUuinterface::st_setbandwidth( TConfigValue aArgs, TResult *rv )
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
	*rv = *st_setbandwidth_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_setdrd
 * 
 ***************************************************************************************/
int CCUuinterface::st_setdrd( TConfigValue aArgs, TResult *rv )
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
	*rv = *st_setdrd_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_clearsettings
 * 
 ***************************************************************************************/
int CCUuinterface::st_clearsettings( TConfigValue aArgs, TResult *rv )
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
	*rv = *st_clearsettings_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_stopservice
 * 
 ***************************************************************************************/
int CCUuinterface::st_stopservice( TResult *rv )
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
	*rv = *st_stopservice_2( &aArgs, cl );

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
 * PUBLIC FUNCTION: st_reset
 * 
 ***************************************************************************************/
int CCUuinterface::st_reset( TResult *rv )
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
	*rv = *st_reset_2( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}

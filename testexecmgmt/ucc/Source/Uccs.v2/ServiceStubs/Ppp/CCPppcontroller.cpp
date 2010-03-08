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
#include "CCPppcontroller.h"


/****************************************************************************************
 * 
 * Implementation
 * 
 ***************************************************************************************/
CCPppcontroller::CCPppcontroller()
{
	cl = NULL;
	iLastRPCError.re_status = RPC_SUCCESS;
}

CCPppcontroller::~CCPppcontroller()
{
	assert( cl == NULL );
}

char *CCPppcontroller::GetLastRPCError( int *aIntErr )
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

int CCPppcontroller::Connect( string aRemoteHost )
{
	// check that we are not already connected
	if( cl != NULL ) {
		return ERR_STUB_ALREADY_CONNECTED;
	}

	// start the rpc library
	rpc_nt_init();

	// connect to the service
	cl = clnt_create( aRemoteHost.c_str(), PPPCONTROLLER, PPPCONTROLLER_VERSION, "tcp" );
	if( cl == NULL ) {
		rpc_nt_exit();
		return ERR_FAILED_TO_CONNECT;
	}

	// done
	return ERR_NONE;
}

int CCPppcontroller::Disconnect( )
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
int CCPppcontroller::ss_startuprpcservice( TStartupInfo aArgs, int *rv )
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
	*rv = *ss_startuprpcservice_4( &aArgs, cl );

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
int CCPppcontroller::sc_shutdownrpcservice( int aArgs, int *rv )
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
	*rv = *sc_shutdownrpcservice_4( &aArgs, cl );

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
int CCPppcontroller::list_devices( TComponentList *rv )
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
	*rv = *list_devices_4( &aArgs, cl );

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
 * PUBLIC FUNCTION: cstr_startpppsession
 * 
 ***************************************************************************************/
int CCPppcontroller::cstr_startpppsession( TPppSessionConfig aArgs, TResult *rv )
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
	*rv = *cstr_startpppsession_4( &aArgs, cl );

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
 * PUBLIC FUNCTION: dstr_removepppsession
 * 
 ***************************************************************************************/
int CCPppcontroller::dstr_removepppsession( int aArgs, TResult *rv )
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
	*rv = *dstr_removepppsession_4( &aArgs, cl );

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
 * PUBLIC FUNCTION: killsession
 * 
 ***************************************************************************************/
int CCPppcontroller::killsession( int aArgs, TResult *rv )
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
	*rv = *killsession_4( &aArgs, cl );

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
 * PUBLIC FUNCTION: stopsession
 * 
 ***************************************************************************************/
int CCPppcontroller::stopsession( int aArgs, TResult *rv )
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
	*rv = *stopsession_4( &aArgs, cl );

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
 * PUBLIC FUNCTION: getsessioninfo
 * 
 ***************************************************************************************/
int CCPppcontroller::getsessioninfo( int aArgs, TPppSessionDesc *rv )
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
	*rv = *getsessioninfo_4( &aArgs, cl );

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
 * PUBLIC FUNCTION: getppplog
 * 
 ***************************************************************************************/
int CCPppcontroller::getppplog( int aArgs, TVarData *rv )
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
	*rv = *getppplog_4( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}

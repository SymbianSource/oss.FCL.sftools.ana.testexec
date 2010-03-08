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
#include "CCHacontroller.h"


/****************************************************************************************
 * 
 * Implementation
 * 
 ***************************************************************************************/
CCHacontroller::CCHacontroller()
{
	cl = NULL;
	iLastRPCError.re_status = RPC_SUCCESS;
}

CCHacontroller::~CCHacontroller()
{
	assert( cl == NULL );
}

char *CCHacontroller::GetLastRPCError( int *aIntErr )
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

int CCHacontroller::Connect( string aRemoteHost )
{
	// check that we are not already connected
	if( cl != NULL ) {
		return ERR_STUB_ALREADY_CONNECTED;
	}

	// start the rpc library
	rpc_nt_init();

	// connect to the service
	cl = clnt_create( aRemoteHost.c_str(), HACONTROLLER, HACONTROLLER_VERSION, "tcp" );
	if( cl == NULL ) {
		rpc_nt_exit();
		return ERR_FAILED_TO_CONNECT;
	}

	// done
	return ERR_NONE;
}

int CCHacontroller::Disconnect( )
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
int CCHacontroller::ss_startuprpcservice( TStartupInfo aArgs, int *rv )
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
	*rv = *ss_startuprpcservice_8( &aArgs, cl );

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
int CCHacontroller::sc_shutdownrpcservice( int aArgs, int *rv )
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
	*rv = *sc_shutdownrpcservice_8( &aArgs, cl );

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
int CCHacontroller::list_devices( TComponentList *rv )
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
	*rv = *list_devices_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: cstr_createagent
 * 
 ***************************************************************************************/
int CCHacontroller::cstr_createagent( TResult *rv )
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
	*rv = *cstr_createagent_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: dstr_removeagent
 * 
 ***************************************************************************************/
int CCHacontroller::dstr_removeagent( int aArgs, TResult *rv )
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
	*rv = *dstr_removeagent_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: startmobileagent
 * 
 ***************************************************************************************/
int CCHacontroller::startmobileagent( int aArgs, TResult *rv )
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
	*rv = *startmobileagent_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: stopmobileagent
 * 
 ***************************************************************************************/
int CCHacontroller::stopmobileagent( int aArgs, TResult *rv )
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
	*rv = *stopmobileagent_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: getmobileagentstatus
 * 
 ***************************************************************************************/
int CCHacontroller::getmobileagentstatus( int aArgs, TResult *rv )
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
	*rv = *getmobileagentstatus_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: setsingleoption
 * 
 ***************************************************************************************/
int CCHacontroller::setsingleoption( TOptionDesc aArgs, TResult *rv )
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
	*rv = *setsingleoption_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: removesingleoption
 * 
 ***************************************************************************************/
int CCHacontroller::removesingleoption( TOptionDesc aArgs, TResult *rv )
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
	*rv = *removesingleoption_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: addlistoption
 * 
 ***************************************************************************************/
int CCHacontroller::addlistoption( TOptionDesc aArgs, TResult *rv )
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
	*rv = *addlistoption_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: removelistoption
 * 
 ***************************************************************************************/
int CCHacontroller::removelistoption( TOptionDesc aArgs, TResult *rv )
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
	*rv = *removelistoption_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: getstatus
 * 
 ***************************************************************************************/
int CCHacontroller::getstatus( int aArgs, THaStatus *rv )
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
	*rv = *getstatus_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: destroytunnelid
 * 
 ***************************************************************************************/
int CCHacontroller::destroytunnelid( THaTunnelID aArgs, TResult *rv )
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
	*rv = *destroytunnelid_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: listtunnels
 * 
 ***************************************************************************************/
int CCHacontroller::listtunnels( int aArgs, THaTunnelList *rv )
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
	*rv = *listtunnels_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: gettunnelinfo
 * 
 ***************************************************************************************/
int CCHacontroller::gettunnelinfo( TGetTunnelRequest aArgs, THaTunnelInfo *rv )
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
	*rv = *gettunnelinfo_8( &aArgs, cl );

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
 * PUBLIC FUNCTION: settimeout
 * 
 ***************************************************************************************/
int CCHacontroller::settimeout( TTimeoutRequest aArgs )
{
	struct rpc_err rerr;

	// check that we have a connection
	if( cl == NULL ) {
		return ERR_STUB_NOT_CONNECTED;
	}

	// do the call
	settimeout_8( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}

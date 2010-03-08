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
#include "CCGenericstub.h"


/****************************************************************************************
 * 
 * Implementation
 * 
 ***************************************************************************************/
CCGenericstub::CCGenericstub( const int anIID, const int aVersion )
	: iIID(anIID), iVersion(aVersion)
{
	cl = NULL;
	iLastRPCError.re_status = RPC_SUCCESS;
}

CCGenericstub::~CCGenericstub()
{
	assert( cl == NULL );
}

char *CCGenericstub::GetLastRPCError( int *aIntErr )
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

int CCGenericstub::Connect( string aRemoteHost )
{
	int res = ERR_NONE;
	// check that we are not already connected
	if( cl != NULL )
	{
		res = ERR_STUB_ALREADY_CONNECTED;
	}

	if( res == ERR_NONE )
	{
		// start the rpc library
		res = rpc_nt_init();
		if( res == ERR_NONE )
		{
			// connect to the service
			cl = clnt_create( aRemoteHost.c_str(), iIID, iVersion, "tcp" );
			if( cl == NULL )
			{
				rpc_nt_exit();
				res = ERR_FAILED_TO_CONNECT;
			}
		}
		else
		{
			res = ERR_RPC_ERROR;
		}
	}

	// done
	return res;
}

int CCGenericstub::Disconnect( )
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
int CCGenericstub::ss_startuprpcservice( TStartupInfo aArgs, int *rv )
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
	*rv = *ss_startuprpcservice_11( &aArgs, cl );

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
int CCGenericstub::sc_shutdownrpcservice( int aArgs, int *rv )
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
	*rv = *sc_shutdownrpcservice_11( &aArgs, cl );

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
 * PUBLIC FUNCTION: list_connections
 * 
 ***************************************************************************************/
int CCGenericstub::list_connections( TComponentList *rv )
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
	*rv = *list_connections_11( &aArgs, cl );

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
 * PUBLIC FUNCTION: cstr_startprocess
 * 
 ***************************************************************************************/
int CCGenericstub::cstr_startprocess( char *aArgs, int *rv )
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
	*rv = *cstr_startprocess_11( &aArgs, cl );

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
 * PUBLIC FUNCTION: dstr_removeprocess
 * 
 ***************************************************************************************/
int CCGenericstub::dstr_removeprocess( int aArgs, int *rv )
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
	*rv = *dstr_removeprocess_11( &aArgs, cl );

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
 * PUBLIC FUNCTION: run_command
 * 
 ***************************************************************************************/
int CCGenericstub::run_command( TCall aArgs, int *rv )
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
	*rv = *run_command_11( &aArgs, cl );

	// check for rpc errors and return the result
	clnt_geterr( cl, &rerr );
	if( rerr.re_status != RPC_SUCCESS ) {
		iLastRPCError = rerr;
		return ERR_RPC_ERROR;
	}
	return ERR_NONE;
}

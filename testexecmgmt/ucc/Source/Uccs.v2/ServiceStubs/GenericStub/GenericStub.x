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


#include "penstd.x"

/*******************************************************************************
 *
 * Constants
 *
 ******************************************************************************/
const MAXPARAMNAMELENGTH			= 1024;
const MAXPARAMVALUELENGTH			= 1024;

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/

struct TStartupInfo {
	int iEmpty;
};

struct TParam {
	char	iName[MAXPARAMNAMELENGTH];
	char	iValue[MAXPARAMVALUELENGTH];
};

struct TCall {
	int	iCallID;
	TParam	iParams<>;
};

/*******************************************************************************
 *
 * Interface
 *
 ******************************************************************************/
program GENERICSTUB {
	version GENERICSTUB_VERSION
	{
		/* Initialise the service */
		int SS_STARTUPRPCSERVICE( TStartupInfo )		= 1;
		int SC_SHUTDOWNRPCSERVICE( int )			= 2;
		TComponentList LIST_CONNECTIONS( void )			= 30;

		/* Create instances */
		int CSTR_STARTPROCESS( string )				= 31;
		int DSTR_REMOVEPROCESS( int )				= 32;
		
		/* RunCommand Call */
		int RUN_COMMAND( TCall )				= 10;

	} = 1; 
} = 0x34630999;

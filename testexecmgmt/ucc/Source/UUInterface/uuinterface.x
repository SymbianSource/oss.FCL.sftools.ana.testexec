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
* Standard Interface Values
*
*/


#include "../include/penstd.x"

/*******************************************************************************
 *
 * Constants
 *
 ******************************************************************************/
const X_MAX_ADDRESS_LENGTH	= 64;

const X_TCP = 6;
const X_UDP = 17;

/*******************************************************************************
 *
 * Error codes
 *
 ******************************************************************************/
const ERR_FAILEDTOEXECUTECOMMAND		= -100;
const ERR_PROCESSEXECUTEFAILED			= -101;

const ERR_PORTNOTSPECIFIED				= -103;

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
struct TStartupInfo {
	int iEmpty;
};

struct TConfigValue
{
	char iSourceAddress[X_MAX_ADDRESS_LENGTH];
	char iDestinationAddress[X_MAX_ADDRESS_LENGTH];
	int iProtocol;
	int iSrcPort;
	int iDstPort;
	int iValueIntegerPart;
	int iValueFractionPart;
	int iSigmaIntegerPart;
	int iSigmaFractionPart;
	int iCorrelationIntegerPart;
	int iCorrelationFractionPart;
	int iMaximumValueIntegerPart;
	int iMaximumValueFractionPart;
	int iCongestionIntegerPart;
	int iCongestionFractionPart;
};

struct TResult {
	int iStandardResult;
	int iExtendedCode;
	int iSystemError;
};

/*******************************************************************************
 *
 * Interface
 *
 ******************************************************************************/
program UUINTERFACE {
	version UUINTERFACEVER {

		/* Service Management */
		int SS_STARTUPRPCSERVICE( TStartupInfo )		= 1;
		int SC_SHUTDOWNRPCSERVICE( int )				= 2;
		TComponentList LIST_DEVICES( void )				= 30;

		/* Custom Interface */
		TResult ST_SETDELAY( TConfigValue ) = 3;
		TResult ST_SETDROPPERCENTAGE( TConfigValue ) = 4;
		TResult ST_SETDUPLICATEPERCENTAGE( TConfigValue ) = 5;
		TResult ST_SETBANDWIDTH( TConfigValue ) = 6;
		TResult ST_SETDRD ( TConfigValue ) = 7;
		TResult ST_CLEARSETTINGS( TConfigValue ) = 8;
		TResult ST_STOPSERVICE( void ) = 9;
		TResult ST_RESET( void ) = 10;
	} = 2;
} =0x34630202;

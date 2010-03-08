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


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "MTInterfaces.h"
#include "CDatalinkNull.h"


/*******************************************************************************
 *
 * Constructor - init state vars
 *
 ******************************************************************************/
CDatalinkNull::CDatalinkNull( TPhoneData *aPhoneData, CLog *aLog )
{
	// check params
	assert( aPhoneData != NULL );
	assert( aLog != NULL );

	// initialise members
	iAirInterface = NULL;
	iTEChannel = NULL;
	iPhoneData = aPhoneData;
	iLog = aLog;
}


CDatalinkNull::~CDatalinkNull()
{
//	assert( iAirInterface == NULL );
//	assert( iTEChannel == NULL );
}


/*******************************************************************************
 *
 * Access methods
 *
 ******************************************************************************/
void CDatalinkNull::SetAirInterface( IAirInterface *aAirInterface )
{
	iAirInterface = aAirInterface;
}


void CDatalinkNull::SetTEChannel( ITEChannel *aTEChannel )
{
	iTEChannel = aTEChannel;
}


/*******************************************************************************
 *
 * Process data from the TEChannel
 *
 ******************************************************************************/
TDataPathError CDatalinkNull::ProcessTEData( char *data, int len, int *aErrCode )
{
	TDataPathError err;

	// if no air-interface exists then return error
	if( iAirInterface == NULL ) {
		return DPE_NO_AIR_INTERFACE;
	}

	// otherwise forward the data
	err = iAirInterface->SendPacket( data, len, aErrCode );
	return err;
}


TDataPathError CDatalinkNull::ProcessUUData( char *data, int len, int *aErrCode )
{
	TDataPathError err; 

	// if no te-channel exists then return an error
	if( iTEChannel == NULL ) {
		return DPE_NO_TE_CHANNEL;
	}

	// otherwise forward the data
	err = iTEChannel->SendPacket( data, len, aErrCode );
	return err;
}

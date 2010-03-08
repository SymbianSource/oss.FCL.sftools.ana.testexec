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

/****************************************************************************************
 *
 * Local Includes
 *
 ***************************************************************************************/
#include "CSynchronisation.h"
#include "UCCS_ErrorCodes.h"

/****************************************************************************************
 *
 * Definitions
 *
 ***************************************************************************************/


/****************************************************************************************
 *
 * Construction
 *
 ***************************************************************************************/
CSynchronisation::CSynchronisation( IOutput *aOutput )
{
	assert( aOutput != NULL );
	iOutput = aOutput;
	iObjectCleared = 0;
	hDeviceSideRendezvousSemaphore = CreateSemaphore( NULL, 0, 2, NULL );
	assert( hDeviceSideRendezvousSemaphore != NULL );
	hPCSideRendezvousSemaphore = CreateSemaphore( NULL, 0, 2, NULL );
	assert( hPCSideRendezvousSemaphore != NULL );
	hDeviceSignalSemaphore = CreateSemaphore( NULL, 0, 0x000000FF, NULL );
	assert( hDeviceSignalSemaphore != NULL );
	hScriptSignalSemaphore = CreateSemaphore( NULL, 0, 0x000000FF, NULL );
	assert( hScriptSignalSemaphore != NULL );
}

CSynchronisation::~CSynchronisation()
{
	CloseHandle( hDeviceSideRendezvousSemaphore );
	CloseHandle( hPCSideRendezvousSemaphore );
	CloseHandle( hDeviceSignalSemaphore );
	CloseHandle( hScriptSignalSemaphore );
}


/****************************************************************************************
 *
 * SignalFromDevice
 *
 ***************************************************************************************/
int CSynchronisation::SignalFromDevice()
{
	int err;

	// do stuff
	err = ReleaseSemaphore( hDeviceSignalSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the device signal semaphore." );
		return UCCS_SYSTEMERROR;
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * WaitFromScript
 *
 ***************************************************************************************/
int CSynchronisation::WaitFromScript()
{
	int err;
	err = WaitForSingleObject( hDeviceSignalSemaphore, INFINITE );
	if( err != WAIT_OBJECT_0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Error while waiting for the device signal semaphore." );
		return UCCS_SYSTEMERROR;
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * SignalFromScript
 *
 ***************************************************************************************/
int CSynchronisation::SignalFromScript()
{
	int err;

	// do stuff
	err = ReleaseSemaphore( hScriptSignalSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the script signal semaphore." );
		return UCCS_SYSTEMERROR;
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * WaitFromDevice
 *
 ***************************************************************************************/
int CSynchronisation::WaitFromDevice()
{
	int err;
	err = WaitForSingleObject( hScriptSignalSemaphore, INFINITE );
	if( err != WAIT_OBJECT_0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Error while waiting for the script signal semaphore." );
		return UCCS_SYSTEMERROR;
	}
	return UCCS_OK;
}


/****************************************************************************************
 *
 * RendezvousFromDevice
 *
 ***************************************************************************************/
int CSynchronisation::RendezvousFromDevice()
{
	int err;

	// signal that the device side has arrived
	err = ReleaseSemaphore( hDeviceSideRendezvousSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the device-side rendezvous semaphore." );
		return UCCS_SYSTEMERROR;
	}

	// wait for the pc-side to arrive
	err = WaitForSingleObject( hPCSideRendezvousSemaphore, INFINITE );
	if( err != WAIT_OBJECT_0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Error while waiting for pc-side rendezvous semaphore." );
		return UCCS_SYSTEMERROR;
	}

	// otherwise everything is good
	return UCCS_OK;
}


/****************************************************************************************
 *
 * RendezvousFromScript
 *
 ***************************************************************************************/
int CSynchronisation::RendezvousFromScript()
{
	int err;

	// signal that the pc side has arrived
	err = ReleaseSemaphore( hPCSideRendezvousSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the pc-side rendezvous semaphore." );
		return UCCS_SYSTEMERROR;
	}

	// wait for the device-side to arrive
	err = WaitForSingleObject( hDeviceSideRendezvousSemaphore, INFINITE );
	if( err != WAIT_OBJECT_0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Error while waiting for device-side rendezvous semaphore." );
		return UCCS_SYSTEMERROR;
	}

	// otherwise everything is good
	return UCCS_OK;
}


/****************************************************************************************
 *
 * ClearSynchronisation
 *
 ***************************************************************************************/
void CSynchronisation::ClearSynchronisation()
{
	int err;

	// signal all semaphores so that anyone waiting will be blocked
	err = ReleaseSemaphore( hDeviceSideRendezvousSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the device-side rendezvous semaphore." );
	}
	err = ReleaseSemaphore( hPCSideRendezvousSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the script-side rendezvous semaphore." );
	}
	err = ReleaseSemaphore( hDeviceSignalSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the device signal semaphore." );
	}
	err = ReleaseSemaphore( hScriptSignalSemaphore, 1, NULL );
	if( err == 0 ) {
		iOutput->Error( UCCS_SYSTEMERROR, "Failed to signal the script signal semaphore." );
	}
}

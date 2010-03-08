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
* CSerialPort -- encapsulates the details of using the serial port as 
* a communications channel
* System Includes
*
*/



#include <stdio.h>
#include <assert.h>

/*********************************************************************
 *
 * Local Includes
 *
 ********************************************************************/
#include "CSerialPort.h"

/*********************************************************************
 *
 * Construction
 *
 ********************************************************************/
CSerialPort::CSerialPort()
{
	iComPort = INVALID_HANDLE_VALUE;
}


CSerialPort::~CSerialPort()
{
	assert( iComPort == INVALID_HANDLE_VALUE );
}


/*********************************************************************
 *
 * OpenComPort() -- open a com port, set the timeouts, set the config
 *
 ********************************************************************/
int CSerialPort::OpenPort( char *aComPort )
{
	int err;

	// Open the COM Port
	iComPort = CreateFile( aComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL );
	if( iComPort == INVALID_HANDLE_VALUE ) {
		err = GetLastError();
		return err;
	}

	// Set the timeouts
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;
	err = SetCommTimeouts( iComPort, &CommTimeOuts );
	if( err == 0 ) {
		err = GetLastError();
		CloseHandle( iComPort );
		iComPort = INVALID_HANDLE_VALUE;
		return err;
	}

	// Configure the COM port 
	DCB dcb;
	GetCommState(iComPort, &dcb);
	dcb.DCBlength			= sizeof(dcb); 
	dcb.BaudRate			= CBR_115200;
	dcb.fBinary				= TRUE;
	dcb.fParity				= NOPARITY;
	dcb.fOutxCtsFlow		= TRUE;
	dcb.fOutxDsrFlow		= FALSE;
	dcb.fDtrControl			= DTR_CONTROL_ENABLE;	
	dcb.fDsrSensitivity		= FALSE;
	dcb.fTXContinueOnXoff	= TRUE;
	dcb.fOutX				= FALSE;
	dcb.fInX				= FALSE;
	dcb.fErrorChar			= FALSE;
	dcb.fNull				= FALSE;
	dcb.fRtsControl			= RTS_CONTROL_HANDSHAKE;
	dcb.fAbortOnError		= TRUE;
	dcb.XonLim				= 100;
	dcb.XoffLim				= 100;
	dcb.ByteSize			= 8;
	dcb.Parity				= NOPARITY;
	dcb.StopBits			= ONESTOPBIT;
	dcb.XonChar				= 17;
	dcb.XoffChar			= 19;
	err = SetCommState( iComPort, &dcb );	
	if( err == 0 ) {
		err = GetLastError();
		CloseHandle( iComPort );
		iComPort = INVALID_HANDLE_VALUE;
		return err;
	}

	// Success
	return 0;
}


/*********************************************************************
 *
 * CloseComPort() -- close an open com port.
 *
 ********************************************************************/
void CSerialPort::ClosePort()
{
	// if the port isn't open then just return
	if( iComPort == INVALID_HANDLE_VALUE ) 
		return;
	
	// close the port
	CloseHandle( iComPort );
	iComPort = INVALID_HANDLE_VALUE;
}


/*********************************************************************
 *
 *  ReceiveBytes() -- read specified number of bytes from the comport
 *
 ********************************************************************/
int CSerialPort::ReceiveBytes( char* aBuff, int *aSize)
{
	int err;
	int original_size;
	unsigned long iBytesRead;

	// check the state and the params
	assert( iComPort != INVALID_HANDLE_VALUE );
	assert( aBuff != NULL );
	assert( aSize != NULL );

	// receive bytes
	original_size = *aSize;
	err = ReadFile( iComPort, aBuff, *aSize, &iBytesRead, NULL );

	// set aSize to the number actually read
	*aSize = iBytesRead;

	// check for errors
	if( err == 0 )  {
		err = GetLastError();
		return err;
	}
		
	// this condition should have generated an error -- lets make sure
	assert( *aSize == original_size );
	return 0;
}


/*********************************************************************
 *
 *  SendBytes() -- write the specified number of bytes to the COMport
 *
 ********************************************************************/
int CSerialPort::SendBytes( char *aBuff, int *aSize )
{
	int ret;
	DWORD dwModemStatus, dwBytes = 0;

	// check state and params
	assert( iComPort != INVALID_HANDLE_VALUE );
	assert( aBuff != NULL );
	assert( aSize != NULL );

	// write bytes to the port
	ret = GetCommModemStatus( iComPort, &dwModemStatus );
	ret = WriteFile( iComPort, (LPVOID)aBuff, *aSize, &dwBytes, NULL );

	// set aSize to the number actually written
	*aSize = dwBytes;

	// check for errors
	if( ret == 0 ) {
		ret = GetLastError();
		return ret;
	}
	
	// done	
	return 0;	
}

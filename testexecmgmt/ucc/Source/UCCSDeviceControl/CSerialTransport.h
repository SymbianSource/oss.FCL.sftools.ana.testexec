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
* Serial Transport
*
*/



#ifndef __SERIAL_H__
#define __SERIAL_H__

/********************************************************************************
 *
 * System Includes
 *
 *******************************************************************************/
#include <c32comm.h>
#include <f32file.h>

#include "MUCCTransport.h"

/********************************************************************************
 *
 * Definitions
 *
 *******************************************************************************/
#define KMaxPacketSize								64
#define KModuleSize									8
#define KReadTimeout								30000000

const char  OPT_DELIMITER	= '|';

#ifndef _TCOMMSTATUS
#define _TCOMMSTATUS
enum TCommStatus { 
	EIdle, 
	EInitialising, 
	EInitialised, 
	EConnecting, 
	EConnected,
	EDisconnecting, 
	EDisconnected, 
	EReleasing, 
	ESendPending,
	EReceivePending,
	ELast
};
#endif

/********************************************************************************
 *
 * Class
 *
 *******************************************************************************/
class CSerialTransport : public MUCCTransport
{
public:
	CSerialTransport();
	static CSerialTransport* NewL( TPtrC16 aModule );
	virtual ~CSerialTransport();

	TInt InitialiseL();
	TInt ConnectL( TDesC *aRemoteHost );
	TInt RequestSend( TDesC8 *aCommandData, const TUint aDataLength );
	TInt RequestReceive( TPtr8 *aRecvBufferPtr, TUint aByteCount );
	TInt Disconnect( void );
	TInt Release( void );
	TText8 *Error( void );

private:
	void ConstructL( TPtrC16 aModule );
	void SetStatus( TCommStatus aNewStatus );
	void ExtractOptions( TDesC *aRemoteHost, TInt& aPortNumber, TInt& aBaudCap, TBps& aBaudRate );

private:
	enum TReadWriteStatus { ENoRW, EWritePending, EReadPending };

private:
	TCommStatus iSerialStatus;
	TBuf16<KModuleSize> iModule;
	TInt iRetries;

	RCommServ iCommServer;
	RComm iCommPort;
	TCommConfig iPortSettings, iOldPortSettings;

	TInt iCommPortOpen, iCommOldSettingsValid;
	TRequestStatus iStatus;	 
};

#endif

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



#ifndef __CTCPTRANSPORT_H__
#define __CTCPTRANSPORT_H__

#include <es_sock.h>
#include <in_sock.h>

#include "MUCCTransport.h"

const TUint KStatHeaderSize			= 8;
const TUint KLittleStatPort			= 3000;
const TUint KLittleStatListenQueue	= 1;
const TUint KMaxTCPIPPacketSize		= 1024*32;
const TUint	KUCCTCPPort				= 3001;

class CTCPTransport : public MUCCTransport
	{
public:
	// construction
	CTCPTransport();
	virtual ~CTCPTransport();

	// from MStatNetwork
	TInt InitialiseL();
	TInt ConnectL( TDesC *aRemoteHost );
	TInt RequestSend( TDesC8 *aCommandData, const TUint aDataLength );
	TInt RequestReceive( TPtr8 *aRecvBufferPtr, TUint aByteCount );
	TInt Disconnect( void );
	TInt Release( void );
	TText8 *Error( void );

private:
	RSocket		iSocket;
	RConnection iConnection;
	RSocketServ iServer;
	TInetAddr	iDestAddr;
	};

#endif // __CTCPTRANSPORT_H__

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



/**
 @file MUCCTransport.h
*/

#ifndef __MUCCTRANSPORT_H__
#define __MUCCTRANSPORT_H__

class MUCCTransport
{
public:
	virtual ~MUCCTransport() {}
	virtual TInt InitialiseL() = 0;
	virtual TInt ConnectL( TDesC *aRemoteHost ) = 0;
	virtual TInt RequestSend( TDesC8 *aData, const TUint aLength ) = 0;
	virtual TInt RequestReceive( TPtr8 *aRecvBufferPtr, TUint aByteCount ) = 0;
	virtual TInt Disconnect( void ) = 0;
	virtual TInt Release( void ) = 0;
	virtual TText8 *Error( void ) = 0;
};

#endif // __MUCCTRANSPORT_H__

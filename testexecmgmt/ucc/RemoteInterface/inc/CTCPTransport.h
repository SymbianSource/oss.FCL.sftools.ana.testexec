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

#include <winsock2.h>
#include "MUCCTransport.h"
 
class CTCPTransport : public MUCCTransport
	{
public:
	// construction
	CTCPTransport();
	virtual ~CTCPTransport();
	virtual int Connect( const string &aDestination );
	virtual void Disconnect();
	virtual int RequestSend( const char* aData, const int aLength );
	virtual int RequestReceive( char *aBuff, const int aLength );

private:
	SOCKET		iSocket;
	};


#endif // __CTCPTRANSPORT_H__

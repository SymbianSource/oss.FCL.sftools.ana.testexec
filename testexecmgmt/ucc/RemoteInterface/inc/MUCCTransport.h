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



#ifndef __MUCCTRANSPORT_H__
#define __MUCCTRANSPORT_H__

#include <string>

using std::string;

class MUCCTransport
	{
public:
	virtual ~MUCCTransport() {}
	virtual int Connect( const string &aDestination ) = 0;
	virtual void Disconnect() = 0;
	virtual int RequestSend( const char* aData, const int aLength ) = 0;
	virtual int RequestReceive( char *aBuff, const int aLength ) = 0;
	};

#endif // __MUCCTRANSPORT_H__

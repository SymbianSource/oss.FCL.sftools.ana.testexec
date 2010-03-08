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



#ifndef __CLISTENINGSERVER_H__
#define __CLISTENINGSERVER_H__

#include <windows.h>

class CListeningServer
	{
public:
	CListeningServer();
	~CListeningServer();

	int		Open( const int aPort );
	void	Close();
	int		Send( const int aValue );
	int		Recieve( int& aValue );

private:
	SOCKET			iLocalSock;
	SOCKADDR_IN		iLocalAddr;
	SOCKET			iRemoteSock;
	SOCKADDR_IN		iRemoteAddr;
	int				iPort;
	};

#endif // __CLISTENINGSERVER_H__

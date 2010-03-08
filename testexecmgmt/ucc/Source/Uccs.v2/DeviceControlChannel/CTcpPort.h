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
* CTcpPort
*
*/



#ifndef __CTCPPORT_H__
#define __CTCPPORT_H__

#include <windows.h>
#include "../Core/UCCS_Interfaces.h"

class CTcpPort : public IPort
{
public:
	CTcpPort();
	~CTcpPort();

	int OpenPort( char *aPort );
	void ClosePort();
	int ReceiveBytes( char *aBuff, int *aLength );
	int SendBytes( char *aBuff, int *aLength );

private:
	SOCKET iLocalSock;
	SOCKADDR_IN iLocalAddr;
	SOCKET iRemoteSock;
	SOCKADDR_IN iRemoteAddr;
	int iPort;
};

#endif //__CTCPPORT_H__

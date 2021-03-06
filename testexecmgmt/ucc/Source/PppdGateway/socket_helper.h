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
* Switches
*
*/



#ifndef __SOCKET_HELPER_H__
#define __SOCKET_HELPER_H__

/*******************************************************************************
 *
 * Interface
 *
 ******************************************************************************/
struct sockaddr_in getsockaddr( const char* ip, const char* port);
int is_ip_address( const char* aAddr );
int GetSocketError();

#endif // __SOCKET_HELPER_H__



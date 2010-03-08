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
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "socket_helper.h"

/*******************************************************************************
 *
 * Definition
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * Implementation
 *
 ******************************************************************************/
struct sockaddr_in getsockaddr( const char* ip, const char* port)
{	
	int isip;
	struct sockaddr_in sockaddr;
	int b1, b2, b3, b4;
	unsigned long addr = 0;

	// set the basics
	sockaddr.sin_family = AF_INET;		
	sockaddr.sin_port = htons(atoi(port));

	// if IP is NULL then set to INADDR_ANY otherwise work it out
	if( ip == NULL ) {
#ifdef WIN32
		sockaddr.sin_addr.S_un.S_addr = INADDR_ANY;
#else
		sockaddr.sin_addr.s_addr = INADDR_ANY;
#endif
		return sockaddr;
	} 

	// otherwise we see if this is an IP address
	isip = is_ip_address( ip );
	if( isip ) {
		sscanf( ip, "%d.%d.%d.%d", &b1, &b2, &b3,&b4 );
		addr = ((b1<<24)&0xFF000000);
		addr |= ((b2<<16)&0x00FF0000);
		addr |= ((b3<<8)&0x0000FF00);
		addr |= ((b4)&0x000000FF);
#ifdef WIN32
		sockaddr.sin_addr.S_un.S_addr = htonl(addr);
#else
		sockaddr.sin_addr.s_addr = htonl(addr);
#endif

	} else {
		struct hostent *hp = gethostbyname(ip);
		memcpy(&sockaddr.sin_addr,hp->h_addr,hp->h_length); 	
	}

	return sockaddr;
}

int is_ip_address( const char* aAddr )
{
  int i;
  int dot_count = 0;

  // Our method for checking whether this is an IP address is to check that
  // we have digits and points (3 to be precise).
  for( i = 0; aAddr[i] != 0; i++ ) {
    if( (aAddr[i] != '.') && (isdigit(aAddr[i]) == 0) ) {
      return 0;
    }
    if( aAddr[i] == '.' ) {
      dot_count++;
      if( dot_count > 3 ) {
	return 0;
      }
    }
  }
  return 1;
}

/*******************************************************************************
 *
 * GetSocketError - gets the last socket error - for xdev
 *
 ******************************************************************************/
int GetSocketError()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

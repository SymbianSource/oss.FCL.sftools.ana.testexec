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
* System Includes
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CInterfaceAlias.h"

/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAXALIASNAME                  64
#define MAXALIASCOUNT                 1024
#define MAXINTERFACECOUNT             1024
#define ETHERNET_INTERFACE_PREFIX     "eth"

/*******************************************************************************
 *
 * IMPLEMENTATION: CInterfaceAlias
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * PUBLIC: CInterfaceAlias
 *
 ******************************************************************************/
CInterfaceAlias::CInterfaceAlias()
{
  iState = IAS_INIT;
  iBaseInterfaceIndex = 0;
  iAliasIndex = 0;
}

CInterfaceAlias::~CInterfaceAlias()
{
  assert( iState != IAS_UP );
}


/*******************************************************************************
 *
 * PUBLIC: CreateNewInterfaceAlias
 *
 ******************************************************************************/
TInterfaceAliasError CInterfaceAlias::CreateNewInterfaceAlias( int aBaseInterfaceIndex, int aNetMask, int aHostAddress, int *aAliasIndex, int *aErrorCode )
{
  int alias_index;
  char alias_name_temp[MAXALIASNAME];
  int sockfd;
  int err;
  struct sockaddr_in base_interface_addr;
  struct ifreq ifr;
  struct sockaddr_in *saddr;
  TInterfaceAliasError rv;

  // check the state
  if( iState != IAS_INIT ) {
    return IE_INVALID_STATE;
  }

  // check the params
  assert( aAliasIndex != NULL );
  assert( aErrorCode != NULL );
  *aErrorCode = 0;
  *aAliasIndex = -1;
  if( aBaseInterfaceIndex < 0 ) {
    return IE_INVALID_PARAM;
  }

  // get a free alias index
  rv = GetFreeAliasIndex( aBaseInterfaceIndex, &alias_index, aErrorCode );
  if( rv != IE_NONE ) {
    return rv;
  }

  // get the address of the base interface
  rv = GetBaseInterfaceAddress( aBaseInterfaceIndex, &base_interface_addr, aErrorCode );
  if( rv != IE_NONE ) {
    return rv;
  }

  // now strip off the host part and replace with the passed host address
  base_interface_addr.sin_addr.s_addr = ChangeHostAddress( base_interface_addr.sin_addr.s_addr, aNetMask, aHostAddress ); 

  // setup the interface name
  sprintf( alias_name_temp, "eth%d:%d", aBaseInterfaceIndex, alias_index );
  assert( (strlen(alias_name_temp) + 1) < IFNAMSIZ );

  // create a socket to make ioctl calls on
  sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sockfd <= 0 ) {
    *aErrorCode = errno;
    return IE_SOCKET_FAILED;
  }

  // setup the request
  memcpy( ifr.ifr_name, alias_name_temp, strlen(alias_name_temp) + 1 );
  saddr = (struct sockaddr_in*)(&(ifr.ifr_addr));
  saddr->sin_family = AF_INET;
  saddr->sin_port = 0;
  saddr->sin_addr = base_interface_addr.sin_addr;

  // make the ioctl call
  err = ioctl( sockfd, SIOCSIFADDR, &ifr );
  close( sockfd );
  if( err == -1 ) {
    *aErrorCode = errno;
    return IE_IOCTL_FAILED;
  }

  // update the state vars
  iState = IAS_UP;
  iBaseInterfaceIndex = aBaseInterfaceIndex;
  *aAliasIndex = iAliasIndex = alias_index;
  iInterfaceName = alias_name_temp;
  iInterfaceAddress = inet_ntoa( base_interface_addr.sin_addr );

  // done
  return IE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC:  DestroyInterfaceAlias
 *
 ******************************************************************************/
TInterfaceAliasError CInterfaceAlias::DestroyInterfaceAlias( int *aErrorCode )
{
  int sockfd;
  int err;
  struct ifreq ifr;
  char ifn[64];

  // check state
  if( iState != IAS_UP ) {
    return IE_INVALID_STATE;
  }

  // check params
  assert( aErrorCode != NULL );
  *aErrorCode = 0;

  // create a socket to make ioctl calls on
  sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
  assert( sockfd >= 0 );
  
  // setup the request record
  sprintf( ifn, "eth%d:%d", iBaseInterfaceIndex, iAliasIndex );
  memcpy( ifr.ifr_name, ifn, strlen(ifn) + 1 );
  ifr.ifr_flags = 0;
  
  // get the current flags - this is very important since most flags actually go straight through to
  // base interface - so we need to keep this and just clear the IFF_UP flag which is local to the alias
  err = ioctl( sockfd, SIOCGIFFLAGS, &ifr );
  if( err == -1 ) {
    iState = IAS_DOWN;
    *aErrorCode = errno;
    close( sockfd );
    return IE_IOCTL_FAILED;
  }

  // mask out the IFF_UP flag
  ifr.ifr_flags &= ~IFF_UP; 

  // make the ioctl calls 
  err = ioctl( sockfd, SIOCSIFFLAGS, &ifr );
  close( sockfd );
  if( err == -1 ) {
    iState = IAS_DOWN;
    *aErrorCode = errno;
    return IE_IOCTL_FAILED;
  }
  
  // otherwise done
  iState = IAS_DOWN;
  return IE_NONE;
}



/*******************************************************************************
 *
 * PUBLIC:  GetInterfaceIndex
 *
 ******************************************************************************/
TInterfaceAliasError CInterfaceAlias::GetInterfaceIndex( int *aBaseInterfaceIndex, int *aAliasIndex )
{
  // check params
  assert( aBaseInterfaceIndex != NULL );
  assert( aAliasIndex != NULL );

  // check state
  if( iState == IAS_INIT ) {
    return IE_INVALID_STATE;
  }

  // return info
  *aBaseInterfaceIndex = iBaseInterfaceIndex;
  *aAliasIndex = iAliasIndex;
  return IE_NONE;
}

 
/*******************************************************************************
 *
 * PUBLIC:  GetInterfaceName
 *
 ******************************************************************************/
TInterfaceAliasError CInterfaceAlias::GetInterfaceName( string *aInterfaceName )
{
  assert( aInterfaceName != NULL );
  if( iState == IAS_INIT ) {
    return IE_INVALID_STATE;
  }
  *aInterfaceName = iInterfaceName;
  return IE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC:  GetInterfaceAddress
 *
 ******************************************************************************/
TInterfaceAliasError CInterfaceAlias::GetInterfaceAddress( string *aInterfaceAddress )
{
  assert( aInterfaceAddress != NULL );
  if( iState == IAS_INIT ) {
    return IE_INVALID_STATE;
  }
  *aInterfaceAddress = iInterfaceAddress;
  return IE_NONE;
}


/*******************************************************************************
 *
 * PRIVATE: ParseInterfaceName
 *
 ******************************************************************************/
int CInterfaceAlias::ParseInterfaceName( char *aInterfaceName, int *aBaseIndex, int *aAliasIndex )
{
  int rv;
 
  // check params
  assert( aInterfaceName != NULL );
  assert( aBaseIndex != NULL );
  assert( aAliasIndex != NULL );

  // init params
  *aBaseIndex = *aAliasIndex = -1;

  // do the scanf
  rv = sscanf( aInterfaceName, "eth%d:%d", aBaseIndex, aAliasIndex );

  // return valid if at least a base index was read
  if( rv >= 1 ) {
    return 1;
  }
  return 0;
}


/*******************************************************************************
 *
 * PRIVATE: GetInterfaceList
 *
 ******************************************************************************/
TInterfaceAliasError CInterfaceAlias::GetInterfaceList( char *aInterfaceBuffer, int aInputBufferLength, int *aOutputBufferLength, int *aErrorCode )
{
  int sockfd;
  int err;
  struct ifconf ifc;
    
  // check params
  assert( aInterfaceBuffer != NULL );
  assert( aOutputBufferLength != NULL );
  assert( aErrorCode != NULL );

  // set the params
  *aOutputBufferLength = *aErrorCode = 0;
  
  // create a socket to make ioctl calls on
  sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sockfd <= 0 ) {
    *aErrorCode = errno;
    return IE_SOCKET_FAILED;
  }

  // do the ioctl() call to retrieve all the interfaces
  ifc.ifc_len = aInputBufferLength;
  ifc.ifc_buf = aInterfaceBuffer;
  err = ioctl( sockfd, SIOCGIFCONF, &ifc );
  close( sockfd );
  if( err == -1 ) {
    *aErrorCode = errno;
    return IE_IOCTL_FAILED;
  }

  // call was successful, set the output buffer length
  *aOutputBufferLength = ifc.ifc_len;

  // done
  return IE_NONE;
}


/*******************************************************************************
 *
 * PRIVATE: GetFreeAliasIndex
 *
 ******************************************************************************/ 
TInterfaceAliasError CInterfaceAlias::GetFreeAliasIndex( int aBaseInterfaceIndex, int *aAliasIndex, int *aErrorCode )
{
  TInterfaceAliasError rv;
  struct ifreq ifr[MAXINTERFACECOUNT];  
  char alias_status[MAXALIASCOUNT];
  int is_valid, base_index, alias_index, interface_count, i;
  int base_index_found = 0;
  int current_alias_count = 0;

  // check and set params
  assert( aAliasIndex != NULL );
  assert( aErrorCode != NULL );
  *aAliasIndex = 0;
  *aErrorCode = 0;
  
  // clear the arrays
  memset( alias_status, 0, sizeof(alias_status) );

  // get the interface list
  rv = GetInterfaceList( (char*)ifr, sizeof(ifr), &interface_count, aErrorCode );
  if( rv != IE_NONE ) {
    return rv;
  }
  interface_count = interface_count / sizeof(struct ifreq);

  // for each interface entry...
  for( i = 0; i < interface_count; i++ ) {
    
    // parse the name of the interface
    is_valid = ParseInterfaceName( (ifr[i]).ifr_name, &base_index, &alias_index );

    // process the interface
    if( (is_valid) && (base_index == aBaseInterfaceIndex) ) {
      base_index_found = 1;
      if( alias_index >= 0 ) {
	assert( alias_index < MAXALIASCOUNT );
	alias_status[alias_index] = 1;
	current_alias_count++;
      }
    }
  }
  
  // see if there are any free aliases
  if( current_alias_count == MAXALIASCOUNT ) {
    return IE_NO_FREE_ALIAS;
  }

  // otherwise return the first free alias
  for( i = 0; i < MAXALIASCOUNT; i++ ) {
    if( alias_status[i] == 0 )
      break;
  }
  assert( i < MAXALIASCOUNT );
  
  // done
  *aAliasIndex = i;
  return IE_NONE;
}


/*******************************************************************************
 *
 * PRIVATE: GetBaseInterfaceAddress
 *
 ******************************************************************************/ 
TInterfaceAliasError CInterfaceAlias::GetBaseInterfaceAddress( int aBaseInterfaceIndex, struct sockaddr_in *aInterfaceAddress, int *aErrorCode )
{
  int sockfd, err;
  struct ifreq ifr;

  // check args
  assert( aInterfaceAddress != NULL );
  assert( aErrorCode != NULL );
  *aErrorCode = 0;

  // create the interface name
  memset( &ifr, 0, sizeof(ifr) );
  snprintf( ifr.ifr_name, IFNAMSIZ, "eth%d", aBaseInterfaceIndex );
  
  // create a socket to make the ioctl calls on
  sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sockfd <= 0 ) {
    *aErrorCode = errno;
    return IE_SOCKET_FAILED;
  }

  // make the ioctl call
  err = ioctl( sockfd, SIOCGIFADDR, &ifr );
  close( sockfd );
  if( err == -1 ) {
    *aErrorCode = errno;
    return IE_IOCTL_FAILED;
  }
    
  // save the address
  *aInterfaceAddress = *((struct sockaddr_in*)(&ifr.ifr_addr));
  
  // done
  return IE_NONE;
}


/*******************************************************************************
 *
 * PRIVATE: ChangeHostAddress
 *
 ******************************************************************************/ 
int CInterfaceAlias::ChangeHostAddress( int aBaseAddress, int aNetMask, int aHostAddress )
{
  int hostmask = 0;
  int netmask = 0;
  int rv;

  // create masks for the netpart and the hostpart
  netmask = NetmaskFromBitcount( aNetMask );
  hostmask = ~netmask;

  // now create the address
  rv = ntohl( aBaseAddress );
  rv &= netmask;
  rv |= (aHostAddress & hostmask);
  rv = htonl( rv );
  return rv;
}


/*******************************************************************************
 *
 * PRIVATE: NetmaskFromBitcount
 *
 ******************************************************************************/ 
int CInterfaceAlias::NetmaskFromBitcount( int aBitCount )
{
  int i, netmask = 0;
  for( netmask = 0, i = 0; i < aBitCount; i++ ) {
    netmask |= 1<<(31-i);
  }
  return netmask;
}

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
* CInterfaceAlias
*
*/



#ifndef __CINTERFACEALIAS_H__
#define __CINTERFACEALIAS_H__

/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <string>
using namespace std;

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
typedef enum {
  IAS_INIT,
  IAS_UP,
  IAS_DOWN
} TInterfaceState;

typedef enum {
  IE_NONE,
  IE_NO_FREE_ALIAS,
  IE_INVALID_PARAM,
  IE_SOCKET_FAILED,
  IE_INVALID_ADDR,
  IE_IOCTL_FAILED,
  IE_INVALID_STATE,
  IE_INVALID_INTERFACE
} TInterfaceAliasError;

/*******************************************************************************
 *
 * Class CInterfaceAlias
 *
 ******************************************************************************/
class CInterfaceAlias
{
 public:
  CInterfaceAlias();
  ~CInterfaceAlias();

  TInterfaceAliasError CreateNewInterfaceAlias( int aBaseInterfaceIndex, int aNetMask, int aHostAddress, int *aAliasIndex, int *aErrorCode );
  TInterfaceAliasError DestroyInterfaceAlias( int *aErrorCode );
  TInterfaceAliasError GetInterfaceIndex( int *aBaseInterfaceIndex, int *aAliasIndex );
  TInterfaceAliasError GetInterfaceName( string *aInterfaceName );
  TInterfaceAliasError GetInterfaceAddress( string *aInterfaceAddress );

 private:
  TInterfaceAliasError GetBaseInterfaceAddress( int aBaseInterfaceIndex, struct sockaddr_in *aInterfaceAddress, int *aErrorCode );
  TInterfaceAliasError GetFreeAliasIndex( int aBaseInterfaceIndex, int *aAliasIndex, int *aErrorCode );
  int ParseInterfaceName( char *aInterfaceName, int *aBaseIndex, int *aAliasIndex );
  TInterfaceAliasError GetInterfaceList( char *aInterfaceBuffer, int aInputBufferLength, int *aOutputBufferLength, int *aErrorCode );
  int ChangeHostAddress( int aBaseAddress, int aNetMask, int aHostAddress );
  int NetmaskFromBitcount( int aBitCount );

  TInterfaceState iState;
  int iBaseInterfaceIndex;
  int iAliasIndex;
  string iInterfaceAddress;
  string iInterfaceName;
};

#endif //__CINTERFACEALIAS_H__

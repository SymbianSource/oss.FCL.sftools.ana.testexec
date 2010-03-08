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
* This file was autogenerated by rpcgen, but should be modified by the developer.
* Make sure you don't use the -component_mod flag in future or this file will be overwritten.
* Fri Oct 10 17:55:34 2003
*
*/




#ifndef __CSHACONTROLLER_H__
#define __CSHACONTROLLER_H__


/****************************************************************************************
 * 
 * Local Includes
 * 
 ***************************************************************************************/
#include "../include/standard_unix.h"
#include "../DynamicsConfigurationLibrary/CDynamicsConfigFile.h"
#include "../ProcessLibrary/proclib.h"
#include "../AliasLibrary/CInterfaceAlias.h"
#include "../IntegerAllocatorLibrary/CIntegerAllocator.h"
#include "../DynamicsCommandWrapper/CDynamicsCommand.h"
#include "hacontroller.h"


/****************************************************************************************
 * 
 * Definition: CSHacontroller
 * 
 ***************************************************************************************/
class CSHacontroller
{
public:
	// Standard Methods
	CSHacontroller();
	~CSHacontroller();
	int GetKey();
	void SetKey( int aKey );

	// RPC Service Methods
	TResult cstr_createagent( void );
	TResult dstr_removeagent( int aArgs, int *aDeleteInstance );
	TResult startmobileagent( int aArgs );
	TResult stopmobileagent( int aArgs );
	TResult getmobileagentstatus( int aArgs );
	TResult setsingleoption( TOptionDesc aArgs );
	TResult removesingleoption( TOptionDesc aArgs );
	TResult addlistoption( TOptionDesc aArgs );
	TResult removelistoption( TOptionDesc aArgs );
	THaStatus getstatus( int aArgs );
	TResult destroytunnelid( THaTunnelID aArgs );
	THaTunnelList listtunnels( int aArgs );
	THaTunnelInfo gettunnelinfo( TGetTunnelRequest aArgs );
	void settimeout( TTimeoutRequest aArgs );

private:
	TResult CreateDynamicsConfigFile();
	TResult RemoveDynamicsConfigFile();
	TResult CreateAliasInterface();
	TResult RemoveAliasInterface();
	TResult CreateVirtualNetwork();
	TResult RemoveVirtualNetwork();

	TResult is_agent_running();
	void set_dynamics_error( TResult *result, TDynamicsCallInfo *cres );

private:
	int iKey;
	int iDynamicsCallTimeout;
	int iAliasHostAddress, iAliasInterfaceIndex;
	string iAliasInterfaceAddress;
	int iVirtualNetworkSegmentAddress;
	int iVirtualNetworkSegmentSize;
	int iVirtualNetworkSegmentNetmaskBitcount;

	CDynamicsConfigFile iDynamicsConfigFile;
	CAProcess *iAgentProcess;
	CInterfaceAlias iAgentInterface;
	CDynamicsCommand iDynamicsCommand;
};

#endif
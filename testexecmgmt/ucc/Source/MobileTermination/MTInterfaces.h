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
* Contains the abstract interfaces for the MT components.
*
*/



#ifndef __MTINTERFACES_H__
#define __MTINTERFACES_H__


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum
{
	INTERFACE_DISABLED,
	INTERFACE_CONNECTED
} TInterfaceStatus;

typedef struct {
	int iPhoneID;
} TPhoneData;

typedef enum
{
	DPE_NONE,
	DPE_NO_AIR_INTERFACE,
	DPE_NO_TE_CHANNEL,
	DPE_NO_TE_CHANNEL_CONNECTION,
	DPE_SEND_FAILED,
	DPE_NO_REMOTE_ADDRESS,
} TDataPathError;

/*******************************************************************************
 *
 * Interface on the Air Interface called by the DataLinkLayer (or whatever)
 *
 ******************************************************************************/
class IAirInterface
{
public:
	virtual TDataPathError SendPacket( char *data, int len, int *aErrCode ) = 0;
};


/*******************************************************************************
 *
 * Interface on the TE Channel -- called by the higher layer (demux/datalink)
 *
 ******************************************************************************/
class ITEChannel
{
public:
	virtual TDataPathError SendPacket( char *data, int len, int *aErrCode ) = 0;
};


/*******************************************************************************
 *
 * Interface on the datalink layer or signal stack, called by the demux and 
 * the air interface
 *
 ******************************************************************************/
class IProcessData
{
public:
	virtual TDataPathError ProcessTEData( char *data, int len, int *aErrCode ) = 0;
	virtual TDataPathError ProcessUUData( char *data, int len, int *aErrCode ) = 0;
	virtual void SetAirInterface( IAirInterface *aAirInterface ) = 0;
	virtual void SetTEChannel( ITEChannel *aTEChannel ) = 0;
};


/*******************************************************************************
 *
 * Interface on the filters called by the filter manager
 *
 ******************************************************************************/
class IFilter
{
public:
	virtual int ProcessIncomingData( char *data, int len ) = 0;
	virtual int ProcessOutgoingData( char *data, int len ) = 0;
};

#endif //__MTINTERFACES_H__

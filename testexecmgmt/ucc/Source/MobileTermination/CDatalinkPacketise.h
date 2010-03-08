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
* CDatalinkPacketise
* CDMA has no datalink protocol in the MT -- however -- since the incoming data 
* is from a stream and I'd like to at least have the ability to force whole frames
* to be lost -- I'm making a packetisation handler so that the outgoing UDP
* packets can contain exactly one PPP frame. Incoming data is not altered.
*
*/



#ifndef __CDATALINKPACKETISE_H__
#define __CDATALINKPACKETISE_H__


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "MTInterfaces.h"
#include "CPppFrame.h"
#include "CLog.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CDatalinkPacketise : public IProcessData
{
public:
	// Construction and Access
	CDatalinkPacketise( TPhoneData *aPhoneData, CLog *aLog );
	virtual ~CDatalinkPacketise();

	// IProcessData
	virtual TDataPathError ProcessTEData( char *data, int len, int *aErrCode );
	virtual TDataPathError ProcessUUData( char *data, int len, int *aErrCode );
	virtual void SetAirInterface( IAirInterface *aAirInterface );
	virtual void SetTEChannel( ITEChannel *aTEChannel );

private:
	IAirInterface *iAirInterface;
	ITEChannel *iTEChannel;
	TPhoneData *iPhoneData;
	CLog *iLog;

	CPppFrame iIncomingFrame;
	CPppFrame iOutgoingFrame;
};

#endif //__CDATALINKPACKETISE_H__

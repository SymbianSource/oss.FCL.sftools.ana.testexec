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
* CLOGPPPFILTER
* Prints out the details of all PPP frames that pass through the MT.
*
*/



#ifndef __CLOGPPPFILTER_H__
#define __CLOGPPPFILTER_H__

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "MTInterfaces.h"
#include "CLog.h"
#include "CPppFrame.h"


/*******************************************************************************
 *
 * Definition
 *
 ******************************************************************************/
#define MAXFRAMEDESCRIPTION	1024


 /*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CLogPppFilter : public IFilter
{
public:
	CLogPppFilter( TPhoneData *aPhoneData, CLog *aLog );
	virtual ~CLogPppFilter();
	virtual int ProcessIncomingData( char *data, int len );
	virtual int ProcessOutgoingData( char *data, int len );
	
private:
	void ProcessByte( CPppFrame *aFrame, char c, int *iOverflowFlag );
	char *CreateFrameDescription( CPppFrame *aFrame );
	int AddProtocolName( char *aFrameBuffer, int aFrameSize );
	void AddLcpMessageTypeString( int aMessageType );

	TPhoneData *iPhoneData;
	CLog *iLog;
	CPppFrame iOutgoingFrame;
	CPppFrame iIncomingFrame;
	char iFrameDescription[MAXFRAMEDESCRIPTION];
	int iOutgoingFrameOverflowFlag;
	int iIncomingFrameOverflowFlag;
};

#endif //__CLOGPPPFILTER_H__

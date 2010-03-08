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
* CDatalinkNull
* This datalink layer simply forwards data synchronously between the techannel
* and the air interface with no buffering or processing (except maybe some 
* filters attached to the module).
*
*/



#ifndef __CDATALINKNULL_H__
#define __CDATALINKNULL_H__

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "MTInterfaces.h"
#include "CLog.h"

/*******************************************************************************
 *
 * Class Definition
 *
 ******************************************************************************/
class CDatalinkNull : public IProcessData
{
public:
	// Construction and Access
	CDatalinkNull( TPhoneData *aPhoneData, CLog *aLog );
	virtual ~CDatalinkNull();

	// IProcessData
	virtual TDataPathError ProcessTEData( char *data, int len, int *aErrCode );
	virtual TDataPathError ProcessUUData( char *data, int len, int *aErrCode );
	void SetAirInterface( IAirInterface *aAirInterface );
	void SetTEChannel( ITEChannel *aTEChannel );

private:
	IAirInterface *iAirInterface;
	ITEChannel *iTEChannel;
	TPhoneData *iPhoneData;
	CLog *iLog;
};

#endif //__CDATALINKNULL_H__

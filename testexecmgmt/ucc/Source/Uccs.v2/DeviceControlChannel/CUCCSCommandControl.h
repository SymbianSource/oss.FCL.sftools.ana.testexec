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
* CUCCSCommandControl Class
*
*/



#ifndef _CUCCSCommandControl_H
#define _CUCCSCommandControl_H

/*****************************************************************************
 *
 * System Includes 
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/
#include "CommandControlErrors.h"
#include "../Core/UCCS_Interfaces.h"
#include "CProtocolTypes.h"
#include "CUCCSCommandProtocol.h"

/*****************************************************************************
 *
 *  Types
 *
 ****************************************************************************/

/*****************************************************************************
 *
 *  Defines
 *
 ****************************************************************************/
#define KMAXDATALENGTH			4096


/*****************************************************************************
 *
 * Class Definition
 *
 ****************************************************************************/
class CUCCSCommandControl : public IRemoteControl
{
public:
	CUCCSCommandControl( MUccsControl *aCallBackService, IOutput *aOutput );	
	~CUCCSCommandControl();
	void LocalUpdateMessageIn( int aCommandId, char *aMsg);
	void LocalUpdateMessageOut( int aCommandId );

	// from IRemoteControl
	virtual TCommandControlError Start( TRemoteControlTransport aTransport, char* aPortname, int *aErrorCode, int *aScriptError );
	
private:
	CUCCSCommandProtocol iProtocol;
	MUccsControl *iCallBackService;
	IOutput *iOutput;
	int iStopFlag;
	TRemoteControlTransport iTransport;
};

#endif

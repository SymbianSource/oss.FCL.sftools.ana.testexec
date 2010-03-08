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
* CApplicationControlNull
*
*/



#ifndef __CAPPLICATIONCONTROLNULL_H__
#define __CAPPLICATIONCONTROLNULL_H__


/*************************************************************************************
 *
 * Local Includes
 *
 ************************************************************************************/
#include "CommandControlErrors.h"
#include "../Core/UCCS_Interfaces.h"


/*************************************************************************************
 *
 * Types
 *
 ************************************************************************************/
typedef struct {
	int iUsecaseID;
	int iRendezvousCount;
	int iInteractiveFlag;
} TArgs;


/*************************************************************************************
 *
 * class CApplicationControlNull
 *
 ************************************************************************************/
class CApplicationControlNull : public IRemoteControl
{
public:
	CApplicationControlNull( MUccsControl *aCallBackService, IOutput *aOutput );	
	~CApplicationControlNull();

	// from IRemoteControl
	virtual TCommandControlError Start( TRemoteControlTransport aTransport, char* aPortname, int *aErrorCode, int *aScriptError );

private:
	int ParseArgumentString( char *str );

	MUccsControl *iCallBackService;
	IOutput *iOutput;
	TArgs iArgs;
};

#endif // __CAPPLICATIONCONTROLNULL_H__

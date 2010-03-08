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
* CSynchronisation
*
*/



#ifndef __CSYNCHRONISATION_H__
#define __CSYNCHRONISATION_H__


/****************************************************************************************
 *
 * Includes
 *
 ***************************************************************************************/
#include <windows.h>
#include "UCCS_Interfaces.h"

/****************************************************************************************
 *
 * CSynchronisation
 *
 ***************************************************************************************/
class CSynchronisation
{
public:
	CSynchronisation( IOutput *aOutput );
	~CSynchronisation();

	int SignalFromDevice();
	int WaitFromScript();
	int WaitFromDevice();
	int SignalFromScript();
	int RendezvousFromDevice();
	int RendezvousFromScript();
	void ClearSynchronisation();

private:
	int iObjectCleared;
	IOutput *iOutput;
	HANDLE hDeviceSideRendezvousSemaphore;
	HANDLE hPCSideRendezvousSemaphore;
	HANDLE hDeviceSignalSemaphore;
	HANDLE hScriptSignalSemaphore;
};


#endif //__CSYNCHRONISATION_H__

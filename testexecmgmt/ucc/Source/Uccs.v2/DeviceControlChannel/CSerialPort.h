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
* CSerialPort
*
*/



#ifndef __CSERIALPORT_H__
#define __CSERIALPORT_H__

/*************************************************************************************
 *
 * Includes
 *
 ************************************************************************************/
#include <windows.h>
#include "../Core/UCCS_Interfaces.h"

/*************************************************************************************
 *
 * CSerialPort Definition
 *
 ************************************************************************************/
class CSerialPort : public IPort
{
public:
	CSerialPort();
	~CSerialPort();

	int OpenPort( char *aComPort );
	void ClosePort();
	int ReceiveBytes( char *aBuff, int *aLength );
	int SendBytes( char *aBuff, int *aLength );

private:
	HANDLE iComPort;
};

#endif //__MUCCSCONTROL_H__

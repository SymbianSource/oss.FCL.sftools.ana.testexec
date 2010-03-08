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
* CConsolePort
*
*/



#ifndef __CCONSOLEPORT_H__
#define __CCONSOLEPORT_H__

/*********************************************************************
 *
 * Definitions
 *
 ********************************************************************/
#define MAXLINESIZE 1024

/*************************************************************************************
 *
 * Local Includes
 *
 ************************************************************************************/
#include <windows.h>
#include "CProtocolTypes.h"
#include "../Core/UCCS_Interfaces.h"

#define E8DataLen	8
#define E4DataLen	4

/*************************************************************************************
 *
 * CNetworkPort Definition
 *
 ************************************************************************************/
class CConsolePort : public IPort
{
public:
	CConsolePort();
	~CConsolePort();

	int OpenPort( char *port );
	void ClosePort();
	int ReceiveBytes( char *aBuff, int *aLength );
	int SendBytes( char *aBuff, int *aLength );

private:
	int ParseNextLine( int *uid );
	int GetTPCommand( int aCommandID, TPCommand* aTPCommand );
	int ReadHeader( char* aBuff, int *aSize);
	int ReadData( char* aBuff, int *aSize);

	int iReadingData;	
	int iUcid;			// Member variable to reocrd the last usecase id read on the commandline
	int iCommand;		// Member varaible to record the last command read on the commandline
	char iCommandLine[MAXLINESIZE];

};

#endif //__CCONSOLEPORT_H__

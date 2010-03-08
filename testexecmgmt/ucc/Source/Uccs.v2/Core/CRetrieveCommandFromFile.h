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
* CRetrieveCommandFromFile
*
*/



#ifndef __CRETRIEVECOMMANDFROMFILE_H__
#define __CRETRIEVECOMMANDFROMFILE_H__


/***********************************************************************************
 *
 * System Includes
 *
 **********************************************************************************/
#include <stdio.h>


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_Interfaces.h"


/***********************************************************************************
 *
 * CRetrieveCommandFromFile
 *
 **********************************************************************************/
class CRetrieveCommandFromFile : public IRetrieveCommand
{
public:
	CRetrieveCommandFromFile();
	~CRetrieveCommandFromFile();

	virtual int StartUseCase( int aUsecaseID );
	virtual int EndUseCase();
	virtual int GetNextCommand( char *aBuffer, int aLength );

private:
	FILE *iFile;
	int iUsecaseID;
};

#endif //__CRETRIEVECOMMANDFROMFILE_H__

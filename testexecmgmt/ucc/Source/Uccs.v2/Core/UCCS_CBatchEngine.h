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
* Filename: UCCS_CBatchEngine.h
*
*/



#ifndef __UCCSBATCHENGINE_H__
#define __UCCSBATCHENGINE_H__


/***********************************************************************************
 *
 * System Includes
 *
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


/***********************************************************************************
 *
 * Local Includes
 *
 **********************************************************************************/
#include "UCCS_Interfaces.h"	
#include "UCCS_CExecuteCommand.h"
#include "CSynchronisation.h"


/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
#define MAXFILENAMELENGTH		256
#define INVALID_USECASE_ID		-1

#define ETS_IDLE				0
#define ETS_EXECUTING_SCRIPT	1
#define ETS_COMPLETED_SCRIPT	2

#define CTS_IDLE				0
#define CTS_USECASE_STARTED		1
#define CTS_USECASE_ENDED		2

/***********************************************************************************
 *
 * Definitions
 *
 **********************************************************************************/
class CBatchEngine : public MUccsControl
{
	public:
		
		//methods
		CBatchEngine( IRetrieveCommand *aRetrieveCommand, IOutput *aOutput );
		~CBatchEngine();	

		// From MUccsControl - THREAD A
		int StartUsecase( int aUsecaseID );
		int Signal( int aUsecaseID );
		int Rendezvous( int aUseCaseID );
		int Wait( int aUseCaseID );
		int EndUsecase( int aUsecaseID, int aResult, int *aScriptResult);
		int GetEnvVariable(char *aVariableName, char *aOutputBuffer, int aOutputBufferLen);
		int RunCommand( char* aCommandLine );

		// Script execution - THREAD B
		int ExecuteScript( void );
			
	private:
		
		// shared state that is static for the lifetime of batchengine 
		IRetrieveCommand *iRetrieveCommand;
		IOutput *iOutput;

		// shared state -- note that these are set (NOT UPDATED!!) by one thread and
		// read by another so there is no need for synchronisation
		int iExecutionThreadState;
		int iControlThreadState;
		int iLastError;

		// shared synchronisation state
		CSynchronisation *iSync;

		// THREAD A state
		int iUsecaseID;
		HANDLE hThreadHandle;

		// THREAD B state
		CExecuteCommand *iExecuteCommand;
};

//--------------------------------------------------------------------------------------------------

#endif __UCCSBATCHENGINE_H__

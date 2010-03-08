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
* UCCS_CExecuteCommand - Executes commands on the service manager. Maintains the 
* intercommand logic.
*
*/



#ifndef __CEXECUTECOMMAND_H__
#define __CEXECUTECOMMAND_H__


/****************************************************************************************
 *
 * Local Includes
 *
 ***************************************************************************************/
#include "UCCS_CServMgr.h"
#include "UCCS_CCommandDecoder.h"
#include "UCCS_CDataRecord.h"
#include "CSynchronisation.h"

/****************************************************************************************
 *
 * Definitions
 *
 ***************************************************************************************/
#define MAXRECENTCOMMANDS 256

/****************************************************************************************
 *
 * Types
 *
 ***************************************************************************************/

/****************************************************************************************
 *
 * Prototypes
 *
 ***************************************************************************************/
const char *GetScriptCommandString( int aCommandID );

/****************************************************************************************
 *
 * CExecuteCommand
 *
 ***************************************************************************************/
class CExecuteCommand
{
public:
	CExecuteCommand( CSynchronisation *aSync, IOutput *aOutput );
	~CExecuteCommand();
	int ExecuteCommand( char *aCommandLine );
	int GetEnvironmentVariable ( char *aVariableName, char *aOutputBuffer, int aOutputBufferSize );

private:
	int ExecuteCommand( char *aCommand, char *aArgs );
	int InternalExecuteCommand( TUccsCommand aCmd, CDataRecord *aArgs );

	int HandleHelp( CDataRecord *aArgs );
	int HandleRunCmd( CDataRecord *aArgs );
	int HandlePrintLastCommand( CDataRecord *aArgs );
	int HandlePrintLastResult( CDataRecord *aArgs );
	int HandleAssignment( CDataRecord *aArgs );
	int HandleWaitFor( CDataRecord *aArgs );
	int HandleWaitForNot( CDataRecord *aArgs );
	int HandleRequire( CDataRecord *aArgs );
	int HandleRequireNot( CDataRecord *aArgs );
	int HandleCheck( CDataRecord *aArgs );
	int HandleCheckNot( CDataRecord *aArgs );
	int HandleWaitForSignal( CDataRecord *aArgs );
	int HandleSignal( CDataRecord *aArgs );
	int HandleRendezvous( CDataRecord *aArgs );
	int HandleReset( CDataRecord *aArgs );
	int HandleResetAll( CDataRecord *aArgs );

	void AddToCommandContext( TUccsCommand aCommand, CDataRecord *aRec );
	void ClearCommandContext();

	CCommandDecoder *iCommandDecoder;
	CServiceManager *iServiceManager;

	TUccsCommand iCommandContextCommands[MAXRECENTCOMMANDS];
	CDataRecord *iCommandContextArgs[MAXRECENTCOMMANDS];
	int iCommandContextSize;
	CDataRecord *iLastResult;
	CDataRecord *iEnvironment;
	IOutput *iOutput;
	CSynchronisation *iSync;
};

#endif //__CEXECUTECOMMAND_H__

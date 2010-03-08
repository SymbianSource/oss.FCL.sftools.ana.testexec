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
* Filename: UCCS_Interfaces.h
* Service interfaces
*
*/



#ifndef __UCCSINTERFACES_H__
#define __UCCSINTERFACES_H__


/*************************************************************************************
 *
 * Local Includes
 *
 ************************************************************************************/
#include "UCCS_CDataRecord.h"
#include "../DeviceControlChannel/CommandControlErrors.h"


/*************************************************************************************
 *
 * IRetrieveCommand - interface on the command retriever called by the batch engine
 *
 ************************************************************************************/
class IRetrieveCommand
{
public:
	virtual int StartUseCase( int aUsecaseID ) = 0;
	virtual int EndUseCase() = 0;
	virtual int GetNextCommand( char *aBuffer, int aLength ) = 0;
};



/*************************************************************************************
 *
 * IOutput - interface on the output object called by everyone
 *
 ************************************************************************************/
typedef enum {
	UC_INVALID,
	UC_QUIT,
	UC_RUNCMD,
	UC_ASSIGN,
	UC_WAITFOR,
	UC_WAITFORSIGNAL,
	UC_RENDEZVOUS,
	UC_REQUIRE,
	UC_REQUIRENOT,
	UC_CHECK,
	UC_CHECKNOT,
	UC_PRINTENVIRONMENT,
	UC_WAITFORNOT,
	UC_SIGNAL,
} TUccsCommand;


class IOutput
{
public:
	virtual void StartUsecase( int aID ) = 0;
	virtual void EndUsecase( int aID, int aResult ) = 0;
	virtual void Error( int aErrorCode, char *aErrorString ) = 0;
	virtual void CompletedScript() = 0;

	virtual void ExecuteCommand( TUccsCommand aCommand, CDataRecord *aArgs ) = 0;
	virtual void ExecuteCommandResult( int aUccsErrorcode ) = 0;
	virtual void ExecuteCommandReply( CDataRecord *aReply ) = 0;

	virtual void DisplayHelp() = 0;
	virtual void DisplayLastCommand( CDataRecord *aCommand ) = 0;
	virtual void DisplayLastReply( CDataRecord *aResult ) = 0;
	virtual void DisplayEnvironment( CDataRecord *aEnvironment ) = 0;

	virtual void ExecuteString( char *aCommandLine ) = 0;
	virtual void ExecuteStringResult( int aUccsErrorcode ) = 0;

	virtual void StartServiceResult( int aServiceID, char *aServiceHost, int aStubReturnCode, int aErrorCode, int aUnused ) = 0;
	virtual void StopServiceResult( int aServiceID, char *aServiceHost, int aStubReturnCode, int aErrorCode, int aUnused ) = 0;
};


/*************************************************************************************
 *
 * IPort - interface on the transport port called by the remote control
 *
 ************************************************************************************/
class IPort
{
public:
	virtual int OpenPort( char *aComPort ) = 0;
	virtual void ClosePort() = 0;
	virtual int ReceiveBytes( char *aBuff, int *aLength ) = 0;
	virtual int SendBytes( char *aBuff, int *aLength ) = 0;
};


/*************************************************************************************
 *
 * IRemoteControl - interface on the remote control input guy called by the creator
 * and the batch engine
 *
 ************************************************************************************/
typedef enum {
	RCI_NONE,
	RCI_SERIAL,
	RCI_SERVERSOCKET,
	RCI_CONSOLE,
	RCI_TCP
} TRemoteControlTransport;

class IRemoteControl
{
public:
	virtual TCommandControlError Start( TRemoteControlTransport aTransport, char* aPortname, int *aErrorCode, int *aScriptError ) = 0;
};


/*************************************************************************************
 *
 * IService - interface on the service stubs called by the service manager
 *
 ************************************************************************************/
class IService
{

	public:
		virtual int StartUccsService( char *aHostName, int *aLocalError, int *aRemoteError ) = 0;
		virtual int StopUccsService( int *aLocalerror, int *aRemoteError ) = 0;
		virtual CDataRecord* IssueCommand(CDataRecord* aCommand) = 0;		//return result of the actual command
		virtual int GetStatus() = 0;
		//virtual int RegisterObserver(aNotify) = 0;
};


/*************************************************************************************
 *
 * MUccsControl - interface on the batch engine called by the remote controller
 *
 ************************************************************************************/
class MUccsControl
{
public:
	virtual int StartUsecase( int aUsecaseID ) = 0;
	virtual int Signal( int aUsecaseID ) = 0;
	virtual int Rendezvous( int aUseCaseID ) = 0;
	virtual int Wait( int aUseCaseID ) = 0;
	virtual int EndUsecase( int aUsecaseID, int aResult, int *aScriptResult ) = 0;
	virtual int GetEnvVariable( char *aVariableName, char *aOutputBuffer, int aOutputBufferLen ) = 0;
	virtual int RunCommand( char* aCommandLine ) = 0;
};


#endif __UCCSINTERFACES_H__

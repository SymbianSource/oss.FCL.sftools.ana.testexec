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
* procexec.h
* Process execution class which can be used to execute a process and
* optionally record all the processes output.
*
*/


#ifndef __CAPROCESS_H__
#define __CAPROCESS_H__

/////////////////////////////////////////////////////////////////////////////////////////////
//
// System Includes
//
/////////////////////////////////////////////////////////////////////////////////////////////
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
/////////////////////////////////////////////////////////////////////////////////////////////
typedef enum { 
  CAE_NONE,
  CAE_INVALID_STATE,
  CAE_FAILED_TO_CREATE_PIPE,
  CAE_FORK_FAILED,
  CAE_FAILED_TO_SET_NONBLOCKING,
  CAE_SIGNAL_FAILED,
  CAE_TIMEOUT,
  CAE_WAITPID_FAILED,
  CAE_SELECT_FAILED,
  CAE_READ_FAILED,
} TCAProcessError;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Types
//
/////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
  PS_INVALID,
  PS_INIT,
  PS_STARTED,
  PS_STOPPED,
  PS_ABANDONNED
} TProcessStatus;

typedef enum {
  // If process is in PS_INIT or PS_STARTED state then the value is invalid
  ER_INVALID,

  // If process is in PS_STOPPED state then it must be one of the 
  ER_UNKNOWN,
  ER_EXITED,
  ER_SIGNALLED,

  // If process is PS_ABANDONED state then it must be one of the following. Processes 
  // are abandonned if something goes wrong that we can't recover from.
  ER_SIGNALFAILED,
  ER_STOPFAILED,
  ER_WAITPIDFAILED
} TProcessExitReason;


/////////////////////////////////////////////////////////////////////////////////////////////
//
// CAProcess definition
//
/////////////////////////////////////////////////////////////////////////////////////////////
class CAProcess
{
 public:

  CAProcess();
  ~CAProcess();
  
  // This function will run the process and wait for it to finish. It is intended as a single call 
  // function.  Use the other functions for more control of the process. Do not mix the two.
  // Result:
  //          CAE_FORK_FAILED / CAE_FAILED_TO_CREATE_PIPE / CAE_FAILED_TO_SET_NON_BLOCKING - PS_INIT
  TCAProcessError Execute( const char *aCommand, int *aErrorCode, int aTimeoutInMilliseconds = -1, 
			   string *aStandardOutput = NULL, string *aStandardError = NULL );

  // The basic process control functions
  TCAProcessError StartProcess( const char *aCommand, int *aErrorCode, bool aRecordStdOut = true, 
				bool aRecordStdErr = true, bool aMakeNewProcessGroup = false );
  TCAProcessError RequestStop( int aSignal );
  TCAProcessError GetProcessStatus( TProcessStatus *aProcessStatus );
  TCAProcessError WaitForProcessToTerminate( int aMaxWait );

  // Poll for new output on either stdout or stdin. If there is no new data then the method will
  // wait up to aMaxWait seconds for new data to show up. The Get- methods will return all data
  // received since the last call. This data will then be removed from the object's buffers.
  TCAProcessError PollProcessForNewOutput( int aMaxWait, int *aStandardOutputRead, int *aStandardErrorRead );
  int GetRecordedStandardOutput( string *aStdout, int *aReadError );
  int GetRecordedStandardError( string *aStdout, int *aReadError );

  // Accessor methods
  string GetCommandString();
  TCAProcessError GetExitReason( TProcessExitReason *aExitReason );
  TCAProcessError GetExitCode( int *aExitCode );
  
 private:
  // help methods
  TCAProcessError CreatePipes( int *aErrorCode );  
  void ClosePipes();
  void ClosePipePair( int *aPipes );
  void ClosePipeDesc( int *aPipeDescriptor );
  TProcessExitReason GetExitReasonFromStatus( int aStatus, int *aExitCode );
  TCAProcessError ReadOutput( int *aFileDes, string *aBuffer, bool aStoreFlag, int *aReadError );
  TCAProcessError InternalPollProcessForNewOutput( int aMaxWait, int *aStandardOutputRead, int *aStandardErrorRead );

  // state variables
  TProcessStatus iProcessStatus;

  // information variables
  string *iCommand;
  int  iPID;
  bool iRecordStdOut;
  bool iRecordStdErr;
  TProcessExitReason iProcessExitReason;
  int iProcessExitCode;
  int iReadStdoutError;
  int iReadStderrError;

  // File descriptors for process output
  int iStdInPipe[2];
  int iStdOutPipe[2];
  int iStdErrPipe[2];

  // Current output of the process.
  string iRecordedStdOut;
  string iRecordedStdErr;
};

#endif

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
* Switches
*
*/



/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "proclib.h"
#include "../include/standard_unix.h"


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/
#ifdef UT10
#undef fork
#define fork() (-1)
#endif


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define INVALID_PIPE_DESC           (-1)
#define INVALID_PID                 (0)
#define READ_BUFFER_SIZE            (128)
#define EXECUTE_PAUSE_PERIOD        (100000)


/*******************************************************************************
 *
 * PUBLIC: Constructor
 *
 ******************************************************************************/
CAProcess::CAProcess( void )
{
  // initialise all variables
  iCommand = NULL;
  iProcessStatus = PS_INIT;
  iProcessExitReason = ER_INVALID;
  iProcessExitCode = 0;
  iPID = INVALID_PID;
  iRecordStdOut = false;
  iRecordStdErr = false;
  iStdInPipe[0] = iStdInPipe[1] = INVALID_PIPE_DESC;
  iStdOutPipe[0] = iStdOutPipe[1] = INVALID_PIPE_DESC;
  iStdErrPipe[0] = iStdErrPipe[1] = INVALID_PIPE_DESC;
  iReadStdoutError = 0;
  iReadStderrError = 0;
}

/*******************************************************************************
 *
 * PUBLIC: Destructor 
 *
 ******************************************************************************/
CAProcess::~CAProcess()
{
  // I don't allow the object to be destroyed in the PS_STARTED state
  assert( iProcessStatus != PS_STARTED );

  // clean up any handles
  if( iCommand != NULL ) {
    delete iCommand;
    iCommand = NULL;
  }
  ClosePipes();
} 

/*******************************************************************************
 *
 * SECTION-1: PROCESS CONTROL 
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * PUBLIC: StartProcess 
 *
 ******************************************************************************/
TCAProcessError CAProcess::StartProcess( const char *aCommand, int *aErrorCode, bool aRecordStdOut, 
					 bool aRecordStdErr, bool aMakeNewProcessGroup )
{
  int err;

  // check params 
  assert( aCommand != NULL );
  assert( aErrorCode != NULL );
  *aErrorCode = 0;

  // make sure the process is in the init state
  if( iProcessStatus != PS_INIT ) {
    return CAE_INVALID_STATE;
  }

  // verify that the recorded output is empty -- just checking no silly buggers
  assert( iRecordedStdOut.empty() == true );
  assert( iRecordedStdErr.empty() == true );

  // create the pipes that will be setup as the child process's input and output channels. Make the reading 
  err = CreatePipes( aErrorCode );
  if( err != CAE_NONE ) {
    return (TCAProcessError)err;
  }

  // flush all stream before forking (AE: why???)
  fflush( NULL );

  // fork now
  iPID = fork();

  // if an error occurred then return failed
  if( iPID == -1 ) {
    ClosePipes();
    iPID = INVALID_PID;
    *aErrorCode = errno;
    return CAE_FORK_FAILED;
  }

  // if success and we are the parent then...
  if( iPID > 0 ) { 

    // Close the write end of the output pipes and the read end of the input pipe
  	ClosePipeDesc( &(iStdInPipe[0]) );
  	ClosePipeDesc( &(iStdOutPipe[1]) );
  	ClosePipeDesc( &(iStdErrPipe[1]) );

    // Update the state
    iProcessStatus = PS_STARTED;
    iCommand = new string( aCommand );
    iRecordStdOut = aRecordStdOut;
    iRecordStdErr = aRecordStdErr;
    
    // done
    return CAE_NONE;
  }

  // if success and we are the child then...
  if( iPID == 0 ) {

    // if requested - put the process in it's own process group
    if( aMakeNewProcessGroup ) {
      err = setpgid( 0, 0 );
      if( err != 0 ) {
        fprintf( stderr, "WARNING: failed to setpgid for process %d - %s (%d).\n", getpid(), strerror(errno), errno );
      }
    } 

    // close the write end of stdin and make the read end desc 0
  	ClosePipeDesc( &(iStdInPipe[1]) );
    close( stdin->_fileno );
    err = dup( iStdInPipe[0] );
    assert( err == (stdin->_fileno) );

    // close the read end of stdout and make the write end desc 1
    if( aRecordStdOut ) {
      ClosePipeDesc( &(iStdOutPipe[0]) );
      close( stdout->_fileno );
      err = dup( iStdOutPipe[1] );
      assert( err == (stdout->_fileno) );
    }

    // close the read end of stderr and make the write end desc 2
    if( aRecordStdErr ) {
      ClosePipeDesc( &(iStdErrPipe[0]) );
      close( stderr->_fileno );
      err = dup( iStdErrPipe[1] );
      assert( err == (stderr->_fileno) );
    }

    // NOTE: A problem exists that because we have inherited all the descriptors
    // form our parent, any descriptors that are subsequently closed by the
    // parent will still be open until the child exits.
    
    // exec the target image. There is a choice here of whether to exec the target
    // directly or to use /bin/sh. The tradeoff is that /bin/sh gives you argument
    // parsing -- but exec will not fail with an invalid target because /bin/sh
    // is always available to exec. Currently I'm choosing the former but you 
    // should be aware of the tradeoff.m
    execl("/bin/sh", "csh", "-c", aCommand, NULL);
    //execl( aCommand, NULL );

    // The call to execl() failed.  The following error message will be
    // sent back as if it came from the process (on stderr). The process
    // then exits.
    fprintf( stderr, "ERROR: execl() failed with error '%s' (%d)\n", strerror(errno), errno );
    ClosePipeDesc( &(iStdInPipe[0]) );
    ClosePipeDesc( &(iStdOutPipe[1]) );
    ClosePipeDesc( &(iStdErrPipe[1]) );
    exit( -1 );
  }

  // should never get here
  assert( !"Invalid code path" );
  return CAE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC: RequestStop 
 *
 ******************************************************************************/
TCAProcessError CAProcess::RequestStop( int aSignal )
{
  int err;

  // get the state
  err = GetProcessStatus( NULL );
  assert( err == CAE_NONE );

  // check the state
  if( iProcessStatus != PS_STARTED ) {
    return CAE_INVALID_STATE;
  }

  // send the specified signal to the process
  err = kill( iPID, aSignal );
  
  // if kill() fails then we abandon the process -- STATE TRANSITION
  if( err != 0 ) {
    iProcessStatus = PS_ABANDONNED;
    iProcessExitReason = ER_SIGNALFAILED;
    iProcessExitCode = errno;
    ClosePipes();
    return CAE_SIGNAL_FAILED;
  }

  // otherwise all is ok
  return CAE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC: GetProcessStatus
 *
 ******************************************************************************/
TCAProcessError CAProcess::GetProcessStatus( TProcessStatus *aProcessStatus )
{
  int err;
  int status;

  // if the process is in the PS_STARTED state then we have to verify that it is still running
  if( iProcessStatus == PS_STARTED ) {

    // see if the process is still running 
    assert( iPID > 0 );
    err = waitpid( iPID, &status, WNOHANG );

    // if an error occured then transition from started to abandonned state
    if( err == -1 ) {
      iProcessStatus = PS_ABANDONNED;
      iProcessExitReason = ER_WAITPIDFAILED;
      iProcessExitCode = errno;
    }

    // if the process is still running then no change
    if( err == 0 ) {
    }

    // if the process has finished then transition from started to stopped state
    if( err > 0 ) {
      iProcessStatus = PS_STOPPED;
      iProcessExitReason = GetExitReasonFromStatus( status, &iProcessExitCode );
    }
  }      
    
  // if the passed pointer is non-null then we return the status
  if( aProcessStatus != NULL ) {
    *aProcessStatus = iProcessStatus;
  }

  // done
  return CAE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC: WaitForProcessToTerminate
 *
 ******************************************************************************/
TCAProcessError CAProcess::WaitForProcessToTerminate( int aMaxWait )
{
  int err;
  int status;
  int i;

  // if the process isn't in the PS_STARTED state then this call is invalid -- note
  // that GetProcessStatus is not used to update the status here since if the 
  // process has stopped we want this to be caught in the waitpid below, not here.
  if( iProcessStatus != PS_STARTED ) {
    return CAE_INVALID_STATE;
  }

  // Wait for the process. We can't just do a blocking wait since the stdout/stderr
  // pipes would overflow and people wouldn't be able to read the output as 
  // expected when the process has terminated. So we do a non-blocking wait in a loop.
  for( i = 0; (aMaxWait == -1) || (i < aMaxWait); i++ ) {
    
    // see if the process has finished
    err = waitpid( iPID, &status, WNOHANG );
    if( err != 0 ) {
      break;
    }

    // see if there is any data to read in from the pipes -- this will transfer the data 
    // from the pipes to the internal buffers
    InternalPollProcessForNewOutput( 0, NULL, NULL );
  }

  // see if we've timedout
  if( err == 0 ) {
    assert( i == aMaxWait );
    return CAE_TIMEOUT;
  }
   
  // if the call failed otherwise we abandon the process
  if( err == -1 ) {
    iProcessStatus = PS_ABANDONNED;
    iProcessExitReason = ER_WAITPIDFAILED;
    iProcessExitCode = errno;
    return CAE_WAITPID_FAILED;
  }

  // otherwise everything is good and we have stopped the process
  assert( err > 0 );
  iProcessStatus = PS_STOPPED;
  iProcessExitReason = GetExitReasonFromStatus( status, &iProcessExitCode );
  return CAE_NONE;
}

/*******************************************************************************
 *
 * PUBLIC: Execute() 
 *
 ******************************************************************************/
TCAProcessError CAProcess::Execute( const char *aCommand, int *aErrorCode, int aTimeoutInMilliseconds, string *aStandardOutput, 
				    string *aStandardError )
{
	TCAProcessError err;
	int i, stdout_bytes_read, stderr_bytes_read, errcode, max_iterations, timeout_in_microseconds;
	TProcessStatus pstatus;
	bool record_std_out, record_std_err;
	struct timespec pause_period = { 0, EXECUTE_PAUSE_PERIOD };

	// validate params
	assert( aCommand != NULL );
	assert( aErrorCode != NULL );
	record_std_out = (aStandardOutput != NULL);
	record_std_err = (aStandardError != NULL);
	
	// start the process
	err = StartProcess( aCommand, aErrorCode, record_std_out, record_std_err, false );
	if( err != CAE_NONE ) {
		return err;
	}

	// work out the maximum number of iterations
	timeout_in_microseconds = aTimeoutInMilliseconds * 1000;
	max_iterations = (timeout_in_microseconds / EXECUTE_PAUSE_PERIOD);
	//	fprintf( stderr, "DEBUG: timeout_in_micro = %d, pause_period = %d, timeout = %d, max_iterations = %d\n", 
	//		 timeout_in_microseconds, EXECUTE_PAUSE_PERIOD, aTimeoutInMilliseconds, max_iterations );

	// wait for the process to exit - store the output
	for( i = 0; (i < max_iterations) || (aTimeoutInMilliseconds == -1); i++ ) {

		// check the status of the process 
		err = GetProcessStatus( &pstatus );
		assert( err == CAE_NONE );

		// sanity check on the status
		assert( (pstatus != PS_INVALID) && (pstatus != PS_INIT) );

		// if the status isn't PS_STARTED then we exit
		if( pstatus != PS_STARTED ) {
			break;
		}

		// get any new output - errors are ignored
		if( record_std_out || record_std_err ) {
			PollProcessForNewOutput( 0, &stdout_bytes_read, &stderr_bytes_read );
			GetRecordedStandardOutput( aStandardOutput, &errcode );
			GetRecordedStandardError( aStandardError, &errcode );
		}

		// now wait for one second and then lets do it all again!!
		nanosleep( &pause_period, NULL );
	}

	// get the current status
	err = GetProcessStatus( &pstatus );
	assert( err == CAE_NONE );

	// if we are still running then send a kill signal and wait to exit
	if( pstatus == PS_STARTED ) {
		err = RequestStop( SIGKILL );
		if( (err != CAE_NONE) && (err != CAE_INVALID_STATE) ) {
			return err;
		}
		err = WaitForProcessToTerminate( -1 );
		if( err != CAE_NONE ) {
			return err;
		}
	}

	// get any last minute output
	if( record_std_out || record_std_err ) {
	  PollProcessForNewOutput( 0, &stdout_bytes_read, &stderr_bytes_read );
	  GetRecordedStandardOutput( aStandardOutput, &errcode );
	  GetRecordedStandardError( aStandardError, &errcode );
	}
	
	// we have to return an indication of whether the process timed out
	return ((i == max_iterations) ? CAE_TIMEOUT : CAE_NONE);
}


/*******************************************************************************
 *
 * SECTION-2: DATA CONTROL 
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * PUBLIC: PollProcessForNewOutput. Polls the executing process for new output.
 * Any output is then transferred to the internal buffers.
 *
 ******************************************************************************/
TCAProcessError CAProcess::PollProcessForNewOutput( int aMaxWait, int *aStandardOutputRead, int *aStandardErrorRead )
{
  GetProcessStatus( NULL );
  if( iProcessStatus == PS_INIT ) {
    return CAE_INVALID_STATE;
  }
  return InternalPollProcessForNewOutput( aMaxWait, aStandardOutputRead, aStandardErrorRead );
}

TCAProcessError CAProcess::InternalPollProcessForNewOutput( int aMaxWait, int *aStandardOutputRead, int *aStandardErrorRead )
{
    struct timeval maxWait;
    fd_set readset;
    int fdsToProcess;
    int is_set;
    int err_stdout = 0;
    int err_stderr = 0;

    // initialise the out params
    if( aStandardOutputRead != NULL ) {
      *aStandardOutputRead = 0;
    }
    if( aStandardErrorRead != NULL ) {
      *aStandardErrorRead = 0;
    }

	// make sure one of the pipes is still open
	if( (iStdOutPipe[0] == INVALID_PIPE_DESC) && (iStdErrPipe[0] == INVALID_PIPE_DESC) ) {
		return CAE_NONE;
	}

	// Perform a select (of upto the maximum allowed time) and read in the available data.
    FD_ZERO( &readset );
	if( iStdOutPipe[0] != INVALID_PIPE_DESC ) {
	    FD_SET( iStdOutPipe[0], &readset );
	}
	if( iStdErrPipe[0] != INVALID_PIPE_DESC ) {
	    FD_SET( iStdErrPipe[0], &readset );
	}
    maxWait.tv_sec = aMaxWait;
    maxWait.tv_usec = 0;
    fdsToProcess = select( FD_SETSIZE, &readset, NULL, NULL, &maxWait );
    if( fdsToProcess < 0 ) {
      iReadStdoutError = iReadStderrError = errno;
      return CAE_SELECT_FAILED;
    }

    // If there is no data to read then exit
    if( fdsToProcess == 0 ) {
      return CAE_NONE;
    }

    // Read the stdout pipe
	if( iStdOutPipe[0] != INVALID_PIPE_DESC ) {
	    is_set = FD_ISSET( iStdOutPipe[0], &readset );
		if( is_set != 0 ) {
			err_stdout = ReadOutput( &(iStdOutPipe[0]), &iRecordedStdOut, iRecordStdOut, &iReadStdoutError );
			if( aStandardOutputRead != NULL ) {
				*aStandardOutputRead = iRecordedStdOut.length();
			}
		}
    }

    // Read the stderr pipe
	if( iStdErrPipe[0] != INVALID_PIPE_DESC ) {
	    is_set = FD_ISSET( iStdErrPipe[0], &readset );
		if( is_set != 0 ) { 
			err_stderr = ReadOutput( &(iStdErrPipe[0]), &iRecordedStdErr, iRecordStdErr, &iReadStderrError );
			if( aStandardErrorRead != NULL ) {
				*aStandardErrorRead = iRecordedStdErr.length();
			}
		}
    }
    
    // If one of the reads returned an error then return this
    if( (err_stdout != 0) || (err_stderr != 0) ) {
      return CAE_READ_FAILED;
    }
    return CAE_NONE;
}


/*******************************************************************************
 *
 * PUBLIC: GetRecordedStandardOutput
 *
 ******************************************************************************/
int CAProcess::GetRecordedStandardOutput( string *aStdout, int *aReadError )
{
  // validate params
  assert( aReadError != NULL );
  if( aStdout == NULL ) {
    return 0;
  }

  // append a copy of the stored data to the passed string
  (*aStdout) += iRecordedStdOut;

  // clear the current string
  iRecordedStdOut.erase( 0, iRecordedStdOut.length() );

  // set the read error 
  *aReadError = iReadStdoutError;
  iReadStdoutError = 0;

  // return the number of bytes read
  return aStdout->size();
}


/*******************************************************************************
 *
 * PUBLIC: GetRecordedStandardError
 *
 ******************************************************************************/
int CAProcess::GetRecordedStandardError( string *aStdErr, int *aReadError )
{
  // validate params
  assert( aReadError != NULL );
  if( aStdErr == NULL ) {
    return 0;
  }
  
  // append a copy of the stored data to the passed string
  (*aStdErr) += iRecordedStdErr;

  // clear the current string
  iRecordedStdErr.erase( 0, iRecordedStdErr.length() );

  // set the read error 
  *aReadError = iReadStderrError;
  iReadStderrError = 0;

  // return the number of bytes read
  return aStdErr->size();
}


/*******************************************************************************
 *
 * SECTION-3: General Accessors
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * PUBLIC: GetCommandString
 *
 ******************************************************************************/
string CAProcess::GetCommandString()
{
  string cmd;
  if( iProcessStatus == PS_INIT ) {
    return cmd;
  }
  cmd = *iCommand;
  return cmd;
}

/*******************************************************************************
 *
 * PUBLIC: GetExitReason
 *
 ******************************************************************************/  
TCAProcessError CAProcess::GetExitReason( TProcessExitReason *aExitReason )
{
  // check the state -- otherwise this is not valid
  if( (iProcessStatus != PS_STOPPED) && (iProcessStatus != PS_ABANDONNED) ) {
    return CAE_INVALID_STATE;
  }

  // set the value
  if( aExitReason != NULL ) {
    *aExitReason = iProcessExitReason;
  }
 
  // done
  return CAE_NONE;
}

/*******************************************************************************
 *
 * PUBLIC: GetExitCode
 *
 ******************************************************************************/  
TCAProcessError CAProcess::GetExitCode( int *aExitCode )
{
  // check the state -- otherwise this is not valid
  if( (iProcessStatus != PS_STOPPED) && (iProcessStatus != PS_ABANDONNED) ) {
    return CAE_INVALID_STATE;
  }

  // set the value
  if( aExitCode != NULL ) {
    *aExitCode = iProcessExitCode;
  }
 
  // done
  return CAE_NONE;
}


/*******************************************************************************
 *
 * SECTION-4: Helpers
 *
 ******************************************************************************/  

/*******************************************************************************
 *
 * PRIVATE: GetExitReasonFromStatus
 *
 ******************************************************************************/
TProcessExitReason CAProcess::GetExitReasonFromStatus( int aStatus, int *aExitCode )
{
  assert( aExitCode != NULL );
  if( WIFEXITED(aStatus) ) {
    *aExitCode = WEXITSTATUS(aStatus);
    return ER_EXITED;
  }
  if( WIFSIGNALED(aStatus) ) {
    *aExitCode = WTERMSIG(aStatus);
    return ER_SIGNALLED;
  }
  return ER_UNKNOWN;
}

/*******************************************************************************
 *
 * PRIVATE: ReadOutput
 *
 ******************************************************************************/  
TCAProcessError CAProcess::ReadOutput( int *aFileDes, string *aBuffer, bool aStoreFlag, int *aReadError )
{
    char buff[READ_BUFFER_SIZE];
    int bytesRead;

    // check params
    assert( aBuffer != NULL );
    assert( aReadError != NULL );
    assert( aFileDes != NULL );

    // read until there is nothing left to read
    while( true ) {

      // read
      bytesRead = read( (*aFileDes), &buff, READ_BUFFER_SIZE-1 );
      
      // null-terminate the buffer
      if( bytesRead > 0 ) {
	buff[bytesRead] = 0;
      }

      // temporarily unavailable resource is not an error 
      if( (bytesRead == -1) && (errno == EAGAIN) ) {
	return CAE_NONE;
      }

      // check for error 
      if( bytesRead == -1 ) {
	fprintf( stderr, "DEBUG: read() returned error %s\n", strerror(errno) );
	*aReadError = errno;
	return CAE_READ_FAILED;
      }

      // check for no more data -- if this is the case then close the pipe
      if( bytesRead == 0 ) {
	ClosePipeDesc( aFileDes );
	return CAE_NONE;
      }

      // store the data in the object if requested 
      if( aStoreFlag ) {
	fflush( stderr );
	(*aBuffer) += buff;
      }
    }
 
    // should never get here
    assert( !"Invalid code path" );
    return CAE_NONE;
}

/*******************************************************************************
 *
 * PRIVATE: CreatePipes / ClosePipes / ClosePipePair
 *
 ******************************************************************************/
TCAProcessError CAProcess::CreatePipes( int *aErrorCode )
{
  int err;
  int flags;

  // check params
  assert( aErrorCode != NULL );

  // create the stdin pipe
  err = pipe( iStdInPipe );
  if( err != 0 ) {
    *aErrorCode = errno;
    return CAE_FAILED_TO_CREATE_PIPE;
  }

  // create the stdout pipe
  err = pipe( iStdOutPipe );
  if( err != 0 ) {
    ClosePipes();
    *aErrorCode = errno;
    return CAE_FAILED_TO_CREATE_PIPE;
  }

  // create the stderr pipe
  err = pipe( iStdErrPipe );
  if( err != 0 ) {
    ClosePipes();
    *aErrorCode = errno;
    return CAE_FAILED_TO_CREATE_PIPE;
  }

  // make the read end of the stdout pipe non-blocking
  flags = fcntl( iStdOutPipe[0], F_GETFL, 0 );
  flags |= O_NONBLOCK;
  err = fcntl( iStdOutPipe[0], F_SETFL, flags );
  if( err != 0 ) {
    ClosePipes();
    *aErrorCode = errno;
    return CAE_FAILED_TO_SET_NONBLOCKING;
  }

  // make the read end of the stderr pipe non-blocking    
  flags = fcntl( iStdErrPipe[0], F_GETFL, 0 );
  flags |= O_NONBLOCK;
  err = fcntl( iStdErrPipe[0], F_SETFL, flags );
  if( err != 0 ) {
    ClosePipes();
    *aErrorCode = errno;
    return CAE_FAILED_TO_SET_NONBLOCKING;
  }

  // OK
  return CAE_NONE;
}

void CAProcess::ClosePipes()
{
  ClosePipePair( iStdInPipe );
  ClosePipePair( iStdOutPipe );
  ClosePipePair( iStdErrPipe );
}


void CAProcess::ClosePipePair( int *aPipes )
{
	assert( aPipes != NULL );
	ClosePipeDesc( &(aPipes[0]) );
	ClosePipeDesc( &(aPipes[1]) );
}


void CAProcess::ClosePipeDesc( int *aPipeDescriptor )
{
	assert( aPipeDescriptor != NULL );
	close( *aPipeDescriptor );
	*aPipeDescriptor = INVALID_PIPE_DESC;
}


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
* Filename: CXServer.cpp
* Author: Sanjeet Matharu 
* This is the server side of the protocol which takes name/value pairs and does stuff
* System Includes
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "cxserver.h"

#define TIMEOUTPERIOD	15	//seconds

//------------------------------------------------------------------------------
//constructor
CXServer::CXServer()
{
	iStatus = RECEIVE_HELLO;	//initialise to RECEIVE_HELLO

	//initialise the name and value pairs
	for(int i = 0; i < MAXLISTLENGTH; i++)
	{
		memset( iList[i].iName, 0, MAXBUFFERLEN );
		memset( iList[i].iValue, 0, MAXBUFFERLEN );
	}

	iListPtr = NULL;					//this is used to point to either the name or the value in the list
	timeout.tv_sec = TIMEOUTPERIOD;		//set up socket timeout period
	timeout.tv_usec = 0;
}

//------------------------------------------------------------------------------
//destructor
CXServer::~CXServer()
{
	iStatus = RECEIVE_HELLO;	//end on RECEIVE_HELLO

	iListPtr = NULL;
	
	//clear the name and value pairs
	for(int i = 0; i < MAXLISTLENGTH; i++)
	{
		memset( iList[i].iName, 0, MAXBUFFERLEN );
		memset( iList[i].iValue, 0, MAXBUFFERLEN );
	}
}

//------------------------------------------------------------------------------
//this function adds the specified value into the list of name value pairs
int CXServer::OnExecute(int handle, int handletype)
{
	int err = 0;
	int buff = -1;

	//check the list for the next available space
	for(int i = 0; i < MAXLISTLENGTH; i++ ) 
	{
		if( *iList[i].iName == 0 ) 
		{
			buff = i;
			break;
		}
	}

	// check that a slot was found, return error if not
	if( buff == -1 ) 
	{
		return CSP_LISTFULL;
	}

	//if we get here and the status is value there is a problem and we'd loop like forever
	while( iStatus != RECEIVE_VALUE )
	{
		switch( iStatus )
		{
			case RECEIVE_HELLO:
				
				err = CheckForHello(handle, handletype, buff);
				
				//the was a problem with the socket connection
				if(err != 1)
				{
					return err;
				}

				break;
	
			case RECEIVE_NAME:

				err = CheckForData(handle, handletype, buff);
				
				if(err != 0)
				{
					return err;
				}
				
				break;

			case RECEIVE_GOODBYE:

				iStatus = RECEIVE_HELLO;
				break;

			default:

				break;
		}
	}

	// done
	return err;
}

//------------------------------------------------------------------------------
// This function checks to see whether the Hello call has been made by the client
// it returns OK or ERROR accordingly
int CXServer::CheckForHello(int handle, int handletype, int freeslot)
{
	char c;
	int counter = 0;
	int err;
	int buff = -1;
	int bufflen = 0;
	
	//initialisation
	sprintf(&c, "%c", "");
	sprintf(iHellobuffer, "%s", "HELLO");

	//ensure that there are some free slots available in the list
	assert(freeslot != -1);

	buff = freeslot;

	//set the pointer to point to the name element
	iListPtr = iList[buff].iName;

	while( 1 ) 
	{
		// receive the next byte
		err = Read( handle, handletype, &c );
				
		//if something has been read from the socket
		if( ( err == 1 ) )
		{
			//...and the character received is zero
			if( strcmp( &c, "" ) == 0)
			{
				//we have the complete string so compare with our buffer
				if(strcmp(iHellobuffer, iListPtr) != 0)
				{
					Write(handle, handletype, REPLY_ERROR, strlen(REPLY_ERROR) );
					//clear the buffer
					memset( iListPtr, 0, MAXBUFFERLEN );
					//reset bufferlen counter so that writing can start from the beginning next time
					bufflen = 0;
				}
				else	//data is ok
				{
					Write( handle, handletype, iListPtr, strlen(iListPtr) );
					Write( handle, HT_SOCKET, "", 1 );
					break;
				}
			}
			else
			{
				// add the byte to the buffer
				iListPtr[bufflen++] = c;
			}
		}
		else	//if
		{
			//if there was a dodgy read then the socket link is broken
			memset( iListPtr, 0, MAXBUFFERLEN );
			bufflen = 0;
			err = GetLastError();
			printf( "Data could not be read.  Error code is %d...\n", err);
			return err;
		}
	}	//while

	//update the status
	iStatus = RECEIVE_NAME;

	//since the first element in the list will have HELLO in it, it should be reinitialised for use later on
	memset( iListPtr, 0, MAXBUFFERLEN );
	
	return err;	
}

//------------------------------------------------------------------------------
//this checks the socket for data
int CXServer::CheckForData(int handle, int handletype, int freeslot)
{
	int bufflen = 0;
	int buff = 0;
	int err = 0;
	char c;

	assert( iStatus == RECEIVE_NAME);

	buff = freeslot;

	//check for data on the socket
	if( CheckForTimeout(handle) != 0 )
	{
		//revert status to previous as we received no data within the alloted time
		iStatus = RECEIVE_HELLO;
		Write(handle, handletype, REPLY_TIMEOUT, strlen(REPLY_TIMEOUT) );
		return err;
	}

	// the client should send the list of arguments - this is the name and value receiving section!!!!
	// this section will continue to loop until the the goodbye message has been received
	while( iStatus != RECEIVE_GOODBYE ) 
	{
		//need to do the timeout thing here for value data
		if( iStatus == RECEIVE_VALUE )
		{
			if( CheckForTimeout(handle) != 0 )
			{
				//revert status to previous as we received no data within the alloted time
				iStatus = RECEIVE_NAME;
				//point to the previous name element in the array so that the last bit of data is overwritten
				iListPtr = iList[buff].iName;
				bufflen = 0;
				Write( handle, handletype, REPLY_TIMEOUT, strlen(REPLY_TIMEOUT) );
				return err;
			}
		}

		// receive the next byte
		err = Read( handle, handletype, &c );

		//if something has been read from the socket
		if( ( err == 1 ) )
		{
			//need to check for available data space
			if( buff > (MAXLISTLENGTH - 1) ) 
			{
				Write(handle, handletype, REPLY_ERROR, strlen(REPLY_ERROR));
				return 	CSP_LISTFULL;
			}

			// if the data buffer is full then we have an error
			if( bufflen == (MAXBUFFERLEN - 1) ) 
			{
				Write(handle, handletype, REPLY_ERROR, strlen(REPLY_ERROR));
				return 	CSP_BUFFERFULL;
			}
			
			//...or the character received is zero		
			if( strcmp( &c,"" ) == 0)
			{
				//reply to say that we got the data ok
				Write( handle, handletype, iListPtr, strlen(iListPtr) );
				Write( handle, HT_SOCKET, "", 1 );
			
				CheckForGoodbye(handle, handletype, buff);
				
				//if GOODBYE has not been received and the status has not changed
				if( iStatus != RECEIVE_GOODBYE )
				{
					//change the status from name -> value or vice versa
					if( iListPtr == iList[buff].iName )
					{
						iListPtr = iList[buff].iValue;
						//reset bufferlen counter so that writing can start from the beginning

						bufflen = 0;

						iStatus = RECEIVE_VALUE;
					}
					else
					{	
						//move onto the next element in the list
						iListPtr = iList[++buff].iName;
						
						//reset bufferlen counter so that writing can start from the beginning
						bufflen = 0;
						
						iStatus = RECEIVE_NAME;
					}		//else
				}			//if (iStatus != ...)
			}				//if (c == 0)
			else
			{
				// add the byte to the buffer
				iListPtr[bufflen++] = c;
			}
		}				//if
		else
		{
			//if there was a dodgy read
			Write(handle, handletype, REPLY_ERROR, strlen(REPLY_ERROR));
			memset( iListPtr, 0, MAXBUFFERLEN );
			
			//reset bufferlen counter so that writing can start from the beginning next time
			bufflen = 0;
			
			//and if status was VALUE then go back to NAME
			if(iStatus = RECEIVE_VALUE)
			{
				iStatus = RECEIVE_NAME;
				iListPtr = iList[buff].iName;
				return err;
			}
		}
	}

	//change status back to receive hello
	iStatus = RECEIVE_HELLO;

	//everthing should be ok at this stage so reset err
	err = 0;

	return err;
}

//------------------------------------------------------------------------------
//this function checks for the GOODBYE message.  If it has been set then it changes the status
void CXServer::CheckForGoodbye(int handle, int handletype, int latestposition)
{
	
	sprintf(iGoodbyebuffer, "%s", "GOODBYE");

	//if an exact match is found
	if(strcmp(iGoodbyebuffer, iListPtr) == 0)
	{
		//change status
		iStatus = RECEIVE_GOODBYE;

		//reset the data in the name/value slot
		memset( iListPtr, 0, MAXBUFFERLEN );

		printf( "Expecting HELLO from client side...\n" );
	}
}
//------------------------------------------------------------------------------
//this function reads data from a specific port
int CXServer::Read(int handle, int handletype, char* c)
{
	int err = 0;

	assert( c != NULL );
	
	// socket handles
	if( handletype == HT_SOCKET ) 
	{
		err = recv( handle, c, 1, 0 );
		if( err != 1 ) //if the single char has not been received
		{
			err = GetLastError();
			return err;
		}
	}

	return err;		
}

//------------------------------------------------------------------------------
//this function writes on a specific port
int CXServer::Write(int handle, int handletype, char *buff, int bufflen)
{
	int err;
	
	assert( buff != NULL );

	// socket handles
	if( handletype == HT_SOCKET ) 
	{
		err = send( handle, buff, bufflen, 0 );
		if( err != bufflen ) 
		{
			err = GetLastError();
			return err;
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
//this function waits for input to be received on the specified socket for a specific time
int CXServer::CheckForTimeout(int handle)
{
	int live = 0;
	int err = 0;
	
	while( 1 )
	{
		//set up socket timer structure
		FD_ZERO( &iReadSocketSet );
		FD_SET( (unsigned)handle, &iReadSocketSet );

		//wait for some input
		live = select(NULL, &iReadSocketSet, NULL, NULL, &timeout);

		//if there is some input
		if( live < 0 )
		{
			printf( "Call to select() failed...\n" );
			return live;
		}
		else if ( live == 0 )	
		{
			printf( "No descriptors ready...\n" );
			printf( "Timed out...\n" );
			err = -1;
			return err;
		}
		else	//something is ready
		{
			FD_ISSET( handle, &iReadSocketSet );
			printf( "Data read...\n" );
			return err;
		}
	}

	return err;
}
//------------------------------------------------------------------------------

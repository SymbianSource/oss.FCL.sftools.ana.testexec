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
* Filename: CXClient.cpp
* Author: Sanjeet Matharu 
* This is the client side of the protocol which takes name/value pairs and does stuff
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
#include "cxclient.h"

#define MAXRETRIES		5

//------------------------------------------------------------------------------
//constructor
CXClient::CXClient()
{
	iClientStatus = SENDING_HELLO;

	sprintf(hellobuffer, "%s", "HELLO");
	sprintf(goodbyebuffer, "%s", "GOODBYE");
}

//------------------------------------------------------------------------------
//destructor
CXClient::~CXClient()
{
	
}

//------------------------------------------------------------------------------
//this function adds the specified value into the list of name value pairs
int CXClient::OnExecute(int handle, char* cValue, int dim1, int dim2)
{
	int error = 0;
	int localsocket = handle;
	int counter = 0;

	if(localsocket != 0)
	{
		error = SendData(localsocket, hellobuffer);
		//send the hello to the server side first
		if( error != OK_DATATRANSFER)
		{
			//give up and return a message
			printf("There was a problem saying hello...\n");
			return -1;
		}
	}
	else
	{
		printf("There was a problem with the socket...\n");
		return -1;
	}

	//go through the valuebuffer and send the data
	for(counter = 0; counter < dim1; counter++)
	{
		error = SendData(localsocket, &cValue[counter*dim2]);
		if( error != OK_DATATRANSFER)
		{
			//give up and return a message
			printf("There was a problem sending the data...\n");
			return -1;
		}
	}

	//send the goodbye to the server side 
	if(localsocket != 0)
	{
		error = SendData(localsocket, goodbyebuffer);
		if( error != OK_DATATRANSFER )
		{
			//give up and return a message
			printf("There was a problem when saying goodbye...\n");
			return -1;
		}
	}
	else
	{
		printf("There was a problem with the socket...\n");
		return -1;
	}

	return 0;
}

//------------------------------------------------------------------------------
//this sends the hello command to the client over the specified socket to initiate comms
int CXClient::SendData(int handle, char* aValue)
{
	char c;		//this is the buffer that is used to receive the data from across the link
	char receivebuffer[MAXBUFFERLEN];

	int counter = 0;
	int bufferlength = 0;
	int sendcounter = 0;
	int err = -1;
	
	bool success = false;
	memset(receivebuffer, 0, sizeof(receivebuffer));		//initialise

	bufferlength = strlen(aValue);

	//send the data
	while( sendcounter < MAXRETRIES ) 
	{
		//write to the socket
		err = WriteClient( handle, HT_SOCKET, aValue, bufferlength );

		//if unable to send for some reason (verify that the error code is the length of one char)
		if( err != bufferlength )
		{
			printf("Data %s sending failed %d...\n", aValue, err);
			sendcounter++;
		}
		else	//print ok 
		{
			printf("Data %s sending ok %d chars sent...\n", aValue, bufferlength);
			sendcounter = MAXRETRIES;
			//send an end of string character so the server can evaluate what has been received
			WriteClient( handle, HT_SOCKET, "", 1 );
		}
	}

	//receive the reply ONLY if send has succeeded (hence inside the maxretries limit)
	if( ( sendcounter <= MAXRETRIES ) )
	{
		while( 1 )
		{
			err = ReadClient( handle, HT_SOCKET, &c );

			if( strcmp( &c, "" ) == 0)
			{
				//if there was a problem with the read (i.e. invalid command received from server, knackered link)
				if( ( err <= 0 ) || ( strcmp(receivebuffer, aValue) != 0 ) )
				{
					printf("Data read failed ( device returned %s and receive code was %d )...\n", receivebuffer, err);
					memset(receivebuffer, 0, sizeof(receivebuffer));		//reset
					return ERROR_DATATRANSFER;
				}
				else
				{
					printf("Data read succeeded ( device returned %s )...\n", receivebuffer);
					memset(receivebuffer, 0, sizeof(receivebuffer));		//reset
					counter = 0;	//reset
					break;
				}
			}	//if

			receivebuffer[counter++] = c;
		}		//while
	}
	
	return OK_DATATRANSFER;
}
//------------------------------------------------------------------------------
int CXClient::ReadClient(int handle, int handletype, char* c)
{
	int err;
	
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

	// success
	return err;
}

//------------------------------------------------------------------------------
int CXClient::WriteClient(int handle, int handletype, char *buff, int bufflen )
{
	int err = 0;
	unsigned long bytes_written = 0;

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

	return err;	
}

//------------------------------------------------------------------------------

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
* Filename: CXClient.h
* Author: Sanjeet Matharu 
* This is the client side of the protocol which sneds name/value pairs and does stuff
*
*/



#ifndef __CXCLIENT_H__
#define __CXCLIENT_H__

#define HT_SOCKET			0
#define HT_WIN32FILEHANDLE	1
#define HT_LINUXFILEHANDLE	2

#define OK_DATATRANSFER		10
#define ERROR_DATATRANSFER	20

#define REPLY_OK			"OK"
#define	REPLY_ERROR			"ERROR"
#define REPLY_TIMEOUT		"TIMEOUT"

#define MAXBUFFERLEN		50

//Types
typedef enum 
{
	SENDING_HELLO,
	SENDING_GOODBYE,
	SENDING_DATA,
} TClientStatus;

//----------------------------------------------------------------------------------
//generic server class
class CXClient
{
	public:

		//constructor/destructor
		CXClient();
		~CXClient();

		int OnExecute(int socket, char* cValue, int dim1, int dim2);

	private:

		//functions
		int SendHello(int handle);
		int SendGoodbye(int handle);
		int SendData(int handle, char* aValue);

		int ReadClient(int handle, int handletype, char* c);
		int WriteClient(int handle, int handletype, char *buff, int bufflen);
	
		//data
		TClientStatus iClientStatus;
		char hellobuffer[6];
		char goodbyebuffer[8];
};

//----------------------------------------------------------------------------------

#endif __CXCLIENT_H__

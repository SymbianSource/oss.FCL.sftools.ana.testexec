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
* Filename: CXServer.h
* Author: Sanjeet Matharu 
* This is the server side of the protocol which takes name/value pairs and does stuff
*
*/



#ifndef __CXSERVER_H__
#define __CXSERVER_H__

#define MAXLISTLENGTH		100	//this is the name/value pair max list length
#define MAXBUFFERLEN		64	//maximum length of the name and value strings

#define HT_SOCKET			0
#define HT_WIN32FILEHANDLE	1
#define HT_LINUXFILEHANDLE	2

#define REPLY_OK			"OK"
#define	REPLY_ERROR			"ERROR"
#define REPLY_TIMEOUT		"TIMEOUT"

#define CSP_LISTFULL		2001
#define CSP_BUFFERFULL		2002

//Types
typedef enum 
{
	RECEIVE_HELLO,
	RECEIVE_NAME,
	RECEIVE_VALUE,
	RECEIVE_GOODBYE
} TReceiveStatus;

//struct required for the name/value pairs
typedef struct 
{
	char iName[MAXBUFFERLEN];
	char iValue[MAXBUFFERLEN];
} TNameValuePair;

//----------------------------------------------------------------------------------
//generic server class
class CXServer
{
	public:

		//constructor/destructor
		CXServer();
		~CXServer();

		//functions
		int OnExecute(int handle, int handletype);

		//data
		TReceiveStatus iStatus;
		TNameValuePair iList[MAXLISTLENGTH];
		
	private:

		//functions
		int CheckForHello(int handle, int handletype, int freeslot);
		int CheckForData(int handle, int handletype, int freeslot);
		void CheckForGoodbye(int handle, int handletype, int latestposition);
		int CheckForTimeout(int handle);

		int Read(int handle, int handletype, char* c);
		int Write(int handle, int handletype, char *buff, int bufflen );
		
		//data
		char* iListPtr;

		char iGoodbyebuffer[8];
		char iHellobuffer[6];

		fd_set iReadSocketSet;
		struct timeval timeout;
};

//----------------------------------------------------------------------------------

#endif __CXSERVER_H__


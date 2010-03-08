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
*
*/



#ifndef __SYNCTREE_H__
#define __SYNCTREE_H__

/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <time.h>


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define ERR_OPENFILEFAILED		-100
#define ERR_RECVFAILED			-101
#define ERR_SENDHEADERFAILED	-102
#define ERR_SENDFILENAMEFAILED	-103
#define ERR_FILEREJECTED		-104
#define ERR_CREATESOCKETFAILED	-105
#define ERR_STATFAILED			-106
#define ERR_RECEIVEHEADERFAILED	-107
#define ERR_SENDFAILED			-108
#define ERR_OPENPORTFAILED		-109

/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef enum {
	FTC_REQUESTSEND = 1,
	FTC_ALLOWSEND = 2,
	FTC_REJECTSEND = 3,
	FTC_EXECUTESCRIPT = 4
} TFileTransferCommand;

typedef struct {
	TFileTransferCommand iCommand;
	int iFileLength;
	time_t iTimeLastModified;
	int iFilenameLength;
} TFileTransferHeader;


/*******************************************************************************
 *
 * SyncFile
 *
 ******************************************************************************/
int SyncFile( char *portname, char *filename, int *errcode );
int RecvSyncFile( char *portname, int *errcode );
int RunScript( char *portname, char *filename, int *errcode );

#endif //__SYNCTREE_H__

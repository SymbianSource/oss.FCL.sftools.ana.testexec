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
#include "mobster_client_server_protocol.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAXOPTIONLENGTH		64
#define IDTAG				"ID="


/*******************************************************************************
 *
 * Prototypes
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * client_server_protocol_socket -- waits for a ID=xx string from the client and 
 * replies with a SERVER(xx) string.
 *
 ******************************************************************************/
int client_server_protocol( int aSock )
{
	char c;
	int len, err, phoneid, bufflen = 0;
	char buff[MAXOPTIONLENGTH];

	// init buffer
	memset( buff, 0, MAXOPTIONLENGTH );
	
	// the client should send it's ID
	while( 1 ) {
		
		// receive the next byte
		err = recv( aSock, &c, 1, 0 );
		if( err <= 0 ) {
			return -1;
		}

		// if the buffer is full then we have an error
		if( bufflen == (MAXOPTIONLENGTH-1) ) {
			return -1;
		}

		// add the byte to the buffer
		buff[bufflen++] = c;

		// if this byte is a 0x0d then we have the complete string
		if( c == 0xd ) {
			break;
		}
	}

	// make sure this is the ID string
	err = strncmp( buff, IDTAG, strlen(IDTAG) );
	if( err != 0 ) {
		return -1;
	}

	// now extract the ID
	phoneid = atoi( &(buff[3]) );

#ifdef SYSTEM_TEST_MP4
	assert( !"Test" );
#endif

	// send the reply string -- blocking socket should send all data before returning
	sprintf( buff, "SERVER(%d)", phoneid );
	len = strlen(buff);
	err = send( aSock, buff, len, 0 );
	if( err != len ) {
		return -1;
	}

	// done
	return phoneid;
}


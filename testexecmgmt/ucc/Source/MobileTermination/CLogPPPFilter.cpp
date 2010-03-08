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
* CLOGPPPFILTER
* Prints out the details of all PPP frames that pass through the MT.
* System Includes
*
*/



#include <stdlib.h>
#include <assert.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "CLogPPPFilter.h"


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAX_LCP_MESSAGE_ID		11


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/
#define EXTRACT_SHORT(ptr)     (htons((*((short*)(ptr)))&0x0000FFFF))
#define EXTRACT_CHAR(ptr)      ((0x000000FF)&(ptr))


/*******************************************************************************
 *
 * Static vars
 *
 ******************************************************************************/
static char *lcp_message_desc[] = { "(invalid)", "Configure-Request", "Configure-Ack", "Configure-Nack",
									"Configure-Reject", "Terminate-Request", "Terminate-Ack", "Code-Reject", 
									"Protocol-Reject", "Echo-Request", "Echo-Reply", "Discard-Request", NULL };

/*******************************************************************************
 *
 * PUBLIC METHOD: Constructor
 *
 ******************************************************************************/
CLogPppFilter::CLogPppFilter( TPhoneData *aPhoneData, CLog *aLog ) : iOutgoingFrame("PPP-Log::OutgoingFrame",1), iIncomingFrame("PPP-Log::IncomingFrame",1)
{
	// check params
	assert( aPhoneData != NULL );
	assert( aLog != NULL );

	//set state
	iPhoneData = aPhoneData;
	iLog = aLog;
	iIncomingFrameOverflowFlag = 0;
	iOutgoingFrameOverflowFlag = 0;
}


/*******************************************************************************
 *
 * PUBLIC METHOD: Destructor
 *
 ******************************************************************************/
CLogPppFilter::~CLogPppFilter()
{
}


/*******************************************************************************
 *
 * PUBLIC METHOD: IFilter Interface Methods
 *
 ******************************************************************************/

int CLogPppFilter::ProcessIncomingData( char *data, int len )
{
	int i;
	for( i = 0; i < len; i++ ) {
		ProcessByte( &iIncomingFrame, data[i], &iIncomingFrameOverflowFlag );
	}
	return 0;
}


int CLogPppFilter::ProcessOutgoingData( char *data, int len )
{
	int i;
	for( i = 0; i < len; i++ ) {
		ProcessByte( &iOutgoingFrame, data[i], &iOutgoingFrameOverflowFlag );
	}
	return 0;
}


void CLogPppFilter::ProcessByte( CPppFrame *aFrame, char c, int *iOverflowFlag )
{
	char *framedesc;
	TFrameError ferr;
	TFrameStatus fstatus;

	// add the byte to the frame buffer
	ferr = aFrame->AddByteToFrame( c );

	// if this causes an overflow then log the warning and return
	if( ferr == FE_OVERFLOW ) {
		if( *iOverflowFlag == 0 ) {
			iLog->WriteLogEntry( SV_WARNING, aFrame->GetFrameName(), "Frame overflow." );
			*iOverflowFlag = 1;
		}
		return;
	}
	assert( ferr == FE_NONE );

	// if the frame isn't completed then return
	fstatus = aFrame->GetFrameStatus();
	if( fstatus != FS_COMPLETE ) {
		return;
	}

	// otherwise print a record about the frame and then clear it
	framedesc = CreateFrameDescription( aFrame );
	iLog->WriteLogEntry( SV_INFO, aFrame->GetFrameName(), framedesc );
	aFrame->ClearFrame();
	*iOverflowFlag = 0;
}


/*******************************************************************************
 *
 * PRIVATE METHOD: Create FrameDescription
 *
 * Protocol [Message Type] (Frame Size)
 *   IPCP (21)
 *   LCP Echo-Request (48)
 *
 ******************************************************************************/
char *CLogPppFilter::CreateFrameDescription( CPppFrame *aFrame )
{
	char *frame_ptr;
	int frame_size;
	int protocol_id;
	char *bptr;

	// get the frame
	frame_ptr = aFrame->GetFrameBuffer( &frame_size );

	// clear the description buffer
	iFrameDescription[0] = 0;

	// now insert the protocol and message if appropriate
	protocol_id = AddProtocolName( frame_ptr, frame_size );

	// add the length
	bptr = &(iFrameDescription[strlen(iFrameDescription)]);
	sprintf( bptr, "(%d).", frame_size );

	// done
	return iFrameDescription;
}


/*****************************************************************************************
 *
 * PRIVATE METHOD: AddProtocolName 
 *
 ****************************************************************************************/
int CLogPppFilter::AddProtocolName( char *aFrameBuffer, int aFrameSize )
{
	int protocol_id;
	int datapos = 0;
	char *ptr;
	int message_type;

	// find the position of the protocol field (packet format can change a fair bit)
	if( aFrameBuffer[0] == 0x7E ) {
		datapos++;
	    if( aFrameBuffer[1] == ((char)0x000000FF) ) {
			datapos++;
			if( aFrameBuffer[2] == 0x03 ) {
				datapos++;
			}
		}
	} 

	// get the protocol id and the target pointer
	protocol_id = EXTRACT_SHORT(&aFrameBuffer[datapos]);
	ptr = &(iFrameDescription[strlen(iFrameDescription)]);
	message_type = EXTRACT_CHAR(aFrameBuffer[datapos+2]);

	// now print the protocol name
	switch( protocol_id ) {
	case 0xc021:
		sprintf( ptr, "LCP " );
		AddLcpMessageTypeString( message_type );
		break;

	case 0xc023:
		sprintf( ptr, "PAP " );
	    break;

	case 0x8021:
		sprintf( ptr, "IPCP " );
	    break;

	case 0x80FD:
		sprintf( ptr, "CCP " );
	    break;

	default:
		sprintf( ptr, "0x%X ", protocol_id );
		protocol_id = 0;
	    break;
	}

	// return the protocol id
	return protocol_id;
}


/*****************************************************************************************
 *
 * PRIVATE METHOD: AddLcpMessageTypeString
 *
 ****************************************************************************************/   
void CLogPppFilter::AddLcpMessageTypeString( int aMessageType )
{
	char *bptr;

	// get the ptr to the output buffer
	bptr = &(iFrameDescription[strlen(iFrameDescription)]);

	// make sure the message id is known
	if( (aMessageType <= 0) || (aMessageType >=  MAX_LCP_MESSAGE_ID) ) {
		sprintf( bptr, "0x%X ", aMessageType );
		return;
	}

	// print the name of the 
	sprintf( bptr, lcp_message_desc[aMessageType] );
}


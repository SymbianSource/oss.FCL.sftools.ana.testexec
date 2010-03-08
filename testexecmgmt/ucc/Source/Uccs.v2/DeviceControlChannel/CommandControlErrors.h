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
* CommandControlErrors
*
*/



#ifndef __COMMANDCONTROLERRORS_H__
#define __COMMANDCONTROLERRORS_H__


/*****************************************************************************
 *
 *  Types
 *
 ****************************************************************************/
typedef enum {
	EAC_SUCCESS,		
	EAC_ERRINITTRANSPORT,
	EAC_INVALIDPORT,
	EAC_FAILEDTOOPENPORT,
	EAC_INVALIDTRANSPORT,
	EAC_RECEIVEBYTESERROR,
	EAC_PROCESSINGFAILED,
	EAC_STARTUSECASEFAILED,
	EAC_RENDEZVOUSFAILED,
	EAC_ENDUSECASEFAILED,
	EAC_SCRIPTFAILED,
	EAC_QUIT
} TCommandControlError;


/*****************************************************************************
 *
 * Prototypes
 *
 ****************************************************************************/
char *GetCommandControlErrorString( TCommandControlError aErrorCode );

#endif

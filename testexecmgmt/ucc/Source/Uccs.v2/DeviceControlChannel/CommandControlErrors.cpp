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
* LocalIncludes
*
*/



#include "CommandControlErrors.h"


/*****************************************************************************
 *
 * PUBLIC FUNCTION: GetCommandControlErrorString
 *
 ****************************************************************************/
char *GetCommandControlErrorString( TCommandControlError aErrorCode )
{
	switch( aErrorCode ) {

	case EAC_SUCCESS:
		return "Success";

	case EAC_ERRINITTRANSPORT: 
		return "Error initialising the transport";

	case EAC_INVALIDPORT:
		return "Invalid port string";

	case EAC_FAILEDTOOPENPORT:
		return "Failed to open the port";

	case EAC_INVALIDTRANSPORT:
		return "Invalid transport requested";

	case EAC_RECEIVEBYTESERROR:
		return "Error while trying to receive data";

	case EAC_PROCESSINGFAILED:
		return "Error while trying send reply";

	case EAC_STARTUSECASEFAILED:
		return "StartUsecase failed";

	case EAC_RENDEZVOUSFAILED:
		return "Rendezvous failed";

	case EAC_ENDUSECASEFAILED:
		return "EndUsecase failed";

	case EAC_SCRIPTFAILED:
		return "Script failed";

	default:
		return "Unknown error";
	}
}

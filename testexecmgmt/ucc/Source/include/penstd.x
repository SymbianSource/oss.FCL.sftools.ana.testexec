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
* Constants - STANDARD SET
*
*/


/* success -- returned by client stubs and return values */
const ERR_NONE									= 0;

/* invalid return code used as a place holder */
const ERR_INVALID_ERROR_CODE					= -16;

/* service stub implementation (returned in RV - i.e. as the result) */
const ERR_SERVICE_ALREADY_STARTED				= -1;
const ERR_SERVICE_NOT_RUNNING					= -2;
const ERR_ACTIVE_USER_SESSIONS					= -3;
const ERR_FAILED_TO_REMOVE_ACTIVE_SESSIONS		= -4;
const ERR_FAILED_TO_CREATE_COMPONENT_MANAGER	= -5;
const ERR_FAILED_TO_RETRIEVE_KEY				= -6;
const ERR_INSTANCE_DOES_NOT_EXIST				= -7;

/* component manager (returned in RV - i.e. as the result ) */
const ERR_CANNOT_CREATE_NEW_INSTANCE			= -9;

/* set by the stub glue code as the completion code (i.e. as the RCC) */
const ERR_MISSING_PARAMETER						= -17;
const ERR_INVALID_METHOD						= -8;

/* client stubs return one of these values or ERR_NONE (i.e. as the RCC) */
const ERR_STUB_ALREADY_CONNECTED				= -10;
const ERR_FAILED_TO_CONNECT						= -11;
const ERR_STUB_NOT_CONNECTED					= -12;
const ERR_INVALID_RV_POINTER					= -13;
const ERR_RPC_ERROR								= -14;
const ERR_INVALID_SERVICE						= -15;
const ERR_INVALID_CALL							= -18;

/* standard error values returned from methods (RV) */
const ERR_INVALIDARG							= -50;
const ERR_INVALIDSTATE							= -51;
const ERR_GENERAL								= -52;

/* info - required for the framework */
const INFO_MAXIMUM_OBJECTS						= 256;


/*******************************************************************************
 *
 * Types
 *
 ******************************************************************************/
typedef int TComponentList<>;

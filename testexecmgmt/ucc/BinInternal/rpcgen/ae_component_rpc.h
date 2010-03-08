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



#ifndef __AE_COMPONENT_RPC_H__
#define __AE_COMPONENT_RPC_H__

/*********************************************************************
 *
 * Definitions
 *
 ********************************************************************/
#define TYPE_SVC_STUB_IMPL	0
#define TYPE_SVC_MGR_H		1
#define TYPE_SVC_MGR_C		2
#define TYPE_INST_H			3
#define TYPE_INST_C			4
#define TYPE_CLIENT_H		5
#define TYPE_CLIENT_C		6

#define MAXEXTENDEDNAME		256

/*********************************************************************
 *
 * Prototypes
 *
 ********************************************************************/
void ae_output_rpc_file( char *infile, char *outfile, int type );
void ae_set_trace( int type );
char *ae_extend_custom_types( char *base_type );

#endif //__AE_COMPONENT_RPC_H__

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
* standard_service_manager
*
*/




#ifndef __STANDARD_SERVICE_MANAGER_H__
#define __STANDARD_SERVICE_MANAGER_H__


/****************************************************************************************
 * 
 * Macro Functions
 * 
 ***************************************************************************************/
#define SET_ERROR_CODE(type_name,member_name,ptr_to_result)    { \
                                                                 (ptr_to_result) = (type_name*)aArgs;                    \
                                                                 memset( ptr_to_result, 0, sizeof(type_name) );        \
                                                                 (ptr_to_result)->member_name = aError;                  \
                                                               }

#define SET_LIST_ERROR_CODE(type_name,ptr_to_result)           { \
                                                                 assert( aError != ERR_NONE );                         \
                                                                 ptr_to_result = (type_name*)aArgs;                    \
                                                                 if( ptr_to_result->type_name##_len != 0 ) {           \
                                                                   free( ptr_to_result->type_name##_val );             \
                                                                   ptr_to_result->type_name##_val = NULL;              \
                                                                   ptr_to_result->type_name##_len = 0;                 \
                                                                 }                                                     \
							       }			

/****************************************************************************************
 * 
 * Done
 * 
 ***************************************************************************************/
#endif 

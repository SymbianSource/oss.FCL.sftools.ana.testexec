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
* This file contains definitions to allow windows things to compile on Linux. This is
* nothing major -- just (void*) for HANDLE etc.
*
*/




#ifndef __LINUX_CONV_H__
#define __LINUX_CONV_H__

/****************************************************************************************
 *
 * Switches
 *
 ***************************************************************************************/
#ifdef WIN32
#error
#endif

/****************************************************************************************
 *
 * Definitions
 *
 ***************************************************************************************/
#define HANDLE (void*)

/****************************************************************************************
 *
 * Macro Functions
 *
 ***************************************************************************************/
#define ReadFile(a,b,c,d,e)  (assert(0),1)
#define WriteFile(a,b,c,d,e) (assert(0),1)


#endif //__LINUX_CONV_H__

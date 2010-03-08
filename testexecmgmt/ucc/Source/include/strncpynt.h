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




#ifndef __STRNCPYNT_H__
#define __STRNCPYNT_H__

#define STRNCPY_NULL_TERMINATE( dest, src, dest_size )		{ strncpy((dest),(src),(dest_size)-1); (dest)[(dest_size)-1] = 0; }

#endif //__STRNCPYNT_H__

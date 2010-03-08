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



#ifndef __CCOMPONENTMANAGER_H__
#define __CCOMPONENTMANAGER_H__

/****************************************************************************************
 * 
 * System Includes
 * 
 ***************************************************************************************/
#include <stdio.h>
#include <assert.h>

/****************************************************************************************
 * 
 * Local Includes
 * 
 ***************************************************************************************/

/****************************************************************************************
 * 
 * Class CComponentManager
 * 
 ***************************************************************************************/
template <class T>
class CComponentManager
{
public:
	CComponentManager( int aMaxComponents )
	{
		// check params
		assert( aMaxComponents > 0 );

		// set the state
		iComponentCount = 0;
		iMaxComponents = aMaxComponents;
		iComponentList = (T**)calloc( iMaxComponents, sizeof(T*) );
		assert( iComponentList != NULL );
	}

	~CComponentManager()
	{
		assert( iComponentCount == 0 );
		free( iComponentList );
	}

	int CreateInstance() 
	{
		int i;

		// make sure there is room
		if( iComponentCount == iMaxComponents ) {
			return ERR_CANNOT_CREATE_NEW_INSTANCE;
		}

		// find a free index
		for( i = 0; i < iMaxComponents; i++ ) {
			if( iComponentList[i] == NULL )
				break;
		}
		assert( i < iMaxComponents );

		// create the new instance
		iComponentList[i] = new T();
		assert( iComponentList[i] != NULL );

		// set the key
		(iComponentList[i])->SetKey( i );

		// update the count
		iComponentCount++;

		// done - return the key
		return i;
	}

	int DeleteInstance( int aInstanceKey )
	{
		int is_key_valid;

		// check that the key is valid
		is_key_valid = IsValidKey( aInstanceKey );
		assert( is_key_valid != 0 );

		// verify the key
		assert( (iComponentList[aInstanceKey])->GetKey() == aInstanceKey );

		// delete the instace
		delete (iComponentList[aInstanceKey]);
		iComponentList[aInstanceKey] = NULL;
		iComponentCount--;

		// done
		return ERR_NONE;
	}

	int IsValidKey( int aInstanceKey )
	{
		return ((aInstanceKey >= 0) && (aInstanceKey < iMaxComponents) && (iComponentList[aInstanceKey] != NULL) );
	}

	T *GetInstance( int aInstanceKey )
	{
		// validate the param
		if( (aInstanceKey < 0) || (aInstanceKey >= iMaxComponents) ) {
			return NULL;
		}
		return iComponentList[aInstanceKey];
	}

	int GetInstanceCount()
	{
		return iComponentCount;
	}

	int DeleteAllInstances()
	{
		int i, err;
		for( i = 0; i < iMaxComponents; i++ ) {
			if( iComponentList[i] != NULL ) {
				err = DeleteInstance( i );
				assert( err == ERR_NONE );
			}
		}
		assert( iComponentCount == 0 );
		return ERR_NONE;
	}


private:
	int iComponentCount;
	int iMaxComponents;
	T **iComponentList;
};

#endif //__CCOMPONENTMANAGER_H__

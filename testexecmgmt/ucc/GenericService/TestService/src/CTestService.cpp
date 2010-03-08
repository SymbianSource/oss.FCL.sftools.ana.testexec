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
* CTestService.h
*
*/



#include <stdio.h>

#include "CTestService.h"

CService* Service() { return new CTestService(); }

int CTestService::RunCommand( const CCall& aCall )
{
	int callID = -1;
	if( aCall.CallID( callID ) )
	{
		switch( callID )
		{
			case 1:
				{
					printf( "CallID One has been made\n" );
					// Parse the parameters here and make the relevant call
				}
				break;
			case 2:
				{
					printf( "CallID Two has been made\n" );
					// Parse the parameters here and make the relevant call
				}
				break;
			case 3:
				{
					printf( "CallID Three has been made\n" );
					// Parse the parameters here and make the relevant call
				}
				break;
			default:
				{
					printf( "Unknown CallID\n" );
					return ERR_INVALID_CALL;
				}
		}

		// *** Testing ***
		// Echo the number of parameters
		int numParams = 0;
		if( aCall.Params( numParams ) )
		{
			printf( "Number of parameters: %d\n", numParams );

			for( int index=0 ; index<numParams ; index++ )
			{
				string name;
				if( aCall.Name(index, name) )
				{
					// Echo the Parameter Name
					printf( "Parameter Name: %s\n", name.c_str() );

					string value;
					if( aCall.Value(index, value) )
					{
						// Echo the Parameter Value
						printf( "Parameter Value: %s\n", value.c_str() );
					}
				}
			}
			printf("\n");
			// *** Testing ***
		}
	}

	return 0;
}

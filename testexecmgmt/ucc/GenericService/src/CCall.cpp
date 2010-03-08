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



#include "CCall.h"
#include <stdio.h>

/*
 *	Constructor
 *
 *    Takes in a TCall structure passed in via the RPC framework
 */
CCall::CCall( const TCall& aCall )
	: iCall(aCall)
{
}

/*
 *	Destructor
 */
CCall::~CCall()
{
}

/*
 *	CallID
 *
 *    Retrieve the call id
 */
bool CCall::CallID( int& aCallID ) const
{
	bool ret = false;
	if( iCall.iCallID >= 0 )
	{
		aCallID = iCall.iCallID;
		ret = true;
	}
	return ret;
}

/*
 *	Params
 *
 *    Retrieve the number of parameters in the call
 */
bool CCall::Params( int& aNumParams) const
{
	bool ret = false;
	if( iCall.iParams.iParams_len >= 0 )
	{
		aNumParams = iCall.iParams.iParams_len;
		ret = true;
	}
	return ret;
}

/*
 *	Name
 *
 *    Retrieve the name of a parameter at a specific index
 */
bool CCall::Name( int anIndex, string& aName ) const
{
	bool ret = false;
	if( anIndex >= 0 && anIndex < iCall.iParams.iParams_len )
	{
		if( Check(iCall.iParams.iParams_val[anIndex].iName, MAXPARAMNAMELENGTH) )
		{
			aName.assign( iCall.iParams.iParams_val[anIndex].iName );
			ret = true;
		}
	}
	return ret;
}

/*
 *	Value
 *
 *    Retrieve the value of a parameter at a specific index
 */
bool CCall::Value( int anIndex, string& aValue ) const
{
	bool ret = false;
	if( anIndex >= 0 && anIndex < iCall.iParams.iParams_len )
	{
		if( Check(iCall.iParams.iParams_val[anIndex].iValue, MAXPARAMVALUELENGTH) )
		{
			aValue.assign( iCall.iParams.iParams_val[anIndex].iValue );
			ret = true;
		}
	}
	return ret;
}

/*
 *	Check
 *
 *    Check a string value is within the defined boundaries
 */
bool CCall::Check( const string& aValue, const unsigned int aMaxLength ) const
{
	bool ret = false;
	if( aValue.size() > 0 && aValue.size() <= aMaxLength )
	{
		ret = true;
	}
	return ret;
}

/*
 *	Get
 *
 *    Find the index into the array matching the specified parameter name
 */
int	CCall::GetIndex(const string &aName) const
{
	// search through each param to find a matching (case insensitive) name & return the index
	for (unsigned int i =0; i < iCall.iParams.iParams_len; i++)
	{
		if (!_stricmp(aName.c_str(), iCall.iParams.iParams_val[i].iName))
			return i;
	}
	return -1;
}

/*
 *	Get
 *
 *    Retrieve the value of the specified parameter
 */
bool CCall::Get(const string &aName, string &aValue) const
{
	// find a matching index & copy value
	int index = GetIndex(aName);
	if (index < 0)
		return false;

	aValue.assign(iCall.iParams.iParams_val[index].iValue);
	return true;
}

bool CCall::Get(const string &aName, int &aValue) const
{
	// get value as string
	string value;
	if (!Get(aName, value))
		return false;

	// convert value to int
	if (sscanf(value.c_str(), "0x%x", &aValue) != 1 &&
		sscanf(value.c_str(), "%d", &aValue) != 1)
	{
		return false;
	}

	return true;
}

bool CCall::Get(const string &aName, bool &aValue) const
{
	// get value as string
	string value;
	if (!Get(aName, value))
		return false;

	// convert value to bool
	if (!_stricmp(value.c_str(), "true"))
		return true;
	else if (!_stricmp(value.c_str(), "false"))
		return false;

	aValue = false;
	return false;
}

/*
 *	Dump
 *
 *    Dump the contents to stdout (useful debugging aid)
 *    - Deliberately uses the public APIs
 */
void CCall::Dump() const
{
	// call id
	int callId = 0;
	if (CallID(callId))
		printf("callId = %d\n", callId);
	else
	{
		printf("Error! callId is indeterminate\n");
		return;
	}

	// num params
	int numParams = 0;
	if (Params(numParams))
		printf("numParams = %d\n", numParams);
	else
	{
		printf("Error! numParams is indeterminate\n");
		return;
	}

	// params
	for (int i=0 ; i < numParams ; i++)
	{
		// param name
		string paramName;
		if (Name(i, paramName))
			printf("  arg; \"%s\" = ", paramName.c_str());
		else
			printf("Error! paramName is indeterminate\n");

		// param value
		string paramValue;
		if (Value(i, paramValue))
			printf("\"%s\"\n", paramValue.c_str());
		else
			printf("\nError! paramValue is indeterminate\n");
	}
}


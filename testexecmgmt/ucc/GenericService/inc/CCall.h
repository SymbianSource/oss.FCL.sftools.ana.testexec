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



#ifndef __CCALL_H__
#define __CCALL_H__

#include <string>
#include "GenericStub.h"

using std::string;

class CCall
	{
	public:
		CCall( const TCall& aCall );
		~CCall();

		// Return the Call ID
		bool CallID( int& aCallID ) const;

		// Return the number of parameters
		bool Params( int& aNumParams ) const;

		// Return the name of a parameter within the call
		bool Name( int anIndex, string& aName ) const;

		// Return the value of the parameter within the call
		bool Value( int anIndex, string& aValue ) const;

		// Get the index into the TCall array with a matching specified parameter name
		int	 GetIndex(const string &aName) const;

		// Retrieve the value of the specified parameter
		bool Get(const string &aName, string &aValue) const;
		bool Get(const string &aName, int &aValue) const;
		bool Get(const string &aName, bool &aValue) const;

		// Dump the contents to stdout (useful debugging aid)
		void Dump() const;

	private:
		TCall	iCall;

		bool Check( const string& aValue, const unsigned int aMaxLength ) const;
	};

#endif // __CCALL_H__

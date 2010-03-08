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



#ifndef __CUCCININFILE_H__
#define __CUCCININFILE_H__

#include <vector>
#include <string>

using std::string;
using std::vector;

class CUCCIniFile  
{
	public:
		CUCCIniFile();
		CUCCIniFile(const string& strName);
		~CUCCIniFile();

		void SetIniFileName(const string& strName);
		bool KeyValue(const string& strKey, const string& strSection, string& value) const;
		bool KeyValue(const string& strKey, const string& strSection, int& value) const;
		
		vector<string> SectionNames() const;
		vector<string> SectionParameters(const string& strSection) const;

	private:
		string iIniFileName;
};

#endif //__CUCCININFILE_H__

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
* CUCCIniFile.h
*
*/



#include <afxcoll.h>
#include <assert.h>

#include "CUCCIniFile.h"

#define MAX_BUFFER_SIZE 1024

CUCCIniFile::CUCCIniFile()
{
}

CUCCIniFile::CUCCIniFile(const string& strName)
{
	SetIniFileName( strName );
}

CUCCIniFile::~CUCCIniFile()
{
}

void CUCCIniFile::SetIniFileName(const string& strName)
{
	assert( strName.size() != 0 );
	iIniFileName = strName;
}

bool CUCCIniFile::KeyValue(const string& strKey,const string& strSection, string& value ) const
{
	TCHAR result[MAX_BUFFER_SIZE];
	int ret = GetPrivateProfileString(	(LPCTSTR)strSection.c_str(),
										(LPCTSTR)strKey.c_str(),
										_T(""),
										result,
										MAX_BUFFER_SIZE,
										(LPCTSTR)iIniFileName.c_str());

	if( ret>0 )
	{
		value = result;
	}
	return ret>0?true:false;
}

bool CUCCIniFile::KeyValue(const string& strKey, const string& strSection, int& value) const
{
	char* stopstring = NULL;
	string tmpKeyValue;

	bool ret = KeyValue( strKey, strSection, tmpKeyValue );
	if( ret )
	{
		// Check to see if the input value is in hex
		if( tmpKeyValue[0] == '0' && tmpKeyValue[1] == 'x' )
		{
			value = strtol(tmpKeyValue.c_str()+2, &stopstring, 16 );
		}
		else
		{
			value = strtol(tmpKeyValue.c_str(), &stopstring, 10 );
		}
	}
	return ret;
}

vector<string> CUCCIniFile::SectionNames() const
{
	TCHAR result[MAX_BUFFER_SIZE];
	long lRetValue = GetPrivateProfileSectionNames(	result,
													MAX_BUFFER_SIZE,
													(LPCTSTR)iIniFileName.c_str());

	// Return a vector of all the section names
	vector<string> sections;

	int pos = 0;
	while( pos <= MAX_BUFFER_SIZE )
	{
		string section = (char*)(result+pos);
		if( section.size() > 0 )
		{
			sections.push_back( section );
			pos +=section.size()+1;
		}
		else
		{
			break;
		}
	}

	return sections;
}

vector<string> CUCCIniFile::SectionParameters(const string& strSection) const
{
	TCHAR result[MAX_BUFFER_SIZE];
	long lRetValue = GetPrivateProfileString(	(LPCTSTR)strSection.c_str(),
												NULL,
												_T(""),
												result,
												MAX_BUFFER_SIZE,
												(LPCTSTR)iIniFileName.c_str());

	// Return a vector of parameters
	vector<string> params;

	int pos = 0;
	while( pos <= MAX_BUFFER_SIZE )
	{
		string param = (char*)(result+pos);
		if( param.size() > 0 )
		{
			params.push_back( param );
			pos +=param.size()+1;
		}
		else
		{
			break;
		}
	}

	return params;
}

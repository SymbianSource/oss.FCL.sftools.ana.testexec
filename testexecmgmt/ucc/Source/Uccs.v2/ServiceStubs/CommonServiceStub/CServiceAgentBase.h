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
* CServiceAgentBase
*
*/



#ifndef __CSERVICEAGENTBASE_H__
#define __CSERVICEAGENTBASE_H__


/*******************************************************************************
 *
 * System Includes
 *
 ******************************************************************************/
#include <rpc/rpc.h>
#include <rpc/types.h>


/*******************************************************************************
 *
 * Local Includes
 *
 ******************************************************************************/
#include "../../Core/UCCS_Interfaces.h"
#include "../../Core/UCCS_ServiceValues.h"


/*******************************************************************************
 *
 * Macro Functions
 *
 ******************************************************************************/
#define GETSTRINGARGUMENT(field,token,index,optional,request,reply)		{	int _err; \
																			_err = GetStringArgument(field,token,index,optional,request,reply); \
																			if( _err != 0 ) \
																				break; \
																		}


#define GETINTEGERARGUMENT(fieldname,var,index,optional,request,reply)		{	int _err; \
						 														_err = GetIntegerArgument(fieldname,var,index,optional,request,reply); \
																				if( _err != 0 ) \
																					break; \
																			}


#define SETINTEGERRESULT( client_call_result, success_value, result, request)			{	int _err; \
																							if( client_call_result == success_value ) { \
																								_err = request->NewField( "RESULT", result ); \
																								assert( _err == UCCS_OK ); \
																							} \
																						} 


/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define STD_REPLY_FIELD_SERVICE_ID					"SVCID"
#define STD_REPLY_FIELD_SERVICE_HOST				"SVCHOST"
#define STD_REPLY_FIELD_SERVICE_NAME				"SVCNAME"
#define STD_REPLY_FIELD_METHODID					"METHODID"
#define STD_REPLY_FIELD_CALL						"CALL"
#define STD_REPLY_FIELD_REQUESTCOMPLETIONCODE		"REQUESTCOMPLETIONCODE"
#define STD_REPLY_FIELD_REQUESTCOMPLETIONDESC		"REQUESTCOMPLETIONDESC"

#define STD_REPLY_FIELD_RESULT						"RESULT"


/*******************************************************************************
 *
 * CServiceAgentBase
 *
 ******************************************************************************/
class CServiceAgentBase
{
public:
	CServiceAgentBase();
	~CServiceAgentBase();

protected:

	// Stateless data transformers
	char *IntToAddress( int aAddr );
	int AddressToInt( char *aAddress );

	// Extract named arguments from a record 
	int GetStringArgument( char *aFieldName, char **aOutput, int aParamIndex, int aOptional, CDataRecord *aRequest, CDataRecord *aReply );
	int GetIntegerArgument( char *aFieldName, int *aOutput, int aParamIndex, int aOptional, CDataRecord *aRequest, CDataRecord *aReply );

	// Add return values to reply record
	void AddIteratedIntegerFieldName( char *aFieldname, int aIndex, int aValue, CDataRecord *aReply );
	void AddIteratedStringFieldName( char *aFieldname, int aIndex, char *aValue, CDataRecord *aReply );

	// Update the standard fields of a reply record
	CDataRecord *CreateBaseReply( CDataRecord *aRequest );
	void UpdateCompletionCode( CDataRecord *aReply, int aPenStdErrorCode );
	virtual char *GetLastRPCError( int *aRpcErrorInt );

	// Misc helpers
	void XdrFree( char **ptr, int *len );

private:
//	void AddCompletionCodeDesc( CDataRecord *aReply, TRequestCompletionCode aResult );
	char iAddressString[32];
};

#endif //__CSERVICEAGENTBASE_H__

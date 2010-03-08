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
* System Includes
*
*/



#include <cdbcols.h>
#include <e32std.h>
#include <d32dbms.h>
#include <stdlib.h>
#include <charconv.h>
#include <f32file.h> 
#include <libc/string.h>


/*****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/
#include "assert.h"
#include "NetworkEmulatorControl.h"


/*****************************************************************************
 *
 *	PUBLIC METHOD: Constructor
 *
 ****************************************************************************/	
EXPORT_C CSetUpCommDb::CSetUpCommDb()
{
	iStatus = TCOMMBD_IDLE;
}

/*****************************************************************************
 *
 *	PUBLIC METHOD: Destructor
 *
 ****************************************************************************/
EXPORT_C CSetUpCommDb::~CSetUpCommDb()
{
	if( iStatus == TCOMMBD_CONNECTED ) {
		assert( iDbComms != NULL ); 
		delete iDbComms;
		iDbComms = NULL;
		iStatus = TCOMMBD_IDLE; 
	}
	assert( iStatus == TCOMMBD_IDLE );
}

/*****************************************************************************
 *
 *	PUBLIC METHOD: Initialise the database
 *
 ****************************************************************************/
EXPORT_C TCommDBError CSetUpCommDb::initialise(void)
{
	assert( iStatus == TCOMMBD_IDLE);

	// Connect to the database.
	// Specifying type as EFalse, so if the dB does not exist, function leaves.
	TRAPD (r, iDbComms = CCommsDatabase::NewL(EFalse));
	if( r != KErrNone ) {
		return 	TCommDB_ErrorInitCommDb;
	}
	assert ( iDbComms != NULL );
	iStatus = TCOMMBD_CONNECTED;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	SECTION: updateEntry
 *
 ****************************************************************************/
/*****************************************************************************
 *
 *	PUBLIC METHOD: updateEntry
 *
 ****************************************************************************/
EXPORT_C TCommDBError CSetUpCommDb::updateEntry(TPtrC aTable, TUint32 aRecordID, TPtrC aFieldName, char* aFieldValue, int* aErrCode)
{
	CCommsDbTableView *table_handle = NULL;
	int ret = KErrNone;

	// Check the params
	assert ( aFieldValue != NULL );
	assert ( aErrCode != NULL );
	*aErrCode = 0;

	// Check the state
	assert( iStatus == TCOMMBD_CONNECTED );
	assert ( iDbComms != NULL );

	// Open the table we are looking for
	TRAP( ret, (table_handle = openTableL(iDbComms,aTable)) );
	if( (ret != KErrNone) || (table_handle == NULL) ) {
		*aErrCode = ret;
		return TCommDB_ErrorTableNotFound;
	}

	// find and update the record
	FindAndUpdateRecord( table_handle, aRecordID, aFieldName, aFieldValue, aErrCode );

	// cleanup and return
	delete table_handle;
	table_handle = NULL;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	PRIVATE METHOD: FindAndUpdateRecord
 *
 ****************************************************************************/
TCommDBError CSetUpCommDb::FindAndUpdateRecord( CCommsDbTableView *aTable, TUint32 aRecordID, TPtrC aFieldName, char* aFieldValue, int* aErrCode)
{
	int err, ret;
	TUint32 id = 0;

	// Look for the matching record in the table
	err = aTable->GotoFirstRecord();
	while( err == KErrNone ) {

		// Read the id for each record in the table and compare to the one we are looking for.
		TRAP( ret, (aTable->ReadUintL(TPtrC(COMMDB_ID),id)) );
		if( ret != KErrNone ) {	
			*aErrCode = ret;
			return TCommDB_ErrorReadingRecord;
		}

		// We have found the record we are looking for then update the field	
		if( id == aRecordID) {
			return UpdateField( aTable, aFieldName, aFieldValue, aErrCode );
		}
	}

	// no matching record found
	return TCommDB_RecNotFound;
}


/*****************************************************************************
 *
 *	PRIVATE METHOD: UpdateField
 *
 ****************************************************************************/
TCommDBError CSetUpCommDb::UpdateField( CCommsDbTableView *aTable, TPtrC aFieldName, char* aFieldValue, int* aErrCode )
{
	int err, ret;
	TDbColType colType = EDbColBinary;
	TUint32 attrib;
	TUint32 entry_integer;

	// Get the type of the field we are writing to
	TRAP( ret, (aTable->ReadTypeAttribL(aFieldName,colType,attrib)) );
	if( ret != KErrNone ) {		
		*aErrCode = ret;
		return TCommDB_ErrorColNotFound;
	}

	// Must call update record before editing any existing record.
	err = aTable->UpdateRecord();
	if( err != KErrNone ) {	
		*aErrCode = err;
		return TCommDB_ErrorUpdatingRecord;
	}

	// Convert the string value to the appropriate type and write entry to database
	switch (colType) {

	// convert the string to an integer and update the entry
	case EDbColUint16:
	case EDbColUint32:
	case EDbColUint8:
		entry_integer = (TUint32)atoi((const char*)aFieldValue);
		TRAP( ret, (aTable->WriteUintL(aFieldName,entry_integer)) );
		if( ret != KErrNone ) {		
			*aErrCode = ret;
			return TCommDB_ErrorUpdatingRecord;
		}
		break;

	// no need for conversion - just add the string
	case EDbColText8:
	{
		TPtrC8 acsiiPtr((TUint8*)aFieldValue);
		TRAP( ret, (aTable->WriteTextL(aFieldName,acsiiPtr)) );
		if( ret != KErrNone ) {
			*aErrCode = ret;
			return TCommDB_ErrorUpdatingRecord;
		}
		break;
	}

	// convert the ascii string to unicode and update the entry
	case EDbColText16:
		err = SetUnicodeString( aTable, aFieldName, aFieldValue, aErrCode );
		if( err != TCommDB_Success ) {
			return TCommDB_ErrorUpdatingRecord;	
		}
		break;

	// no need for conversion - just add the string
	case EDbColLongText8:
	{
		TPtrC8 asciiPtrLong((TUint8*)aFieldValue);
		TRAP( ret, (aTable->WriteTextL(aFieldName,asciiPtrLong)) );
		if( ret != KErrNone ) {		
			*aErrCode = ret;
			return TCommDB_ErrorUpdatingRecord;
		}
		break;
	}

	// convert the ascii string to unicode and update the entry
	case EDbColLongText16:
		ret = SetUnicodeStringLong( aTable, aFieldName, aFieldValue, aErrCode );
		if( ret != TCommDB_Success ) {
			return TCommDB_ErrorUpdatingRecord;	
		}
		break;

	// unknown column type
	default:
		return TCommDB_ErrorColTypeValueNotSupported;
	}

	// commit the changes
	err = aTable->PutRecordChanges();
	if( err != KErrNone ) {
		*aErrCode = ret;
		return TCommDB_ErrorUpdatingRecord;
	}

	// done
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	SECTION: Get...
 *
 ****************************************************************************/
/*****************************************************************************
 *
 *	PUBLIC METHOD: getIntEntry
 *
 ****************************************************************************/
EXPORT_C TCommDBError CSetUpCommDb::getIntEntry( TPtrC aTable, TUint32 aRecordID, TPtrC aFieldName, TUint32& aFieldValue, int* aErrCode )
{
	int ret;
	TCommDBError rv;
	CCommsDbTableView* table = NULL;
	TDbColType colType;

	// check params and state
	assert( iStatus == TCOMMBD_CONNECTED );
	assert ( iDbComms != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// open the table
	TRAP( ret, (table = openTableL(iDbComms, aTable)) );
	if( (ret != KErrNone) || (table == NULL) ) {
		*aErrCode = ret;
		return TCommDB_ErrorTableNotFound;
	}

	// get to the record 
	rv = FindRecordAndGetType( table, aRecordID, aFieldName, &colType, aErrCode );
	if( rv != TCommDB_Success ) {
		delete table;
		return rv;
	}

	// check the field has the expected type
	if( (colType != EDbColUint16) && (colType != EDbColUint8) ) {
		delete table;
		return TCommDB_ErrorUnexpectedColType;
	}

	// get the field
	TRAP( ret, (table->ReadUintL(aFieldName,aFieldValue)) );
	if ( ret != KErrNone ) {	
		delete table;
		*aErrCode = ret;
		return TCommDB_ErrorReadingRecord;
	}

	// done
	delete table;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	PUBLIC METHOD: getBoolEntry
 *
 ****************************************************************************/
EXPORT_C TCommDBError CSetUpCommDb::getBoolEntry(TPtrC aTable, TUint32 aRecordID, TPtrC aFieldName, TBool& aFieldValue, int* aErrCode)
{
	int ret;
	TCommDBError rv;
	CCommsDbTableView* table = NULL;
	TDbColType colType;

	// check params and state
	assert( iStatus == TCOMMBD_CONNECTED );
	assert ( iDbComms != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// open the table
	TRAP( ret, (table = openTableL(iDbComms, aTable)) );
	if( (ret != KErrNone) || (table == NULL) ) {
		*aErrCode = ret;
		return TCommDB_ErrorTableNotFound;
	}

	// get to the record 
	rv = FindRecordAndGetType( table, aRecordID, aFieldName, &colType, aErrCode );
	if( rv != TCommDB_Success ) {
		delete table;
		return rv;
	}

	// check the field has the expected type
	if( colType != EDbColBit ) {
		delete table;
		return TCommDB_ErrorUnexpectedColType;
	}

	// get the field
	TRAP( ret, (table->ReadBoolL(aFieldName,aFieldValue)) );
	if ( ret != KErrNone ) {	
		delete table;
		*aErrCode = ret;
		return TCommDB_ErrorReadingRecord;
	}

	// done
	delete table;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	PUBLIC METHOD: getAsciiEntry
 *
 ****************************************************************************/
EXPORT_C TCommDBError CSetUpCommDb::getAsciiEntry(TPtrC aTable, TUint32 aRecordID, TPtrC aFieldName, TDes8& aFieldValue, int* aErrCode)
{
	int ret;
	TCommDBError rv;
	CCommsDbTableView* table = NULL;
	TDbColType colType;

	// check params and state
	assert( iStatus == TCOMMBD_CONNECTED );
	assert ( iDbComms != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// open the table
	TRAP( ret, (table = openTableL(iDbComms, aTable)) );
	if( (ret != KErrNone) || (table == NULL) ) {
		*aErrCode = ret;
		return TCommDB_ErrorTableNotFound;
	}

	// get to the record 
	rv = FindRecordAndGetType( table, aRecordID, aFieldName, &colType, aErrCode );
	if( rv != TCommDB_Success ) {
		delete table;
		return rv;
	}

	// check the field has the expected type
	if( colType != EDbColText8 ) {
		delete table;
		return TCommDB_ErrorUnexpectedColType;
	}

	// get the field
	TRAP( ret, (table->ReadTextL(aFieldName,aFieldValue)) );
	if ( ret != KErrNone ) {	
		delete table;
		*aErrCode = ret;
		return TCommDB_ErrorReadingRecord;
	}

	// done
	delete table;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	PUBLIC METHOD: getUnicodeEntry
 *
 ****************************************************************************/
EXPORT_C TCommDBError CSetUpCommDb::getUnicodeEntry(TPtrC aTable, TUint32 aRecordID, TPtrC aFieldName, TDes16& aFieldValue, int* aErrCode)
{
	int ret;
	TCommDBError rv;
	CCommsDbTableView* table = NULL;
	TDbColType colType;

	// check params and state
	assert( iStatus == TCOMMBD_CONNECTED );
	assert ( iDbComms != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// open the table
	TRAP( ret, (table = openTableL(iDbComms, aTable)) );
	if( (ret != KErrNone) || (table == NULL) ) {
		*aErrCode = ret;
		return TCommDB_ErrorTableNotFound;
	}

	// get to the record 
	rv = FindRecordAndGetType( table, aRecordID, aFieldName, &colType, aErrCode );
	if( rv != TCommDB_Success ) {
		delete table;
		return rv;
	}

	// check the field has the expected type
	if( colType != EDbColText16 ) {
		delete table;
		return TCommDB_ErrorUnexpectedColType;
	}

	// get the field
	TRAP( ret, (table->ReadTextL(aFieldName,aFieldValue)) );
	if ( ret != KErrNone ) {	
		delete table;
		*aErrCode = ret;
		return TCommDB_ErrorReadingRecord;
	}

	// done
	delete table;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 * PRIVATE METHOD: FindRecordAndGetType
 *
 ****************************************************************************/
TCommDBError CSetUpCommDb::FindRecordAndGetType( CCommsDbTableView *aTable, TUint32 aRecordID, TPtrC aFieldName, TDbColType *aColType, int* aErrCode )
{
	int err = KErrNone;
	int ret = KErrNone;
	TUint32 id = 0;
	TDbColType colType = EDbColBit;
	TUint32 attrib;

	// check params
	assert( aTable != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// find the matching record in the table
	err = aTable->GotoFirstRecord();
	while( err == KErrNone) {

		// get the id field
		TRAP( ret, (aTable->ReadUintL(TPtrC(COMMDB_ID),id)) );
		if( ret != KErrNone ) {	
			*aErrCode = ret;
			return TCommDB_ErrorReadingRecord;
		}

		// see if the id matches the requested id
		if( id == aRecordID) {
			break;
		}

		// go to the next record
		err = aTable->GotoNextRecord();
	}

	// see if no record was found
	if( err != KErrNone ) {
		return TCommDB_RecNotFound;
	}

	// check the type of the field and see if it matches Int
	TRAP( ret, (aTable->ReadTypeAttribL(aFieldName,colType,attrib)) );
	if( ret != KErrNone ) {
		*aErrCode = ret;
		return TCommDB_ErrorColNotFound;
	}

	// return the coltype
	*aColType = colType;

	// done
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	SECTION: Data Writers
 *
 ****************************************************************************/
/*****************************************************************************
 *
 *	Set a unicode string in the db
 *
 ****************************************************************************/
int CSetUpCommDb::SetUnicodeString( CCommsDbTableView* aTable, TPtrC aFieldName, char* aFieldValue, int* aErrCode )
{
	int ret;
	HBufC16 *unicode_string;

	// check params
	assert( aTable != NULL );
	assert( aFieldValue != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;

	// convert the passed string to unicode			
	unicode_string = ConvertAsciiToUnicode( aFieldValue );
	if( unicode_string == NULL ) {
		return TCommDB_ErrorConvertingToUnicode;
	}
	TPtr16 unicode_string_ptr( unicode_string->Des() );

	// update the entry 
	TRAP( ret, (aTable->WriteTextL(aFieldName,unicode_string_ptr)) );
	if( ret != KErrNone ) {
		delete unicode_string;
		return TCommDB_ErrorUpdatingRecord;
	}
	
	// done 
	delete unicode_string;
	return TCommDB_Success;
}


/*****************************************************************************
 *
 *	Set a long unicode string in the db
 *
 ****************************************************************************/
int CSetUpCommDb::SetUnicodeStringLong(CCommsDbTableView* aTable, TPtrC aFieldName, char* aFieldValue, int *aErrCode)
{
	int ret, i, value_length;
	unsigned short *unicode_string;

	// check params
	assert( aTable != NULL );
	assert( aFieldValue != NULL );
	assert( aErrCode != NULL );
	*aErrCode = 0;
	unicode_string = (unsigned short *)aFieldValue;

	// get the length of the (wide) string
	for( i = 0; unicode_string[i] != 0; i++ )
		;
	value_length = i;

	// convert the passed descriptors into the correct format
	TPtrC field_value_ptr( unicode_string, value_length );

	// update the entry 
	TRAP( ret, (aTable->WriteLongTextL(aFieldName,field_value_ptr)) );
	if( ret != KErrNone ) {
		return TCommDB_ErrorUpdatingRecord;
	}
	
	// done 
	return TCommDB_Success;
}	


/*****************************************************************************
 *
 *	SECTION: Helpers
 *
 ****************************************************************************/
/*****************************************************************************
 *
 *	Open the given table
 *
 ****************************************************************************/
CCommsDbTableView* CSetUpCommDb::openTableL( CCommsDatabase *aDbComms, TPtrC aTable )
{
	assert ( aDbComms != NULL );
	CCommsDbTableView *rv = NULL;
	rv = aDbComms->OpenTableLC( aTable );
	CleanupStack::Pop();
	return rv;
}


/*****************************************************************************
 *
 *	Convert an ascii string to unicode
 *
 ****************************************************************************/
HBufC16 *CSetUpCommDb::ConvertAsciiToUnicode( char *aAsciiString )
{
	RFs aFs;
	int error_thrown, err;
	TInt state;
	HBufC16* unicode_string = NULL;
	CCnvCharacterSetConverter* conv = NULL;
	CCnvCharacterSetConverter::TAvailability available;

	// Setup the descriptors and allocate the memory
	const TPtrC8 textData( (TUint8*)aAsciiString );
	TRAP( error_thrown, (unicode_string = HBufC16::NewL(textData.Length())) );	
	if( (error_thrown != KErrNone) || (unicode_string == NULL) ) {
		return NULL;
	}
	TPtr16 textPtr( unicode_string->Des() );

	// connect to the file-system to do the conversion
	err = aFs.Connect();				
	if( err != KErrNone) {	
		return NULL;
	}

	// get the character convertor
	TRAP( error_thrown, (conv = CCnvCharacterSetConverter::NewL()) );
	if( (error_thrown != KErrNone) || (conv == NULL) ) {
		delete unicode_string;
		return NULL;
	}

	// Check if the required character set we are converting too is available
	available = conv->PrepareToConvertToOrFromL( KCharacterSetIdentifierUtf8, aFs );  
	if( available == CCnvCharacterSetConverter::ENotAvailable ) {
		delete unicode_string;
		delete conv;
		return NULL;
	}

	// finally, do the actual conversion
	state = CCnvCharacterSetConverter::KStateDefault;
	conv->ConvertToUnicode( textPtr, textData, state );

	// ok we are done - return the new string
	delete conv;
	return unicode_string;
}


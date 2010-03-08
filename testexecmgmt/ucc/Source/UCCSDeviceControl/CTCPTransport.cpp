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



#include <e32def.h>
#include <inet.h>

#include "CTCPTransport.h"
#include "assert.h"

#include <CommDbConnPref.h>


CTCPTransport::CTCPTransport()
	{
	}

CTCPTransport::~CTCPTransport()
	{
	iSocket.Close();
	iServer.Close();
	}

TInt CTCPTransport::InitialiseL()
	{
	return KErrNone;
	}

TInt CTCPTransport::Release( void )
	{
	iSocket.Close();
	iServer.Close();
	return KErrNone;
	}

TInt CTCPTransport::ConnectL( TDesC* aRemoteHost )
	{
	// Retrieve the destination port location
	TInt colonPos = aRemoteHost->Locate( TChar(':') );
	if(colonPos != KErrNotFound)
		{
		// Set the destination port
		TPtrC host( aRemoteHost->Left(colonPos) );
		TLex portLex( aRemoteHost->Mid(colonPos + 1) );
		TInt port = 0;
		portLex.Val( port );
		iDestAddr.SetPort( port );

		// Retrieve and set the destination ip address
		TBuf<20> ipAddress;
		ipAddress.Copy( host.Ptr(), colonPos );
		iDestAddr.Input( ipAddress );
		}
	else
		{
		iDestAddr.SetPort( KUCCTCPPort );
		}
	
	// pick out the iap/snap index..
	TInt index=0;
	TPtrC mode;

	TInt pipePos = aRemoteHost->Locate( TChar('|') );
	if( pipePos != KErrNotFound )
		{
		TPtrC tempstr( aRemoteHost->Ptr() + pipePos + 1 );
		TInt indexPos = tempstr.Locate( TChar(':') );

		if( indexPos != KErrNotFound )
			{
			mode.Set( tempstr.Left(indexPos) );
			TLex iapLex( tempstr.Mid(indexPos + 1) );
			iapLex.Val( index );
			}
		}
	
	// Connect to the server - need to retry as the first few attepts seem to fail over PPP
	User::LeaveIfError( iServer.Connect() );
	User::LeaveIfError( iConnection.Open(iServer) );

	if( index > 0 && mode.Compare( _L("IAP") ) == 0 )
		{
		TCommDbConnPref pref;
		pref.SetIapId(index);
		pref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
		User::LeaveIfError(iConnection.Start(pref));
		}
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	else if( index > 0 && mode.Compare( _L("SNAP") ) == 0 )
		{
		TConnSnapPref pref(index);
		User::LeaveIfError(iConnection.Start(pref));
		}
#endif // SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	else
		{
		User::LeaveIfError(iConnection.Start());
		}

	TInt ret = -1;
	while( ret )
		{
		User::LeaveIfError( iSocket.Open(iServer, KAfInet, KSockStream, KProtocolInetTcp, iConnection) );
		TRequestStatus status;
		iSocket.Connect( iDestAddr, status );
		User::WaitForRequest( status );
		ret = status.Int();
		}

	return KErrNone;
	}

TInt CTCPTransport::Disconnect( void )
	{
	iSocket.Close();
	iServer.Close();
	return KErrNone;
	}

TInt CTCPTransport::RequestSend( TDesC8 *aData, const TUint aDataLength )
	{
	assert( aData->Length() == aDataLength );
	// Write to the socket
	TRequestStatus status;
	iSocket.Write( *aData, status );
	User::WaitForRequest( status );
	return status.Int();
	}

TInt CTCPTransport::RequestReceive( TPtr8 *aRecvBufferPtr, TUint aByteCount )
	{
	assert( aRecvBufferPtr->MaxLength() == aByteCount );
	// Read from the socket
	TRequestStatus status;
	iSocket.Read( *aRecvBufferPtr, status );
	User::WaitForRequest( status );
	return status.Int();
	}

TText8 *CTCPTransport::Error( void )
	{
	return NULL;
	}

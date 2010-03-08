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
* Local Includes
*
*/



#include "assert.h"
#include "CSerialTransport.h"

/**************************************************************************************
 *
 * Definitions
 *
 *************************************************************************************/
_LIT(LDD_NAME,"ECOMM");
#ifdef __WINS__
_LIT(PDD_NAME,"ECDRV");
#else
_LIT(PDD_NAME,"EUART1");
#endif

#define KMaxTimeoutRetries	(0x0FFFFFFF)
#define KWriteTimeout		50000000

/********************************************************************************
 *
 * Macro functions
 *
 ********************************************************************************/

/**************************************************************************************
 *
 * CSerialTransport - Construction
 *
 *************************************************************************************/
CSerialTransport* CSerialTransport::NewL( TPtrC16 aModule )
{
    CSerialTransport *self = new (ELeave) CSerialTransport();
    CleanupStack::PushL(self);
	self->ConstructL( aModule );
	CleanupStack::Pop();
    return self;
}

CSerialTransport::CSerialTransport() 
{
}

CSerialTransport::~CSerialTransport()
{
	iCommPort.Close();
	iCommServer.Close();
}

void CSerialTransport::ConstructL( TPtrC16 aModule )
{
	// set parameters
	assert( aModule.Length() <= KModuleSize );
	iModule.Copy( aModule );
	iSerialStatus = EIdle;
	iRetries = 0;
	iCommPortOpen = 0;
	iCommOldSettingsValid = 0;
}

/**************************************************************************************
 *
 * CSerialTransport - Initialise and Release
 *
 *************************************************************************************/
TInt CSerialTransport::InitialiseL()
{
	TInt r;

	// set the state
	SetStatus( EInitialising );

	// Under WINS we must force a link to the file server so that we're sure we'll be 
	// able to load the device drivers. On a MARM implementation, this code would not
	// be required because higher level components (EIKON) will automatically have started 
	// the services. NOTE: this is now no longer required since we are an app and so even
	// on WINS everything else should have started up by now.

	// Load up the physical and the logical device drivers. If they are already loaded 
	// then it won't make any difference.
	r = User::LoadPhysicalDevice( PDD_NAME );
	if( (r != KErrNone) && (r != KErrAlreadyExists) ) {
		User::Leave( r );
	}
	r = User::LoadLogicalDevice( LDD_NAME );	
	if( (r != KErrNone) && (r != KErrAlreadyExists) ) {
		User::Leave( r );
	}

	// Both WINS and EIKON will have started the comms server process.
	// (this is only really needed for ARM hardware development racks)
#ifndef __WINS__
	r = StartC32();
	if( (r != KErrNone) && (r != KErrAlreadyExists) )
		User::Leave( r );
#endif

	// Now connect to the comm server	
	User::LeaveIfError( iCommServer.Connect() );

	// Load the CSY module
	r = iCommServer.LoadCommModule( iModule );
	User::LeaveIfError( r );

	// check we loaded correctly
	TInt numPorts;
	r = iCommServer.NumPorts( numPorts );
	User::LeaveIfError( r );

	// set the state
	SetStatus( EInitialised );
	return KErrNone;
}

TInt CSerialTransport::Release(void)
{
	// make sure the status is as expected
	assert( (iSerialStatus == EInitialising) || (iSerialStatus == EInitialised) || (iSerialStatus == EDisconnected) );
	SetStatus( EReleasing );

	// disconnect from the comms server
	iCommServer.Close();

	// update state and finish
	SetStatus( EIdle );
	return KErrNone;
}

/**************************************************************************************
 *
 * CSerialTransport - Connect and Close
 *
 *************************************************************************************/
static int atoi( const short *str )
{
	int ret = 0;
	for( int i = 0; str[i] != NULL; i++ ) {
		ret *= 10;
		ret += str[i] - '0';
	}
	return ret;
}

void CSerialTransport::ExtractOptions( TDesC *aRemoteHost, TInt& aPortNumber, TInt& aBaudCap, TBps& aBaudRate )
{
	TInt baud;
	short *delim;

	// get the character array for the remote host (note that this is in unicode)
	short *opt = (short*)aRemoteHost->Ptr();

	// extract the portnumber
	aPortNumber = opt[0] - '0';

	// make sure the next char is a delimiter
	assert( opt[1] == OPT_DELIMITER );

	// now search for the next delimiter and NULL it
	for( delim = &opt[2]; (*delim != NULL) && (*delim != OPT_DELIMITER); delim++ )
		;

	// if this is a delim then NULL it
	if( *delim == OPT_DELIMITER ) {
		*delim = NULL;
	}

	// extract the baud -- and set the correct constants for the given baud
	baud = atoi( &(opt[2]) ); 
	switch( baud ) {
	case 115200:
		aBaudRate = EBps115200;
		aBaudCap = KCapsBps115200;
		break;
	case 38400:
		aBaudRate = EBps38400;
		aBaudCap = KCapsBps38400;
		break;
	case 19200:
		aBaudRate = EBps19200;
		aBaudCap = KCapsBps19200;
		break;
	case 9600:
		aBaudRate = EBps9600;
		aBaudCap = KCapsBps9600;
		break;
	default:
		assert( !"Unsupported baud rate" );
		break;
	}
}

TInt CSerialTransport::ConnectL( TDesC *aRemoteHost )
{
	TInt portNumber;
	TInt baudCap;
	TBps baudRate;

	TBuf8<1> iDummyBuffer;
	
	// verify state 
	assert( iSerialStatus == EInitialised );
	SetStatus( EConnecting );

	// extract the options from the string
	ExtractOptions( aRemoteHost, portNumber, baudCap, baudRate ); 

	// construct the address -- aRemoteHost should provide the COM port number
	TBuf16<KMaxPortName + 4> portName;
	portName.Num( portNumber );
	portName.Insert( 0, _L("::") );
	if( iModule == _L("IrCOMM") ) 
		portName.Insert( 0, _L("IrCOMM") );
	else if( iModule == _L("ECUART") )
		portName.Insert( 0, _L("COMM") );

	// open the serial port
	TInt r = iCommPort.Open( iCommServer, portName, ECommExclusive );
	User::LeaveIfError( r );
	iCommPortOpen = 1;

	// check our configuration is supported
	TCommCaps ourCapabilities;
	iCommPort.Caps( ourCapabilities );
	if (((ourCapabilities ().iRate & baudCap) == 0) ||
		 ((ourCapabilities ().iDataBits & KCapsData8) == 0) ||
		 ((ourCapabilities ().iStopBits & KCapsStop1) == 0) ||
		 ((ourCapabilities ().iParity & KCapsParityNone) == 0)) 
	{
			User::Leave( KErrNotSupported );
	}


	// save port settings for restoring later
	iCommPort.Config( iOldPortSettings );
	iCommOldSettingsValid = 1;

	// set new port settings
	iCommPort.Config( iPortSettings );
	iPortSettings().iRate = baudRate;
	iPortSettings().iParity = EParityNone;
	iPortSettings().iDataBits = EData8;
	iPortSettings().iStopBits = EStop1;
	iPortSettings().iFifo = EFifoEnable;
  	iPortSettings().iHandshake = KConfigObeyCTS;
	iPortSettings().iTerminatorCount = 0;

	// cancel any pending reads / writes to be safe and set the config
	iCommPort.Cancel();
	r = iCommPort.SetConfig( iPortSettings );
	if( r != KErrNone ) {
		User::Leave(r);
	}

	// set the receive buffer length then check it did it ok
	iCommPort.SetReceiveBufferLength( 2*KMaxPacketSize );
	if( iCommPort.ReceiveBufferLength() != 2*KMaxPacketSize ) {
		User::Leave( KErrTooBig );
	}

	// power up the serial port by doing a null read on the port
	if( iModule != _L("IrCOMM") ) {
		iCommPort.Read(iStatus, iDummyBuffer,0);
		User::WaitForRequest(iStatus);
		if(iStatus.Int() != KErrNone){
			Disconnect();
			Release();
			User::Leave(iStatus.Int());

		}
	} else {
		iCommPort.Write(iStatus, iDummyBuffer,0);
		User::WaitForRequest(iStatus);
		if(iStatus.Int() != KErrNone){
			Disconnect();
			Release();
			User::Leave(iStatus.Int());
		
		}
	}

	SetStatus(EConnected);
	return KErrNone;
}

TInt CSerialTransport::Disconnect(void)
{
	// verify the status
	assert( (iSerialStatus == EConnected) || (iSerialStatus == EConnecting) );
	SetStatus( EDisconnecting );

	// clean up the port
	if( iCommPortOpen )
		iCommPort.Cancel();
	if( iCommOldSettingsValid )
		iCommPort.SetConfig( iOldPortSettings );
	if( iCommPortOpen )
		iCommPort.Close();
	iCommPortOpen = 0;
	iCommOldSettingsValid = 0;

	// done
	SetStatus( EDisconnected );
	return KErrNone;
}

/**************************************************************************************
 *
 * CSerialTransport - Receive and Send. The ID / Length / Data nonsense 
 * is handled by the packetisation layer. All serial has to do here is send data -- 
 * serial is also assuming that the upper layer will keep the data until the response
 *
 *************************************************************************************/
TInt CSerialTransport::RequestSend( TDesC8 *aData, const TUint aDataLength )
{

	// make sure we are in the appropriate state 
	assert( iSerialStatus == EConnected );

	// do the send
	assert( (unsigned)aData->Length() == aDataLength );
	iCommPort.Write( iStatus, (*aData), aDataLength );
	User::WaitForRequest(iStatus);
	return iStatus.Int();
}

TInt CSerialTransport::RequestReceive( TPtr8 *recvBufferPtr,TUint aByteCount  )
{
	// make sure we are in the appropriate state
	assert( iSerialStatus == EConnected );

	// allocate a buffer for the read
	assert( aByteCount <= KMaxPacketSize );
	recvBufferPtr->SetLength( 0 );
	iCommPort.ReadCancel();
	iCommPort.Read( iStatus, *recvBufferPtr, aByteCount );
	User::WaitForRequest(iStatus);
	return iStatus.Int();
}

TText8 *CSerialTransport::Error( void )
{
	return NULL;
}

/**************************************************************************************
 *
 * CSerialTransport - Private Functions
 *
 *************************************************************************************/
void CSerialTransport::SetStatus( TCommStatus aNewStatus )
{
	iSerialStatus = aNewStatus;
}

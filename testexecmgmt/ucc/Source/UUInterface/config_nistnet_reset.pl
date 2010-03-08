#
# Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
# Description:  
#

my $LOAD_NISTNET="/home/penuser/3GNetworkEmulator/bin/Load.Nistnet";
my $REM_MOD="rmmod nistnet";
my $CHECK_MOD="lsmod | grep nistnet";

#################################################################################################
#
# Main()
#
#################################################################################################

# Check if the module is loaded -- and unload it if it is
my $result = system( $CHECK_MOD );
if( $result != 0 ) {
	print( "INFO: Nistnet module is not loaded.\n" );
} else {
	$result = system( $REM_MOD );
	if( $result != 0 ) {
		print( "ERROR: Unable to remove the nistnet module.\n" );
		exit( -1 );
    	}
}

# Now reload the module
$result = system( $LOAD_NISTNET );
if( $result != 0 ) {
	print( "ERROR: Unable to load the nistnet module.\n" );
	exit( -1 );
}
 
# Success
exit( 0 );

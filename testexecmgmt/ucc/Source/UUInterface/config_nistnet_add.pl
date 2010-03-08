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
#
# Description:  
#

#!/usr/bin/perl
#

my $NISTNET="/usr/local/bin/cnistnet";
my $EFILE="1.txt";
my $NFILE="2.txt";

#################################################################################################
#
# Main()
#
#################################################################################################

# DEBUG: Show the passed line
my $argc = scalar(@ARGV);
my $dstr = "ArgC: $argc\nArgV: @ARGV\n";
#print $dstr;

# CASE 1 -- REMOVE ENTRIES
if( $ARGV[0] =~ /-r/ ) {
    system( "$NISTNET -r $ARGV[1] $ARGV[2]" );
    exit( 0 );
}

# CASE 2 -- NISTNET DOWN
if( $ARGV[0] =~ /-d/ ) {
    system( "$NISTNET -d" );
    exit( 0 );
}


# CASE 3 -- ADD ENTRY 

# Make sure that NISTNET is running and start it if it isn't
my $result = system( "$NISTNET -G | grep \"Emulator is: On\" > /dev/null" );
if( $result != 0 ) {
    system( "$NISTNET -u" );
    my $result = system( "$NISTNET -G | grep \"Emulator is: On\" > /dev/null" );
    if( $result != 0 ) {
	print( "ERROR: Nistnet is not running.\n" );
	exit( -1 );
    }
}


# Remove the temporary files created previously
system( "rm -f 1.txt 2.txt" );

# Now get the nistnet output and put into file 1.txt
my $result = system( "$NISTNET -R > $EFILE" );


# Grep the nistnet output for our channel
my $gstr = "grep -- \"$ARGV[2] $ARGV[3]\" $EFILE";
my $result = system( "$gstr > $NFILE" );

# DEBUG: print the grep string
#print "GREP STRING: $gstr\n";

# If the output is <> 0 then no match was found so just call nistnet
if( $result != 0 ) {
#    print "cmd: $NISTNET @ARGV\n";
    $result = system( "$NISTNET @ARGV" );
    exit( $result );
}


# Open the file containing the previous command line and read it into an array.
open( CFILE, "$NFILE" ) or die "ERROR: can't open intermediate file $NFILE.\n";
my $cmd = <CFILE>;
my @cmdarray = split( " ", $cmd);


# DEBUG: print the current configuration
#print "Current Configuration: @cmdarray\n";


# Look for the passed option in the existing string. If it is found then blank out these entries
my $option_to_set = $ARGV[4];
my $in_option = 0;
my $len = scalar(@cmdarray);
for( $i = 0; $i < $len; $i++ ) {
    my $current_config_token = $cmdarray[$i];
    if( $current_config_token =~ /^--/ ) {
	$in_option = 0;
    }
    if( $current_config_token eq $option_to_set ) {
	$in_option = 1;
    }
    if( $in_option == 1 ) {
	$cmdarray[$i] = "";
    }
}

# Now append the argument array to the current array
$len = scalar(@ARGV);
for( $i = 4; $i < $len; $i++ ) {
    push( @cmdarray, $ARGV[$i] );
}

# Clear the nistnet cmd as we add this here
$cmdarray[0] = "";

# DEBUG: We now have the complete string
#print "cmda: @cmdarray\n";

# Now run the command
my $result = system( "$NISTNET @cmdarray" );
 
# Success
exit( $result );

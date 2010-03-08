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


$#ARGV==6 or die "7 arguments required";

my $input 		= $ARGV[0];
my $output 		= $ARGV[1];
my $ipAddress	= $ARGV[2];
my $ipGateway 	= $ARGV[3];
my $ipMask 		= $ARGV[4];
my $ipDNS		= $ARGV[5];
my $IpAddrFromServer  = $ARGV[6];

my $writeEnabled = 1;		

open INPUT, "<$input"  or die "Cannot open $input: $!";
open OUTPUT,">$output" or die "Cannot open $output: $!";
  
my $iapTable = <<END;
 	 	<IAP operation=\"add\">
 		<Name>Ethernet WinTAP</Name>
 		<IAPService>LANService.Ethernet WinTAP</IAPService>
 		<IAPBearer>LANBearer.Ethernet WinTAP</IAPBearer>
 		<IAPNetwork>Network.Intranet</IAPNetwork>
 		<IAPNetworkWeighting>0</IAPNetworkWeighting>
 		<LocationRef>Location.Office</LocationRef>
 	</IAP>
END

my $lanBearerTable = <<END;
	<LANBearer operation=\"add\">
		<Name>Ethernet WinTAP</Name>
		<Agent>nullagt.agt</Agent>
		<IfName>ethint</IfName>
		<LDDFilename>enet</LDDFilename>
		<LDDName>=Ethernet</LDDName>
		<PDDFilename>ethertap</PDDFilename>
		<PDDName>Ethertap.Wins</PDDName>
		<PacketDriverName>EtherPkt.drv</PacketDriverName>
		<LastSocketActivityTimeout>-1</LastSocketActivityTimeout>
		<LastSessionClosedTimeout>-1</LastSessionClosedTimeout>
		<LastSocketClosedTimeout>-1</LastSocketClosedTimeout>
    </LANBearer>
END
 
my $lanServiceTable = <<END;
	<LANService operation="add">
        <Name>Ethernet WinTAP</Name>
        <IfNetworks>ip</IfNetworks>
        <IpNetMask>$ipMask</IpNetMask>
        <IpGateway>$ipGateway</IpGateway>
        <IpAddrFromServer>FALSE</IpAddrFromServer>
        <IpAddr>$ipAddress</IpAddr>
        <IpDNSAddrFromServer>$IpAddrFromServer</IpDNSAddrFromServer>
        <IpNameServer1>$ipDNS</IpNameServer1>
        <IpNameServer2>10.23.58.12</IpNameServer2>
        <Ip6DNSAddrFromServer>FALSE</Ip6DNSAddrFromServer>
    </LANService>
END

my $connectionPreferencesTable = <<END;
    <ConnectionPreferences operation="add">
        <Name>ConnectionPreferencesTable1</Name>
        <Ranking>1</Ranking>
        <Direction>OUTGOING</Direction>
        <BearerSet>LAN</BearerSet>
        <DialogPref>PROMPT</DialogPref>
        <IAPRef>IAP.Ethernet WinTAP</IAPRef>
    </ConnectionPreferences>
END
 
while (<INPUT>) 
{	
	 if($writeEnabled == 1)
	 {
	 	print OUTPUT;
 	 }
	 
	 #add IAPTable
	 if(/<IAPTable>/)
	 {
	 	print "<IAPTable> found...";
	 	print OUTPUT $iapTable;
	 	print "updated\n";
  	 }
  	 
  	  #add LANBearerTable
	 if(/<LANBearerTable>/)
	 {
	 	print "<LANBearerTable> found...";
	 	print OUTPUT $lanBearerTable;
	 	print "updated\n";
  	 }
  	 
  	 #add LANServiceTable
	 if(/<LANServiceTable>/)
	 {
	 	print "<LANServiceTable> found...";
	 	print OUTPUT $lanServiceTable;
	 	print "updated\n";
  	 }
  	 
  	  #add ConnectionPreferencesTable
	 if(/<ConnectionPreferencesTable>/)
	 {
		$writeEnabled = 0;
	 	print "<ConnectionPreferencesTable> found...";
	 	print OUTPUT $connectionPreferencesTable;
  	 }

  	 if(/<\/ConnectionPreferencesTable>/)
	 {
		$writeEnabled = 1;
	 	print OUTPUT;
	 	print "updated\n";
  	 }
  	 
}  	 
  
close INPUT;
close OUTPUT;
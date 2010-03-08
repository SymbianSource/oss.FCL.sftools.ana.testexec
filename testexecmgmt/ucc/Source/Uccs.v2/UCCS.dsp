# Microsoft Developer Studio Project File - Name="UCCS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=UCCS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UCCS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UCCS.mak" CFG="UCCS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UCCS - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "UCCS - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "UCCS"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UCCS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\Core" /I "..\..\External\source_for_build\oncrpc" /I "..\include" /I "..\..\..\GPSSimulator\Dev\APICommandHandler\inc" /I "..\..\..\..\testtoolsconn\oncrpc\source_for_build\oncrpc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 oncrpcms.lib SocketLibrary.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:none /machine:I386 /nodefaultlib:"libcd.lib" /out:"..\..\bin\ucc.exe" /libpath:"..\..\BinInternal" /libpath:"..\..\..\..\testtoolsconn\oncrpc\binaries_for_build"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\Core" /I "..\..\External\source_for_build\oncrpc" /I "..\include" /I "..\..\..\GPSSimulator\Dev\APICommandHandler\inc" /I "..\..\..\..\testtoolsconn\oncrpc\source_for_build\oncrpc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 oncrpcms.lib SocketLibrary.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\ucc.exe" /pdbtype:sept /libpath:"..\..\BinInternal" /libpath:"..\..\..\..\testtoolsconn\oncrpc\binaries_for_build"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "UCCS - Win32 Release"
# Name "UCCS - Win32 Debug"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Group "include_"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Core\CRetrieveCommandFromConsole.h
# End Source File
# Begin Source File

SOURCE=.\Core\CRetrieveCommandFromFile.h
# End Source File
# Begin Source File

SOURCE=.\Core\CSynchronisation.h
# End Source File
# Begin Source File

SOURCE=.\Core\CUCCIniFile.h
# End Source File
# Begin Source File

SOURCE=.\Core\HTMLOutput.h
# End Source File
# Begin Source File

SOURCE=..\include\penstd.h
# End Source File
# Begin Source File

SOURCE=..\include\strncpynt.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CBatchEngine.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CCommandDecoder.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CDataField.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CDataRecord.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CExecuteCommand.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CServMgr.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_ErrorCodes.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_Interfaces.h
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_ServiceValues.h
# End Source File
# End Group
# Begin Group "source_"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Core\CRetrieveCommandFromConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\CRetrieveCommandFromFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\CSynchronisation.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\CUCCIniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\HTMLOutput.cpp
# End Source File
# Begin Source File

SOURCE=..\include\penstd.x
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CBatchEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CCommandDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CDataField.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CDataRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CExecuteCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_CServMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_ErrorCodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\UCCS_Main.cpp
# End Source File
# End Group
# End Group
# Begin Group "DeviceControlChannel"

# PROP Default_Filter ""
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DeviceControlChannel\CApplicationControlNull.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CConsolePort.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CFrameBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CommandControlErrors.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CSerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CTcpPort.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CUCCSCommandControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CUCCSCommandProtocol.cpp
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DeviceControlChannel\CApplicationControlNull.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CConsolePort.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CFrameBuffer.h
# End Source File
# Begin Source File

SOURCE=Core\..\DeviceControlChannel\CommandControlErrors.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CProtocolTypes.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CSerialPort.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CTcpPort.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CUCCSCommandControl.h
# End Source File
# Begin Source File

SOURCE=.\DeviceControlChannel\CUCCSCommandProtocol.h
# End Source File
# End Group
# End Group
# Begin Group "ServiceStubs"

# PROP Default_Filter ""
# Begin Group "StandardBaseStub"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\CommonServiceStub\CServiceAgentBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\CommonServiceStub\CServiceAgentBase.h
# End Source File
# End Group
# Begin Group "SS_Mobster"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\CCMobster.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\CCMobster.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\mobster.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\mobster_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\mobster_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\CMobsterStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\CMobsterStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Mobster.v2\mobster.x
# End Source File
# End Group
# Begin Group "SS_Ppp"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\CCPppcontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\CCPppcontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\pppcontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\pppcontroller_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\pppcontroller_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\CPppControllerStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\CPppControllerStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Ppp\pppcontroller.x
# End Source File
# End Group
# Begin Group "SS_ForeignAgent"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files___"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\CCFacontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\CCFacontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\facontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\facontroller_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\facontroller_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\CForeignAgentServiceStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\CForeignAgentServiceStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\ForeignAgent\facontroller.x
# End Source File
# End Group
# Begin Group "SS_HomeAgent"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files____"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\CCHacontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\CCHacontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\hacontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\hacontroller_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\hacontroller_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\CHomeAgentServiceStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\CHomeAgentServiceStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HomeAgent\hacontroller.x
# End Source File
# End Group
# Begin Group "SS_MobileAgent"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files__"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\CCMncontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\CCMncontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\mncontroller.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\mncontroller_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\mncontroller_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\CMobileAgentServiceStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\CMobileAgentServiceStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\MobileAgent\mncontroller.x
# End Source File
# End Group
# Begin Group "SS_UuInterface"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files_______"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\CCUuinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\CCUuinterface.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\uuinterface.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\uuinterface_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\uuinterface_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\CUuinterfaceStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\CUuinterfaceStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\UuInterface\uuinterface.x
# End Source File
# End Group
# Begin Group "SS_Internal"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\Internal\CInternalStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Internal\CInternalStub.h
# End Source File
# End Group
# Begin Group "SS_HostExecute"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files - HE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\CCHostexecute.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\CCHostexecute.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\hostexecute.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\hostexecute_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\hostexecute_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\CHostExecuteStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\CHostExecuteStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecute\hostexecute.x
# End Source File
# End Group
# Begin Group "SS_HostExecuteAsync"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files_____"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\CCHostexecuteasync.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\CCHostexecuteasync.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\hostexecuteasync.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\hostexecuteasync_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\hostexecuteasync_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\CHostExecuteAsyncStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\CHostExecuteAsyncStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\HostExecuteAsync\hostexecuteasync.x
# End Source File
# End Group
# Begin Group "SS_Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\Test\CTestStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\Test\CTestStub.h
# End Source File
# End Group
# Begin Group "SS_GPSSimulator"

# PROP Default_Filter ""
# Begin Group "RPC Generated Files________"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\APICommandHandler.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\APICommandHandler_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\APICommandHandler_xdr.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\CCGpssimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\CCGpssimulator.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\CAPICommandHandlerStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GPSSimulator\CAPICommandHandlerStub.h
# End Source File
# End Group
# Begin Group "SS_GenericStub"

# PROP Default_Filter ""
# Begin Group "RPC generated Files__________"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\CCGenericstub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\CCGenericstub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\GenericStub.h
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\GenericStub_clnt.c
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\GenericStub_xdr.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\CGenericrStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceStubs\GenericStub\CGenericStub.h
# End Source File
# End Group
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Group "PR0084 - TestScripts"

# PROP Default_Filter ""
# Begin Group "UCCSTests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0104.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0105.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0106.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0108.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0109.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0110.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0111.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0112.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0113.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0114.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0115.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0116.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0117.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0119.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0120.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0121.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0122.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\script.0123.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\UCCSTests\UCCSBatch.bat
# End Source File
# End Group
# Begin Group "NistNet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\NistNet\NistNetBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0040.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0041.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0042.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0043.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0044.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0045.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0046.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0047.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0048.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0049.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0050.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0051.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0052.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0053.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0084\NistNet\script.0054.ucd
# End Source File
# End Group
# Begin Group "FAController_UNMAINTAINED"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\FAController\ForeignAgentBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0019.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0020.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0021.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0022.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0023.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0024.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0025.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0026.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0027.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0028.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0030.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0031.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0033.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0034.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\FAController\script.0035.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "HAController_UNMAINTAINED"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\HAController\HAControllerBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0019.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0020.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0021.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0022.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0023.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0024.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0025.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0026.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0027.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0028.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0030.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0033.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0034.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HAController\script.0035.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Mobster_UNMAINTAINED"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\MobsterBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0010.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0011.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0014.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0015.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0016.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0017.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0018.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0019.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0020.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0021.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0025.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0026.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0027.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0028.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0029.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0030.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0031.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\Mobster\script.0032.ucd
# End Source File
# End Group
# Begin Group "PPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PPP\PPPBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0013.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0014.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0022.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0024.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0025.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0026.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0027.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0028.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0029.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0030.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PPP\script.0031.ucd
# End Source File
# End Group
# Begin Group "HostExecuteAsync_UNMAINTAINED"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\HostExecuteBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0010.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0020.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0021.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0022.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0023.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0024.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0025.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0026.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0027.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0028.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0029.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0030.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\HostExecuteAsync\script.0031.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "MNController_UNMAINTAINED"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\MNController\MNControllerBatch.bat
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0019.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0020.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0021.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0022.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0023.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0024.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0025.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0029.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0030.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0033.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0034.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0035.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0036.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0037.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0038.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0039.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0040.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0041.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0042.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0043.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\MNController\script.0044.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ScheduleTest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\ScheduleTestClient\script.0001.ucd
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\ScheduleTest\Test1.ini
# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\ScheduleTest\Test1.script
# End Source File
# End Group
# Begin Group "Lubbock"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\Lubbock\Lubbock_Test.zip
# End Source File
# End Group
# End Group
# Begin Group "Private TestScripts"

# PROP Default_Filter ""
# Begin Group "Demo Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0020.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0021.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0022.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0023.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Standard Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0000.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.0099.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Temp Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestScripts\PrivateScripts\script.1000.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ScheduleTestScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestScripts\ScheduleTestScripts\sts_0100.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "PR0094 - TestScripts"

# PROP Default_Filter ""
# Begin Group "UT_mobster"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0008.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0014.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0015.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0019.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0024.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0028.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0031.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0032.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0034.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0035.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0036.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0037.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mobster\script.0038.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test14_client0_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test14_client0_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test14_client1_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test14_client1_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test1_client0_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test1_client0_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test1_client1_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test1_client1_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test37_client0_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test37_client0_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test37_client1_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test37_client1_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test5_client0_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test5_client0_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test5_client1_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test5_client1_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test6_client0_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test6_client0_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test6_client1_recv.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mobster\test6_client1_send.btxt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_mncontroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0101.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0102.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0103.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\Mncontroller\script.0104.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.0105.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.0106.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.0107.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.1001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.1002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.1003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\mncontroller\script.1004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_service_framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\ServiceFramework\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_facontroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0101.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0102.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0103.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0200.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0204.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.0205.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.1001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\facontroller\script.1002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_hacontroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0101.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0102.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0103.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0200.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0202.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0205.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.0206.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.1001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hacontroller\script.1002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_pppcontroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0101.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0102.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0103.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0110.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0112.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0113.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.0114.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.1000.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.1003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\pppcontroller\script.1004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_hostexecute"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecute\script.0007.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_devicecontrolchannel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0010.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0011.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0014.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0015.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0016.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0017.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0018.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0019.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0031.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0032.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0033.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0034.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0035.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\script.0036.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test1.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test1.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test10.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test10.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test11.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test11.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test12.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test12.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test13.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test13.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test14.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test14.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test15.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test15.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test16.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test16.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test17.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test17.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test18.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test18.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test19.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test19.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test2.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test2.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test3.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test3.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test31.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test31.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test32.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test32.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test33.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test33.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test34.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test34.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test35.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test35.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test36.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test36.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test4.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test5.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test5.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test6.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test6.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test7.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test7.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test8.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test8.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test9.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\devicecontrolchannel\test9.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_uuservice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0007.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0008.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0009.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0010.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0011.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UUService\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_hostexecuteasync"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0007.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0008.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0009.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0010.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0011.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0012.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\hostexecuteasync\script.0013.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_usecasecontroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0007.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0008.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0009.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0010.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\UsecaseController\script.0011.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_setdeviceid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\sti.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\sti.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\sts.0001.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\setdeviceid\sts.0002.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ST_mobster_pppcontroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0005.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0006.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobster_pppcontroller\script.0007.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ST_ppp_session"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\script.0050.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\script.0051.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\script.0052.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_ppp_session\script.0053.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ST_mobile_ip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobile_ip\run_auto_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobile_ip\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobile_ip\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_mobile_ip\script.1001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ST_symbianos_ppp_session"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\dcomm.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\script.0101.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti.0003.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti.0004.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_chargen.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_chargen.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_chargen.0003.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_chargen.0004.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_chargen.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_echo.0003.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sti_echo.0004.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sts.0001.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sts.0002.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sts.0003.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_sessions\sts.0004.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ST_symbianos_ppp_session_md"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\commdb_typhoon_emulator.ne.B.cfg

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\run_manual_verification_tests_A.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\run_manual_verification_tests_B.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\script.0001a.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\script.0001b.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\script.0100.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\script.0101.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\sti.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\sti.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\sti_chargen.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\sti_chargen.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\sts.0001.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_symbianos_ppp_session_multi_device\sts.0002.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ST_uuservice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_uuservice\run_manual_verification_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\st_uuservice\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_lubbock_control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\script.0031.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\script.0032.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\script.0033.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\script.0034.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\script.0035.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\script.0036.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test31.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test31.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test32.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test32.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test33.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test33.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test34.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test34.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test35.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test35.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test36.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_control\test36.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "UT_lubbock_ppp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\run_manual_interaction_tests.bat

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\sti.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\sti.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\sti_chargen.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\sti_chargen.0002.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\sts.0001.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\lubbock_ppp\sts.0002.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\TestScripts\PR0094\readme.txt

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "SampleScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SampleScripts\script.0001.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SampleScripts\script.0002.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SampleScripts\script.0003.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SampleScripts\script.0004.ucd

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SampleScripts\sti.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SampleScripts\sti_chargen.0001.ini

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SampleScripts\sts.0001.script

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Service Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\hacontroller\hacontroller.x

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\pppcontroller\pppcontroller.x

!IF  "$(CFG)" == "UCCS - Win32 Release"

!ELSEIF  "$(CFG)" == "UCCS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\HOWTO_AddService.txt
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SCRIPT_Commands.txt
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ucc.rc
# End Source File
# End Target
# End Project

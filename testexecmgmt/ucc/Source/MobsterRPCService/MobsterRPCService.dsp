# Microsoft Developer Studio Project File - Name="MobsterRPCService" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MobsterRPCService - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MobsterRPCService.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MobsterRPCService.mak" CFG="MobsterRPCService - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MobsterRPCService - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MobsterRPCService - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MobsterRPCService"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MobsterRPCService - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MobsterRPCService - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\MobileTermination" /I "..\..\ExternalSource\modification_source\oncrpc\\" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libmt.lib libthread.lib ..\..\ExternalSource\binaries\oncrpcms.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\BinInternal"

!ENDIF 

# Begin Target

# Name "MobsterRPCService - Win32 Release"
# Name "MobsterRPCService - Win32 Debug"
# Begin Group "RPC Generated Files"

# PROP Default_Filter ""
# Begin Group "include - nomod"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\CComponentManager.h
# End Source File
# Begin Source File

SOURCE=.\mobster.h
# End Source File
# End Group
# Begin Group "modified"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSMobster.cpp
# End Source File
# Begin Source File

SOURCE=.\CSMobster.h
# End Source File
# Begin Source File

SOURCE=.\CSvcMobster.cpp
# End Source File
# Begin Source File

SOURCE=.\CSvcMobster.h
# End Source File
# End Group
# Begin Group "source - nomod"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mobster_svc.c
# End Source File
# Begin Source File

SOURCE=.\mobster_svc_stub_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\mobster_xdr.c
# End Source File
# End Group
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ThreadLibrary\CAThread.h
# End Source File
# Begin Source File

SOURCE=..\MobileTermination\CLog.h
# End Source File
# Begin Source File

SOURCE=..\MobileTermination\CPhone.h
# End Source File
# Begin Source File

SOURCE=.\mobster_client_server_protocol.h
# End Source File
# Begin Source File

SOURCE=..\include\penstd.x
# End Source File
# Begin Source File

SOURCE=..\SocketLibrary\socket_helper.h
# End Source File
# End Group
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mobster.x

!IF  "$(CFG)" == "MobsterRPCService - Win32 Release"

!ELSEIF  "$(CFG)" == "MobsterRPCService - Win32 Debug"

# Begin Custom Build
InputPath=.\mobster.x

"mobster.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\BuildTools\rpcgen -server -component_base -wrapstring mobster.x

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mobster_client_server_protocol.cpp
# End Source File
# End Group
# End Target
# End Project

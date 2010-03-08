# Microsoft Developer Studio Project File - Name="MNController" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MNController - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MNController.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MNController.mak" CFG="MNController - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MNController - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MNController - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MNController"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MNController - Win32 Release"

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

!ELSEIF  "$(CFG)" == "MNController - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\External\source_for_build\oncrpc" /I "..\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libproc.lib libdynconf.lib oncrpcms.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\BinInternal\\" /libpath:"..\..\ExternalSource\binaries"

!ENDIF 

# Begin Target

# Name "MNController - Win32 Release"
# Name "MNController - Win32 Debug"
# Begin Group "RPC Generated Files"

# PROP Default_Filter ""
# Begin Group "include - nomod"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mncontroller.h
# End Source File
# End Group
# Begin Group "modified"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSMncontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\CSMncontroller.h
# End Source File
# Begin Source File

SOURCE=.\CSvcMncontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\CSvcMncontroller.h
# End Source File
# End Group
# Begin Group "source - nomod"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mncontroller_svc.c
# End Source File
# Begin Source File

SOURCE=.\mncontroller_svc_stub_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\mncontroller_xdr.c
# End Source File
# End Group
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\DynamicsCommandWrapper\CDynamicsCommand.h
# End Source File
# Begin Source File

SOURCE=..\include\penstd.h
# End Source File
# Begin Source File

SOURCE=..\ProcessLibrary\proclib.h
# End Source File
# Begin Source File

SOURCE=..\include\standard_service_manager.h
# End Source File
# Begin Source File

SOURCE=..\include\standard_unix.h
# End Source File
# End Group
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\DynamicsCommandWrapper\CDynamicsCommand.cpp

!IF  "$(CFG)" == "MNController - Win32 Release"

!ELSEIF  "$(CFG)" == "MNController - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mncontroller.x

!IF  "$(CFG)" == "MNController - Win32 Release"

!ELSEIF  "$(CFG)" == "MNController - Win32 Debug"

# Begin Custom Build
InputPath=.\mncontroller.x

"mncontroller.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\BuildTools\rpcgen -server  -component_base -wrapstring mncontroller.x

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\HOWTO_add_new_methods.txt

!IF  "$(CFG)" == "MNController - Win32 Release"

!ELSEIF  "$(CFG)" == "MNController - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project

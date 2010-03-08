# Microsoft Developer Studio Project File - Name="GenericService" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GenericService - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GenericService.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GenericService.mak" CFG="GenericService - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GenericService - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GenericService - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GenericService - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./bin/Release"
# PROP Intermediate_Dir "./bin/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\testtoolsconn\oncrpc\source_for_build\oncrpc" /I "./inc" /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "GenericService - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./bin/Debug"
# PROP Intermediate_Dir "./bin/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\testtoolsconn\oncrpc\source_for_build\oncrpc" /I "./inc" /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "GenericService - Win32 Release"
# Name "GenericService - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\CCall.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CSGenericstub.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CSvcGenericstub.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenericStub_svc.c
# End Source File
# Begin Source File

SOURCE=.\src\GenericStub_svc_stub_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenericStub_xdr.c
# End Source File
# End Group
# Begin Group "inc"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\CCall.h
# End Source File
# Begin Source File

SOURCE=.\inc\CComponentManager.h
# End Source File
# Begin Source File

SOURCE=.\inc\CService.h
# End Source File
# Begin Source File

SOURCE=.\inc\CSGenericstub.h
# End Source File
# Begin Source File

SOURCE=.\inc\CSvcGenericstub.h
# End Source File
# Begin Source File

SOURCE=.\inc\GenericStub.h
# End Source File
# End Group
# Begin Group "libs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\testtoolsconn\oncrpc\binaries_for_build\oncrpcms.lib
# End Source File
# End Group
# End Target
# End Project

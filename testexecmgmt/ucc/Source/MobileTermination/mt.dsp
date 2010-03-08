# Microsoft Developer Studio Project File - Name="mt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mt.mak" CFG="mt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mt - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mt - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "mt"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mt - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\BinInternal\libmt.lib"

!ELSEIF  "$(CFG)" == "mt - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\BinInternal\libmt.lib"

!ENDIF 

# Begin Target

# Name "mt - Win32 Release"
# Name "mt - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CDatalinkNull.cpp
# End Source File
# Begin Source File

SOURCE=.\CDatalinkPacketise.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogPppFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\CPhone.cpp
# End Source File
# Begin Source File

SOURCE=.\CPppFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CTCPTEChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\CUDPAirInterface.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CDatalinkNull.h
# End Source File
# Begin Source File

SOURCE=.\CDatalinkPacketise.h
# End Source File
# Begin Source File

SOURCE=.\CLogPppFilter.h
# End Source File
# Begin Source File

SOURCE=.\CPhone.h
# End Source File
# Begin Source File

SOURCE=.\CPppFrame.h
# End Source File
# Begin Source File

SOURCE=.\CTCPTEChannel.h
# End Source File
# Begin Source File

SOURCE=.\CUDPAirInterface.h
# End Source File
# Begin Source File

SOURCE=.\MTInterfaces.h
# End Source File
# End Group
# Begin Group "Imported Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ThreadLibrary\CAThread.h
# End Source File
# Begin Source File

SOURCE=.\CLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CLog.h
# End Source File
# End Group
# End Target
# End Project

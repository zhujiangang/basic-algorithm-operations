# Microsoft Developer Studio Project File - Name="CodeCounter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CodeCounter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CodeCounter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CodeCounter.mak" CFG="CodeCounter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CodeCounter - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CodeCounter - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CodeCounter - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "CodeCounter - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "CodeCounter - Win32 Release"
# Name "CodeCounter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\include\FileParser.h
# End Source File
# Begin Source File

SOURCE=.\include\LangGrammar.h
# End Source File
# End Group
# Begin Group "impl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\impl\BaseLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\BaseLogger.h
# End Source File
# Begin Source File

SOURCE=.\impl\CFileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\CFileParser.h
# End Source File
# Begin Source File

SOURCE=.\impl\CPPFileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\CPPFileParser.h
# End Source File
# Begin Source File

SOURCE=.\impl\FileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\GenericFileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\GenericFileParser.h
# End Source File
# Begin Source File

SOURCE=.\impl\LangGrammar.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\MultiLangGrammar.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\MultiLangGrammar.h
# End Source File
# Begin Source File

SOURCE=.\impl\PLCFileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\PLCFileParser.h
# End Source File
# Begin Source File

SOURCE=.\impl\SingleLangGrammar.cpp
# End Source File
# Begin Source File

SOURCE=.\impl\SingleLangGrammar.h
# End Source File
# Begin Source File

SOURCE=.\impl\TinyVector.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project

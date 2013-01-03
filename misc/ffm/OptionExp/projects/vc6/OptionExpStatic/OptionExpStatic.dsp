# Microsoft Developer Studio Project File - Name="OptionExpStatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OptionExpStatic - Win32 UniDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OptionExpStatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OptionExpStatic.mak" CFG="OptionExpStatic - Win32 UniDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OptionExpStatic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "OptionExpStatic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "OptionExpStatic - Win32 UniDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "OptionExpStatic - Win32 UniRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OptionExpStatic - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\include" /I "..\..\..\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\OptionExpS.lib"

!ELSEIF  "$(CFG)" == "OptionExpStatic - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /I "..\..\..\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\OptionExpSD.lib"

!ELSEIF  "$(CFG)" == "OptionExpStatic - Win32 UniDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OptionExpStatic___Win32_UniDebug"
# PROP BASE Intermediate_Dir "OptionExpStatic___Win32_UniDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "obj\UniDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /I "..\..\..\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\OptionExpSUD.lib"

!ELSEIF  "$(CFG)" == "OptionExpStatic - Win32 UniRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "OptionExpStatic___Win32_UniRelease"
# PROP BASE Intermediate_Dir "OptionExpStatic___Win32_UniRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "obj\UniRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\include" /I "..\..\..\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"bin\OptionExpSU.lib"

!ENDIF 

# Begin Target

# Name "OptionExpStatic - Win32 Release"
# Name "OptionExpStatic - Win32 Debug"
# Name "OptionExpStatic - Win32 UniDebug"
# Name "OptionExpStatic - Win32 UniRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\CmdBuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\DefaultOptionExp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\DefaultOptionExpBuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\OptionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\OptionExp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\OptionExpDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\OptionExpTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\OptionExpUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\include\CmdBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\DefaultOptionExp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\DefaultOptionExpBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OptionContext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OptionExp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OptionExpDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OptionExpTree.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\OptionExpUtils.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="FFProfile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FFProfile - Win32 UniDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FFProfile.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FFProfile.mak" CFG="FFProfile - Win32 UniDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FFProfile - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FFProfile - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "FFProfile - Win32 UniDebug" (based on "Win32 (x86) Application")
!MESSAGE "FFProfile - Win32 UniRelease" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FFProfile - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\cosps\tinyxml_2_5_3" /I "..\OptionExp\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Shlwapi.lib tinyxml.lib cflbase.lib OptionExpS.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\..\..\cosps\tinyxml_2_5_3\Release" /libpath:"..\OptionExp\projects\vc6\OptionExpStatic\bin"

!ELSEIF  "$(CFG)" == "FFProfile - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\OptionExp\include" /I "..\..\..\..\cosps\tinyxml_2_5_3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Shlwapi.lib tinyxmld.lib cflbaseD.lib OptionExpSD.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\cosps\tinyxml_2_5_3\Debug" /libpath:"..\OptionExp\projects\vc6\OptionExpStatic\bin"

!ELSEIF  "$(CFG)" == "FFProfile - Win32 UniDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FFProfile___Win32_UniDebug"
# PROP BASE Intermediate_Dir "FFProfile___Win32_UniDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UniDebug"
# PROP Intermediate_Dir "UniDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\cosps\tinyxml_2_5_3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CFLBASE_STATIC" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\cosps\tinyxml_2_5_3" /I "..\OptionExp\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Shlwapi.lib tinyxmld.lib cflbaseSD.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\cosps\tinyxml_2_5_3\Debug"
# ADD LINK32 Shlwapi.lib tinyxmld.lib cflbaseUD.lib OptionExpSUD.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\cosps\tinyxml_2_5_3\Debug" /libpath:"..\OptionExp\projects\vc6\OptionExpStatic\bin"

!ELSEIF  "$(CFG)" == "FFProfile - Win32 UniRelease"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FFProfile___Win32_UniRelease"
# PROP BASE Intermediate_Dir "FFProfile___Win32_UniRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UniRelease"
# PROP Intermediate_Dir "UniRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\cosps\tinyxml_2_5_3" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\cosps\tinyxml_2_5_3" /I "..\OptionExp\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Shlwapi.lib tinyxml.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\..\..\cosps\tinyxml_2_5_3\Release"
# ADD LINK32 Shlwapi.lib tinyxml.lib cflbaseU.lib OptionExpSU.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /libpath:"..\..\..\..\cosps\tinyxml_2_5_3\Release" /libpath:"..\OptionExp\projects\vc6\OptionExpStatic\bin"

!ENDIF 

# Begin Target

# Name "FFProfile - Win32 Release"
# Name "FFProfile - Win32 Debug"
# Name "FFProfile - Win32 UniDebug"
# Name "FFProfile - Win32 UniRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FFProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\FFProfile.rc
# End Source File
# Begin Source File

SOURCE=.\FFProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MiscUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\PropListMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysConf.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FFProfile.h
# End Source File
# Begin Source File

SOURCE=.\FFProfileDlg.h
# End Source File
# Begin Source File

SOURCE=.\MiscUtils.h
# End Source File
# Begin Source File

SOURCE=.\PropListMgr.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysConf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FFProfile.ico
# End Source File
# Begin Source File

SOURCE=.\res\FFProfile.rc2
# End Source File
# End Group
# End Target
# End Project

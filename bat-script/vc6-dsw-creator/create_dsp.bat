@echo off

set PROJECT_NAME=%1

echo # Microsoft Developer Studio Project File - Name="%PROJECT_NAME%" - Package Owner=^<4^>
echo # Microsoft Developer Studio Generated Build File, Format Version 6.00
echo # ** DO NOT EDIT **
echo.
echo # TARGTYPE "Win32 (x86) Application" 0x0101
echo.
echo CFG=%PROJECT_NAME% - Win32 Debug
echo !MESSAGE This is not a valid makefile. To build this project using NMAKE,
echo !MESSAGE use the Export Makefile command and run
echo !MESSAGE 
echo !MESSAGE NMAKE /f "%PROJECT_NAME%.mak".
echo !MESSAGE 
echo !MESSAGE You can specify a configuration when running NMAKE
echo !MESSAGE by defining the macro CFG on the command line. For example:
echo !MESSAGE 
echo !MESSAGE NMAKE /f "%PROJECT_NAME%.mak" CFG="%PROJECT_NAME% - Win32 Debug"
echo !MESSAGE 
echo !MESSAGE Possible choices for configuration are:
echo !MESSAGE 
echo !MESSAGE "%PROJECT_NAME% - Win32 Release" (based on "Win32 (x86) Application")
echo !MESSAGE "%PROJECT_NAME% - Win32 Debug" (based on "Win32 (x86) Application")
echo !MESSAGE 
echo.
echo # Begin Project
echo # PROP AllowPerConfigDependencies 0
echo # PROP Scc_ProjName ""
echo # PROP Scc_LocalPath ""
echo CPP=cl.exe
echo MTL=midl.exe
echo RSC=rc.exe
echo.
echo !IF  "$(CFG)" == "%PROJECT_NAME% - Win32 Release"
echo.
echo # PROP BASE Use_MFC 0
echo # PROP BASE Use_Debug_Libraries 0
echo # PROP BASE Output_Dir "Release"
echo # PROP BASE Intermediate_Dir "Release"
echo # PROP BASE Target_Dir ""
echo # PROP Use_MFC 0
echo # PROP Use_Debug_Libraries 0
echo # PROP Output_Dir "Release"
echo # PROP Intermediate_Dir "Release"
echo # PROP Target_Dir ""
echo # ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
echo # ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
echo # ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
echo # ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
echo # ADD BASE RSC /l 0x409 /d "NDEBUG"
echo # ADD RSC /l 0x409 /d "NDEBUG"
echo BSC32=bscmake.exe
echo # ADD BASE BSC32 /nologo
echo # ADD BSC32 /nologo
echo LINK32=link.exe
echo # ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
echo # ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
echo.
echo !ELSEIF  "$(CFG)" == "%PROJECT_NAME% - Win32 Debug"
echo.
echo # PROP BASE Use_MFC 0
echo # PROP BASE Use_Debug_Libraries 1
echo # PROP BASE Output_Dir "Debug"
echo # PROP BASE Intermediate_Dir "Debug"
echo # PROP BASE Target_Dir ""
echo # PROP Use_MFC 0
echo # PROP Use_Debug_Libraries 1
echo # PROP Output_Dir "Debug"
echo # PROP Intermediate_Dir "Debug"
echo # PROP Target_Dir ""
echo # ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
echo # ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
echo # ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
echo # ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
echo # ADD BASE RSC /l 0x409 /d "_DEBUG"
echo # ADD RSC /l 0x409 /d "_DEBUG"
echo BSC32=bscmake.exe
echo # ADD BASE BSC32 /nologo
echo # ADD BSC32 /nologo
echo LINK32=link.exe
echo # ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
echo # ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
echo.
echo !ENDIF 
echo.
echo # Begin Target
echo.
echo # Name "%PROJECT_NAME% - Win32 Release"
echo # Name "%PROJECT_NAME% - Win32 Debug"
echo # Begin Group "Source Files"
echo.
echo # PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"

for /f "delims=" %%i in ('dir /b *.cpp *.c *.rc') do (
	echo # Begin Source File
	echo.
	echo SOURCE=.\%%i
	echo # End Source File
)

echo # End Group
echo # Begin Group "Header Files"
echo.
echo # PROP Default_Filter "h;hpp;hxx;hm;inl"

for /f "delims=" %%i in ('dir /b *.h *.hpp *.hxx') do (
	echo # Begin Source File
	echo.
	echo SOURCE=.\%%i
	echo # End Source File
)

echo # End Group
echo # Begin Group "Resource Files"
echo.
echo # PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"

for /f "delims=" %%i in ('dir /b *.ico *.cur *.bmp *.rc2 *.gif *.jpg *.jpeg') do (
	echo # Begin Source File
	echo.
	echo SOURCE=.\%%i
	echo # End Source File
)

echo # End Group
echo # End Target
echo # End Project
echo.


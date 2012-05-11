@echo off
@echo Setting environment for using Microsoft Visual Studio 2010 x86 tools.


set WindowsSdkDir=c:\Program Files\Microsoft SDKs\Windows\v7.0A\

@set FrameworkDir=c:\WINDOWS\Microsoft.NET\Framework\
@set FrameworkVersion=v4.0.30319
@set Framework35Version=v3.5

@rem PATH
set PATH=%VS10_HOME%\VSTSDB\Deploy;%VS10_HOME%\Common7\IDE;%VS10_HOME%\VC\BIN;%VS10_HOME%\Common7\Tools;%FrameworkDir%%FrameworkVersion%;%FrameworkDir%%Framework35Version%;%VS10_HOME%\VC\VCPackages;%ProgramFiles(x86)%\HTML Help Workshop;%ProgramFiles%\HTML Help Workshop;%VS10_HOME%\Team Tools\Performance Tools;%WindowsSdkDir%bin\NETFX 4.0 Tools;%WindowsSdkDir%bin;%PATH%

@rem INCLUDE
set INCLUDE=%VS10_HOME%\VC\INCLUDE;%VS10_HOME%\VC\ATLMFC\INCLUDE;%WindowsSdkDir%include;%INCLUDE%

@rem LIB
set LIB=%VS10_HOME%\VC\LIB;%VS10_HOME%\VC\ATLMFC\LIB;%WindowsSdkDir%lib;%LIB%

@rem LIBPATH
set LIBPATH=%FrameworkDir%%FrameworkVersion%;%FrameworkDir%%Framework35Version%;%VS10_HOME%\VC\LIB;%VS10_HOME%\VC\ATLMFC\LIB;%LIBPATH%

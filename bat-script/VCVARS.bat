@echo off

set VS_VERSION=6
if not "%1" == "" set VS_VERSION=%1

if "%VS_VERSION%"=="6" goto VS6
if "%VS_VERSION%"=="8" goto VS8
if "%VS_VERSION%"=="10" goto VS10

echo Invalid version number: VS_VERSION=%VS_VERSION%
goto VS_END

:VS6
set VS6_HOME=%VS6_HOME:\=/%
call "%VS6_HOME%/VC98/Bin/VCVARS32.BAT"
goto VS_END

:VS8
set VS8_HOME=%VS8_HOME:\=/%
call "%VS8_HOME%/Common7/Tools/vsvars32.bat"
goto VS_END

:VS10
set VS10_HOME=%VS10_HOME:\=/%
call "%VS10_HOME%/VC/vcvarsall.bat"
goto VS_END


:VS_END

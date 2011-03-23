REM Append the following code to content of file /ect/profile
REM for i in /etc/profile.t/*.sh ; do
REM  if [ -f $i ]; then
REM    . $i
REM  fi
REM done

REM rm -f /etc/profile.t/*.sh

@echo off

set PARAM=%1

set DRIVE=
set PATH_NO_DRIVE=

FOR /F "tokens=1,2* delims=:" %%i in (%PARAM%) do (
  set DRIVE=%%i
  set PATH_NO_DRIVE=%%j
)

set DRIVE=/%DRIVE%

REM invalid PATH_NO_DRIVE
if "%PATH_NO_DRIVE%"=="" goto FINISH
if "%PATH_NO_DRIVE%"=="\" (
  set PATH_NO_DRIVE=
  goto FINISH
)
if "%PATH_NO_DRIVE%"=="/" (
  set PATH_NO_DRIVE=
  goto FINISH
)

REM trim the last "\" or "/" character
if "%PATH_NO_DRIVE:~-1%"=="\" set PATH_NO_DRIVE=%PATH_NO_DRIVE:~,-1%
if "%PATH_NO_DRIVE:~-1%"=="/" set PATH_NO_DRIVE=%PATH_NO_DRIVE:~,-1%
set PATH_NO_DRIVE=%PATH_NO_DRIVE:\=/%


:FINISH
set TRANSIENT_SHELL_FILE=%MSYS_HOME%\etc\profile.t\transient______xxxxx.sh
echo #! /bin/sh >%TRANSIENT_SHELL_FILE%
echo cd "%DRIVE%%PATH_NO_DRIVE%" >>%TRANSIENT_SHELL_FILE%

cd %MSYS_HOME%\bin
call %MSYS_HOME%\msys.bat -rxvt

@echo off

set PARAM=%1
if '%1'=='' ( 
  set PARAM=%~dp0
)
REM delete the character "
set PARAM=%PARAM:"=%

echo %PARAM%

set DRIVE=
set PATH_NO_DRIVE=

FOR /F "tokens=1,2* delims=:" %%i in ("%PARAM%") do (
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

REM replace the "\" with "/"
set PATH_NO_DRIVE=%PATH_NO_DRIVE:\=/%

:FINISH
echo "%DRIVE%%PATH_NO_DRIVE%"

pause

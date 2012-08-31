@echo off
setlocal ENABLEDELAYEDEXPANSION
IF ERRORLEVEL 1 (
    echo Unable to enable DELAYED EXPANSION
    goto END
)

IF "%1"=="" (
  ECHO empty paramters.
  GOTO END
)

set WORK_DIR=%1
shift
set ACTION=%1
shift
set SRC_EXT=%1
shift
set PARAM=%1 %2 %3 %4 %5 %6 %7 %8 %9
echo PARAM=%PARAM%

set CUR_DIR=%CD%

cd /d %WORK_DIR%


set count=0
for /f "delims=" %%i in ('dir /b /a-d /od *.%SRC_EXT%') do (
	echo (!count!^) %ACTION% "%%i" %PARAM%
	call %ACTION% "%%i" %PARAM%
	set /A count+=1
)

cd /d %CUR_DIR%

:END
pause
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
set ACTION=%2
set SRC_EXT=%3
set DST_EXT=%4

set CUR_DIR=%CD%

cd /d %WORK_DIR%


set count=0
set add0_count=0
for /f "delims=" %%i in ('dir /b /a-d /od *.%SRC_EXT%') do (
  echo.
	echo (!count!^) conver "%%i" to "%%~ni.%DST_EXT%" ...
	echo.
	call %ACTION% "%%i" "%%~ni.%DST_EXT%"
	set /A count+=1
)

cd /d %CUR_DIR%

:END
pause
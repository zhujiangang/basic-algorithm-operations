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
set FILE_EXT=%2
set FILE_PREFIX=%3

set CUR_DIR=%CD%

cd /d %WORK_DIR%

set count=0
set add0_count=0
for /f "delims=" %%i in ('dir /b /a-d /od *.%FILE_EXT%') do (
	echo (!count!^) %%i
	
	for /f "delims=" %%j in ('add0.bat !count!') do (
		set add0_count=%%j
	)
	echo rename "%%i" to "%FILE_PREFIX%!add0_count!.%FILE_EXT%"
	rename "%%i" "%FILE_PREFIX%!add0_count!.%FILE_EXT%"

	set /A count+=1
)

cd /d %CUR_DIR%

:END
pause
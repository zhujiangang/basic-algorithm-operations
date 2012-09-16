@echo off

set FILE_LIST=%~1
set PROG=%~2

if "%FILE_LIST%"=="" (
  echo FILE_LIST not set
  goto END
)

if "%PROG%"=="" (
  echo PROG not set
  goto END
)

for /f "delims=" %%i in (%FILE_LIST%) do (
	echo.
	echo %%i
	call "%PROG%" "%%i"
)

:END
pause
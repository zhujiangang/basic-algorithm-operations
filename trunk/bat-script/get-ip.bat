@echo off

set TMP_FILE=ip.tmp

ipconfig | findstr /C:"IP Address" >%TMP_FILE%

set TMPSTR=

FOR /F "tokens=2 delims=:" %%i in (%TMP_FILE%) do (
  set TMPSTR=%%i
)

set IPSTR=
FOR /F "tokens=1,2 delims= " %%i in ("%TMPSTR%") do (
  set IPSTR=%%i
)

del /F /Q %TMP_FILE%

echo %IPSTR%

pause

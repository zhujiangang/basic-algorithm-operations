@echo off

set WORK_DIR=%cd%
set TEST_FILE=%~dp0%test.txt

if not exist %TEST_FILE% (
  echo The test file %TEST_FILE% doesn't exist.
  goto END
)

set DST_DIR=C:\Temp

cd /d %DST_DIR%

for /F %%i in (%TEST_FILE%) do (
  if not exist %%~dpi mkdir %%~dpi
)

cd /d %WORK_DIR%

:END
pause

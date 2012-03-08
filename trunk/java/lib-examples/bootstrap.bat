@ECHO off

set ARGV0=%~dp0
:TRIM_LAST_SLASH_A
if not "%ARGV0:~-1%"=="\" (
  goto TRIM_LAST_SLASH_Z
) else (
  set ARGV0=%ARGV0:~,-1%
  goto TRIM_LAST_SLASH_A
)
:TRIM_LAST_SLASH_Z
SET THE_BOOTSTRAP_DIR=%ARGV0%

SET CUR_DIR=%CD%
SET WORK_DIR=%THE_BOOTSTRAP_DIR%

CD /d "%WORK_DIR%"

SET LOCALCLASSPATH=%WORK_DIR%\conf\usr;%WORK_DIR%\conf\default
for %%i in (%WORK_DIR%\boot\*.jar) do call %WORK_DIR%\bin\lcp.bat %%~fi
for %%i in (%WORK_DIR%\lib\*.jar) do call %WORK_DIR%\bin\lcp.bat %%~fi
echo %LOCALCLASSPATH%

if not "%*"=="" (
  set JAVA_PARAMETERS=%*
) else (
  set JAVA_PARAMETERS=com.bao.examples.App
)

java -cp %LOCALCLASSPATH% %JAVA_PARAMETERS%

:END
CD /d "%CUR_DIR%"
pause

@echo off

@rem enable delay expansion
setlocal ENABLEDELAYEDEXPANSION
if ERRORLEVEL 1 (
    echo Unable to enable DELAYED EXPANSION
    goto end
)

@rem remove the trailing \
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

@rem set work dirs
SET CUR_DIR=%CD%
SET WORK_DIR=%THE_BOOTSTRAP_DIR%
CD /d "%WORK_DIR%"

set LOCALCLASSPATH=%WORK_DIR%\conf\usr;%WORK_DIR%\conf\default
for /f "delims=" %%i in ('dir /b /a-d /od /s "%WORK_DIR%\boot\*.jar"') do set LOCALCLASSPATH=!LOCALCLASSPATH!;%%i
for /f "delims=" %%i in ('dir /b /a-d /od /s "%WORK_DIR%\lib\*.jar"') do set LOCALCLASSPATH=!LOCALCLASSPATH!;%%i
echo %LOCALCLASSPATH%

if not "%*"=="" (
  set JAVA_PARAMETERS=%*
) else (
  set JAVA_PARAMETERS=com.bao.lc.App
)

java -cp "%LOCALCLASSPATH%" %JAVA_PARAMETERS%

:END
CD /d "%CUR_DIR%"

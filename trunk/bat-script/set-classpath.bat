@echo off
setlocal ENABLEDELAYEDEXPANSION
IF ERRORLEVEL 1 (
    echo Unable to enable DELAYED EXPANSION
    goto end
)

set JAR_LIBS=.
for /f "delims=" %%i in ('dir /b /a-d /od /s .\lib\*.jar') do set JAR_LIBS=!JAR_LIBS!;%%i

set classpath=%JAR_LIBS%;%classpath%

echo %classpath%

pause

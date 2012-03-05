@ECHO off

SET THE_BOOTSTRAP_DIR=%~dp0
SET CUR_DIR=%CD%
SET WORK_DIR=%THE_BOOTSTRAP_DIR%

CD /d %WORK_DIR%

SET LOCALCLASSPATH=%WORK_DIR%\conf\usr;%WORK_DIR%\conf\default
for %%i in (%WORK_DIR%\boot\*.jar) do call %WORK_DIR%\bin\lcp.bat %%~fi
for %%i in (%WORK_DIR%\lib\*.jar) do call %WORK_DIR%\bin\lcp.bat %%~fi
echo %LOCALCLASSPATH%


java -cp %LOCALCLASSPATH% com.bao.examples.App


:END
CD /d %CUR_DIR%
pause
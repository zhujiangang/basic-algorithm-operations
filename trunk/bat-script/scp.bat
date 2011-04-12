@echo off

REM this tool is used to synchronize the class path file .classpath
set DST=temp.classpath 
set BASE_DIR=.\java\lib 

REM The following code should not be changed
set SRC=%~dp0sources\project.classpath

if not exist %SRC% (
  echo The source class path file "%SRC%" does NOT exist.
  goto END
)

REM make the file to be writable
if exist %DST% attrib -R %DST%

set CLASSPATH=

for /f "delims=" %%i in (%SRC%) do (
	set CLASSPATH=%%i
)

REM echo CLASSPATH=%CLASSPATH%

REM file header
echo ^<?xml version="1.0" encoding="UTF-8"?^> >%DST%
echo ^<classpath^> >>%DST%
echo   ^<classpathentry kind="src" path="sources"/^> >>%DST%
echo   ^<classpathentry kind="con" path="org.eclipse.jdt.launching.JRE_CONTAINER"/^> >>%DST%

set temp_path=
:PARSE_CLASS_PATH_LOOP
FOR /F "tokens=1,* delims= " %%i in ("%CLASSPATH%") do (
  set temp_path=%BASE_DIR%\%%i
  set CLASSPATH=%%j
)

REM convert relative path to absolute path
FOR /F "delims=" %%i in ("%temp_path%") do (
  set temp_path=%%~fi
) 

set temp_path=%temp_path:\=/%
echo %temp_path%
echo   ^<classpathentry kind="lib" path="%temp_path%"/^> >>%DST%

if not "x%CLASSPATH%x"=="xx" goto PARSE_CLASS_PATH_LOOP

echo   ^<classpathentry kind="output" path="bin"/^> >>%DST%
echo ^</classpath^> >>%DST%

echo.
echo.
echo DONE...
echo.


:END

pause

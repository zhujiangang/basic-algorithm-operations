@echo off

set WORKSPACE_NAME=TempByBat

call create_dsw.bat %WORKSPACE_NAME% >%WORKSPACE_NAME%.dsw

pause

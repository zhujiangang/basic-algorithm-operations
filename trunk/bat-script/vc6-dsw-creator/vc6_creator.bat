@echo off

set WORKSPACE_NAME=TempByBat

call create_dsp.bat %WORKSPACE_NAME% >%WORKSPACE_NAME%.dsp
call create_dsw.bat %WORKSPACE_NAME% >%WORKSPACE_NAME%.dsw

pause
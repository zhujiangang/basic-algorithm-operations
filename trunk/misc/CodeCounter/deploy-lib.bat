@echo off

set DST_DIR=..\..\..\cosps\ListCtrl\ListCtrlDemo\Lib
set SRC_DIR=.\Debug

if exist "%SRC_DIR%\CodeCounter.lib" (
	copy /Y /B "%SRC_DIR%\CodeCounter.lib" "%DST_DIR%\SCC.lib"
)

pause

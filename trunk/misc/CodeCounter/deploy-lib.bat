@echo off

set DST_DIR=..\..\..\cosps\ListCtrl\ListCtrlDemo

if exist ".\Debug\CodeCounter.lib" (
	copy /Y /B ".\Debug\CodeCounter.lib" "%DST_DIR%\Lib\SCCd.lib"
)
if exist ".\Release\CodeCounter.lib" (
	copy /Y /B ".\Release\CodeCounter.lib" "%DST_DIR%\Lib\SCC.lib"
)

xcopy /Y ".\include" "%DST_DIR%\Include"

pause

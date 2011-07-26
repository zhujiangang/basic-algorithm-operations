@echo off

set DST_DIR=..\..\..\..\cosps\ListCtrl\ListCtrlDemo

if exist ".\Debug\TinyCrypto.lib" (
	copy /Y /B ".\Debug\TinyCrypto.lib" "%DST_DIR%\Lib\TCd.lib"
)
if exist ".\Release\TinyCrypto.lib" (
	copy /Y /B ".\Release\TinyCrypto.lib" "%DST_DIR%\Lib\TC.lib"
)

copy /Y /B ".\src\*.h" "%DST_DIR%\Include"

pause

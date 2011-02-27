@echo off
setlocal ENABLEDELAYEDEXPANSION
IF ERRORLEVEL 1 (
    echo Unable to enable DELAYED EXPANSION
    goto END
)

set count=0
set add0_count=0
for /f "delims=" %%i in ('dir /b /a-d /od .\jpgs\*.jpg') do (
	echo (!count!^) %%i
	
	for /f "delims=" %%j in ('add0.bat !count!') do (
		set add0_count=%%j
	)
	echo rename ".\jpgs\%%i" to "xyz!add0_count!.jpg"
	rename .\jpgs\%%i xyz!add0_count!.jpg

	set /A count+=1
)

:END
pause
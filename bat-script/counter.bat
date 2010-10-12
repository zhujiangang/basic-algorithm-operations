@echo off
setlocal ENABLEDELAYEDEXPANSION
IF ERRORLEVEL 1 (
    echo Unable to enable DELAYED EXPANSION
    goto END
)

set count=0
for /f "delims=" %%i in ('dir /b /a-d /od /s *') do (
	set /A count+=1
	echo (!count!^) %%i
)

:END
pause

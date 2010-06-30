@echo off

FOR /F "tokens=1,2,3,4* delims=:" %%i in ("IP Address. . . . . . . . . . . . : 192.168.127.26") do @echo 1%%i!2%%j!3%%k!4%%l!5%%m
FOR /F "tokens=1,2,3 delims= " %%i in (" 192.168.127.26  ") do @echo !%%i!%%j!%%k!

rem FOR /F "tokens=1,2,3,4* delims=:" %%i in (myfile.txt) do @echo 1%%i!2%%j!3%%k!4%%l!5%%m


pause

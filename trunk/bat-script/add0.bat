@ECHO OFF
SET result=%1

IF /I %1 LSS 1000 (
  SET result=0%1
) ELSE (
	SET result=%1
)

IF /I %1 LSS 100 SET result=00%1
IF /I %1 LSS 10 SET result=000%1

ECHO %result%
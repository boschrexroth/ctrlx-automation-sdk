@echo OFF

IF "%~1" == "" GOTO ERR
SET Destination=%1%

IF "%~2" == "" GOTO ERR
SET FileList=%2%

SET Source=.\

FOR /F "USEBACKQ TOKENS=*" %%F IN ("%FileList%") DO XCOPY /F /Y "%Source%\%%~F" "%Destination%\"

exit 0 /B


:ERR
echo Parameter is empty
exit 1 /B


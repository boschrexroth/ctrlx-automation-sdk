::Author: marcmaro (DC-IA/ESW1)
@echo off

cd /D "%~dp0"

call npm install
if %ERRORLEVEL% NEQ 0 (	
	goto error
)

call npm prune
if %ERRORLEVEL% NEQ 0 (	
	goto error
)

::--------------------------------------
:: ERROR HANDLING
::--------------------------------------
::ECHO %ERRORLEVEL%
:success
	color A0
	exit /B 0
	
:error
	color C0
	pause
	exit /B %ERRORLEVEL%
@echo off

set QEMU_IMG="C:\Program Files\qemu\qemu-img.exe" 
set ORIGINAL=ubuntu-24.04-server-cloudimg-amd64.qcow2
set SHRINKED=SHRINKED.qcow2

echo.
echo -----------------------------------------------------------------
echo Shrinking App Build Enviromment file:
dir /B ubuntu*.qcow2
echo -----------------------------------------------------------------

if not exist %QEMU_IMG% (
	echo ERROR %QEMU_IMG% "doesn't exist!"
	echo "Please change the variable QEMU_IMG of this bat file!"
	goto ERROR
)

echo This process can take a while!
echo Continue with ENTER, break with Ctrl-C
pause

%QEMU_IMG% convert -p -c %ORIGINAL% -O qcow2 %SHRINKED%
if %ERRORLEVEL% NEQ 0 (	
	goto ERROR
)

if exist %ORIGINAL%.bak (
	del %ORIGINAL%.bak
)

ren %ORIGINAL% %ORIGINAL%.bak
if %ERRORLEVEL% NEQ 0 (	
	goto ERROR
)

ren %SHRINKED% %ORIGINAL%
if %ERRORLEVEL% NEQ 0 (	
	goto ERROR
)

:OK
echo.
echo Converting %ORIGINAL% sucessfully
pause
exit 0

:ERROR
echo.
echo Converting %ORIGINAL% finshed with error %ERRORLEVEL%
pause
exit %ERRORLEVEL%
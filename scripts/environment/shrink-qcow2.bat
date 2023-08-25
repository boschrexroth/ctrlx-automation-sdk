@echo off

echo.
echo App Build Enviromment file:
dir /B ubuntu*.qcow2
echo.
echo -----------------------------------------------------------------
echo This process can take a while.
echo In rare cases, the reduced file may be corrupt.
echo Therefore we recommend saving the original file in a zip archive.
echo -----------------------------------------------------------------
echo.
echo Continue with ENTER, break with Ctrl-C
pause

set SHRINKED=SHRINKED_IZqkx4cTqamJikhsXfq5.qcow2
if exist %SHRINKED% (del %SHRINKED%)

for %%i in (ubuntu*.qcow2) do (	
	echo.
	echo Shrinking %%i
	time /t
	qemu-img convert -p -c %%i -O qcow2 %SHRINKED%
	del "%%i"
	if %ERRORLEVEL% NEQ 0 goto ERROR
	ren %SHRINKED% "%%i"
	echo Completed 
	time /t
)

:OK
echo Finished sucessfully
pause
exit 0

:ERROR
echo Converting %%i finshed with error %ERRORLEVEL%
pause
exit %ERRORLEVEL%
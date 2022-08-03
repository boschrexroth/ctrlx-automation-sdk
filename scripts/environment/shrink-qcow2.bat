@echo off

dir /B *.qcow2
echo Start shrinking?
pause

set SHRINKED=SHRINKED_IZqkx4cTqamJikhsXfq5.qcow2
if exist %SHRINKED% (del %SHRINKED%)

for %%i in (*.qcow2) do (	
	echo %time% Shrinking "%%i" ...
	qemu-img convert -c "%%i" -O qcow2 %SHRINKED%
	del "%%i"
	ren %SHRINKED% "%%i"
	echo %time% done
)

dir /B *.qcow2
echo Finished
pause


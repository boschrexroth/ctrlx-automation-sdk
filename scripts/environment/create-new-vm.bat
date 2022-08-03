@echo OFF

rem %1 amd | aarch64
rem %2 proxy | noproxy
rem %3 Destination directory

set ARCH=%~1
If not defined ARCH (set ARCH=amd64)

set PROXY=%~2
If not defined PROXY (set PROXY=proxy)

set DIR=%~3
If not defined DIR (set DIR=%ARCH%-%PROXY%)

(mkdir %DIR%)

(xcopy /Y README.md %DIR%\)
(xcopy /Y build.bat %DIR%\)
(xcopy /Y launch.bat %DIR%\) 
(xcopy /Y wget.* %DIR%\)


(xcopy /Y launch-%ARCH%-%PROXY%.bat %DIR%\)
(xcopy /Y ubuntu-20.04-server-cloudimg-%ARCH%-user-data-%PROXY%.img %DIR%\)

@echo OFF

rem %1 amd | aarch64
rem %2 proxy | noproxy
rem %3 Destination directory

SET ARCH=%1
IF DEFINED ARCH GOTO LPROXY
SET ARCH=amd64

:LPROXY
SET PROXY=%2
IF DEFINED PROXY GOTO LDIR
SET PROXY=proxy

:LDIR
SET DIR=%3
IF DEFINED DIR GOTO LCHECK
SET DIR=qemu-vm-%ARCH%-%PROXY%

:LCHECK
IF EXIST %DIR%\NUL GOTO LEXISTS

mkdir %DIR%

xcopy README.md %DIR%\
xcopy build.bat %DIR%\ 
xcopy launch.bat %DIR%\ 
xcopy wget.bat %DIR%\

xcopy launch-%ARCH%-%PROXY%.bat %DIR%\
xcopy ubuntu-20.04-server-cloudimg-%ARCH%-user-data-%PROXY%.img %DIR%\

exit 0 /B

:LEXISTS
echo Destination directory %DIR% exists!

@echo off

REM Remove 'REM' in the next two lines if you are using Px.exe as proxy server
REM set https_proxy=localhost:3128
REM set http_proxy=localhost:3128

set OS=ubuntu-20.04-server

set ARCH=amd64
set PROXY=proxy

set AR=ar
set NO=no

rem https://stackoverflow.com/questions/7005951/batch-file-find-if-substring-is-in-string-not-in-a-file
for %%x in (%*) do (
    echo %%x
    echo %%x | FINDSTR /C:%AR% >nul && ( set ARCH=arm64)    
    echo %%x | FINDSTR /C:%NO% >nul && ( set PROXY=noproxy)    
)

:: Check cloud image file ---------------------------------------------------
set U=%OS%-cloudimg
set UA=%U%-%ARCH%
set IMG=%UA%.img
set QCOW2=%UA%-snapshot-%PROXY%.qcow2
set UDIMG=%UA%-user-data-%PROXY%.img

if not exist %IMG% (
    call wget.bat https://cloud-images.ubuntu.com/releases/focal/release/%IMG% %IMG%
)

if not exist %IMG% (
    echo ERROR: File '%IMG%' is missing 
    echo INFO: Download this file from 'https://cloud-images.ubuntu.com/releases/focal/release' and copy it into this directory
    exit /B 1
)

:: Create snapshot file --------------------------------------------------------
if not exist %QCOW2% (
    qemu-img create -b %IMG% -f qcow2 %QCOW2% -F qcow2 1T
)

:: Check user data image file -------------------------------------------------
if not exist %UDIMG% (
    echo ERROR: File '%UDIMG%' is missing 
    echo INFO: Copy this file from your SDK zip archive into this directory
    exit /B 1
)

exit /B 0

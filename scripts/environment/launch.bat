@echo off

rem Parameters
rem %1 CPU Architecture Guest System: amd64 (default), aarch64
rem %2 Proxy usage: proxy (default), no-proxy

rem Important: 
rem Enclose these parameter values with quotation marks otherwise they will be splitted.
rem %3 Port Forwarding: "hostfwd=tcp::10022-:22,hostfwd=tcp::8000-:8000,hostfwd=...."
rem %4 Acceleration: "whpx:hax:tcg"

set ARCH=%~1
If not defined ARCH (set ARCH=amd64)

set PROXY=%~2
If not defined PROXY (set PROXY=proxy)

set PFW=%~3
If not defined PFW (set PFW=hostfwd=tcp::10022-:22)

set NAME=%~4
If not defined NAME (set NAME=App-Build-Environment)

set ACCEL=%~5
If not defined ACCEL (set ACCEL=whpx:hax:tcg)

set ACTIVITY="Initializing, please wait - restart if finshed"
if exist *.qcow2 (set ACTIVITY= )

TITLE %NAME% %ACTIVITY% %CD% %ARCH% %PFW%

call build.bat %ARCH% %PROXY%

REM wget.exe changes the title
TITLE %NAME% %ACTIVITY% %CD% %ARCH% %PFW%

REM Enable ESC sequences for enhanced text output in console - needs no admin rights
REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1

IF "%ARCH%" == "amd64" (
    qemu-system-x86_64.exe ^
	-name "%NAME%" ^
    -machine q35,accel=%ACCEL% ^
    -smp 4 ^
    -m 4G ^
    -netdev user,id=eth0,%PFW% ^
    -device virtio-net-pci,netdev=eth0,mac=DE-AD-BE-00-00-01 ^
    -drive "file=%QCOW2%,format=qcow2" ^
    -drive "file=%UDIMG%,format=raw" ^
    -display none ^
    -nographic

    exit /B 0
)

if not exist QEMU_EFI.fd (
    call wget.bat https://releases.linaro.org/components/kernel/uefi-linaro/16.02/release/qemu64/QEMU_EFI.fd QEMU_EFI.fd
)

if not exist QEMU_EFI.fd (
    echo ERROR: File QEMU_EFI.fd is missing 
    echo INFO: Download this file from 'https://releases.linaro.org/components/kernel/uefi-linaro/16.02/release/qemu64' and copy it into this directory
    exit /B 1
)

REM Windows only: -cpu cortex-a72

qemu-system-aarch64.exe ^
-name "%NAME%" ^
-M virt ^
-cpu cortex-a72 ^
-accel tcg ^
-smp 4 ^
-m 4G ^
-bios QEMU_EFI.fd ^
-netdev user,id=eth0,%PFW% ^
-device virtio-net-pci,netdev=eth0,mac=DE-AD-BE-00-00-01 ^
-drive "file=%QCOW2%,format=qcow2" ^
-drive "file=%UDIMG%,format=raw" ^
-display none ^
-nographic







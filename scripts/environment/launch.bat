@echo off

rem Parameters
rem %1 CPU Architecture Guest System: amd64 (default), aarch64
rem $2 Proxy usage: proxy (default), no-proxy

call build.bat %1 %2

set P22FW=10022-:22

TITLE QEMU %CD% %ARCH% %P22FW%

IF "%ARCH%" == "amd64" (
    qemu-system-x86_64.exe ^
	-name %CD% ^
    -machine q35,accel=whpx:hax:tcg ^
    -smp 4 ^
    -m 4G ^
    -netdev user,id=eth0,hostfwd=tcp::%P22FW% ^
    -device virtio-net-pci,netdev=eth0,mac=DE-AD-BE-00-00-01 ^
    -drive file=%QCOW2%,format=qcow2 ^
    -drive file=%UDIMG%,format=raw ^
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
-name %CD% ^
-M virt ^
-cpu cortex-a72 ^
-accel whpx ^
-accel hax ^
-accel tcg ^
-smp 4 ^
-m 4G ^
-bios QEMU_EFI.fd ^
-netdev user,id=eth0,hostfwd=tcp::%P22FW% ^
-device virtio-net-pci,netdev=eth0,mac=DE-AD-BE-00-00-01 ^
-drive file=%QCOW2%,format=qcow2 ^
-drive file=%UDIMG%,format=raw ^
-display none ^
-nographic







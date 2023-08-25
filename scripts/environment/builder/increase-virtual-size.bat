@echo off

setlocal
SET PATH=c:\Program Files\Rexroth\ctrlX WORKS\qemu;%PATH%
SET QCOW2=ubuntu-22.04-server-cloudimg-amd64.qcow2

CALL :SUB_INFO

echo .
echo Increasing 'virtual size' by +10GB
echo .
timeout /t 20

qemu-img resize %QCOW2% +10G

CALL :SUB_INFO

timeout /t 20

goto EOF

:SUB_INFO
echo ---------------------------------------------------------
qemu-img info %QCOW2%
echo ---------------------------------------------------------
EXIT /B

:EOF

@echo off

REM Moves the actual overlay file to a new backing file
REM and creates a new overlay file.

setlocal
SET PATH=c:\Program Files\Rexroth\ctrlX WORKS\qemu;%PATH%
SET QCOW2=ubuntu-22.04-server-cloudimg-amd64.qcow2

REM Find filename for the new snapshot file
for /l %%x in (1, 1, 100) do (
	if not exist ubuntu-22.04-server-cloudimg-amd64-%%x.qcow2 (
	SET SNAPSHOT=ubuntu-22.04-server-cloudimg-amd64-%%x.qcow2
	goto SNAPSHOT_CREATE)
)

:SNAPSHOT_CREATE
echo New snapshot %SNAPSHOT%
timeout 10


REM The actual overlay file will become the backing file
move %QCOW2% %SNAPSHOT%

REM Create a new overlay file
qemu-img create -F qcow2 -b %SNAPSHOT% -f qcow2 %QCOW2%

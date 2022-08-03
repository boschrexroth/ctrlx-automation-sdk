@echo off
REM
REM Helper script to setup a (new) ssh connection.
REM IMPORTANT:
REM This script can be used if ssh.exe denies a connection to a 'known host' with another foot print.
REM

set HOST=127.0.0.1
set /p HOST=Host (%HOST%):

set SSHPORT=10022
set /p SSHPORT=SSHPORT (%SSHPORT%):

set USER=boschrexroth
set /p USER=User (%USER%):

REM Delete an existing entry in .ssh\known_hosts according Host and Port
ssh-keygen.exe -R [%HOST%]:%SSHPORT%

REM Append content of .ssh\id_rsa.pub to .ssh/authorized_keys on the destination system
REM Hint: Delete duplicate lines in .ssh/authorized_keys
ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% mkdir -p ~/.ssh/
type %USERPROFILE%\.ssh\id_rsa.pub | ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% "cat >> ~/.ssh/authorized_keys"

pause
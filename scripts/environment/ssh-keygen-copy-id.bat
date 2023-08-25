@echo off

echo =======================================================================
echo Helper script to add the users public key on the host operating system
echo located in %USERPROFILE%\.ssh\id_rsa.pub
echo to /home/boschrexroth/.ssh/authorized_keys of the App Build Environment
echo =======================================================================


REM IP address of the host machine is fix 127.0.0.1
set HOST=127.0.0.1

set SSHPORT=10022
set /p SSHPORT=Enter forwarded SSH port (%SSHPORT%):

REM The user name within an App Build environment is fix boschrexroth
set USER=boschrexroth

REM Delete an existing entry in %USERPROFILE%\.ssh\known_hosts
REM regarding the SSH connection to the App Build Enviroment
ssh-keygen.exe -R [%HOST%]:%SSHPORT% 2> nul

echo Appending %USERPROFILE%\.ssh\id_rsa.pub to /home/boschrexroth/.ssh/authorized_keys on t

ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% mkdir -p ~/.ssh/
type %USERPROFILE%\.ssh\id_rsa.pub | ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% "cat >> ~/.ssh/authorized_keys"
ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% "chmod 700 ~/.ssh && chmod 600 ~/.ssh/authorized_keys"

REM Removing duplicate entries from /home/boschrexroth/.ssh/authorized_keys
ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% "sort ~/.ssh/authorized_keys | uniq > ~/.ssh/authorized_keys.uniq"
ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% "mv ~/.ssh/authorized_keys{.uniq,}"

echo Content of /home/boschrexroth/.ssh/authorized_keys ---------------------------------------------------------------- 
ssh -o "StrictHostKeyChecking no" -p %SSHPORT% %USER%@%HOST% "cat ~/.ssh/authorized_keys"
echo --------------------------------------------------------------------------------------------------

pause
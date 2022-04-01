# ctrlX WORKS Feature App Build Environment

## Overview

Based on the scripts in the SDK folder scripts/environment ctrlX WORKS allows to create one or more App Build Envoriments (QEMU Virtual Machines). Each environment can be started, stopped or deleted. Furthermore a SSH shell can be started to login.

Apart from ctrlX WORKS and px.exe, no other software tools are required.

## Installation of the Feature App Build Environment

If ctrlX WORKS is not yet installed start the setup wizard and select function 'ctrlX CORE App Build Environment' on the page Custom setup.

If ctrlX WORKS is installed start ist and select Settings - Setup - Add or remove features. The setup wizard will appear, select function 'ctrlX CORE App Build Environment'.

## Handling of App Build Environments

Select App Build Environments on the side bar of ctrlX WORKS.

### Create an App Build Environment

* Click the + icon in the right corner of the page. 
* A dialog will appear.
* Enter a unique name.
* Enter/select a storage location (destination directory)
* Port forwarding: The default for SSH is defined 10022(=host):22(=build environment) - don't change 22, change 10022 only if you want to run several environments concurrently
* Select 'Use HTTP and HTTPS proxy on localhost:3128' if your host is behind a proxy. In this case install and run [px.exe](px.md)
* Create a new Add Build Enviroment with OK or abort with Cancel

!!! important
    The new enviroment has a default user acount __boschrexroth:boschrexroth__

### Start an App Build Environment

* Start a App Build Enviroment with the play icon.
* On first start the Ubuntu image will be downloaded and the enviroment will be installed. This will take some time. During this state no login will be possible. When this process is complete the VM will shutdown. Click the play icon again to restart.
* When the environment is up ctrlX WORKS will show 'Online' in column State.
* Now you can open a SSH connection (remote shell) by clicking the link in SSH. Default user/password is boschrexroth/boschrexroth.

We recommend to add your public key id_rsa.pub to the ~/.ssh/authorized_keys file of the environment to avoid password input on each login. Therefor use ssh-keygen-copy-id.bat (see below).

### Stop an App Build Environment

* Click the Stop icon.
* If your public key id_rsa.pub is not contained in ~/.ssh/authorized_keys you will be asked for a password.
* If the login succeeded the poweroff command will be send - the environment will be shut down safely.

!!! important
    Do NOT stop the environment by clicking the close icon in the title bar of the console window! This can damage the file system of your enviroment.

### Change an App Build Environment

If the envoriment is Offline you can click the Pencil icon to open the configuration page. 
!!! important
    Storage location and Use HTTP... cannot be changed.

### Delete an App Build Environment 

If the envoriment is Offline you can delete it by clicking the Waste Bin icon. The whole folder will be deleted.

### Helper Functions

The storage location folder of an environment contains two helper bat files which may be can be useful:

* __ssh-keygen-copy-id.bat__ Adds your public key id_rsa.pub to the ~/.ssh/authorized_keys file of the environment to avoid password input on each login. You will be asked for the SSHPORT, enter the host port (e.g. 10022) here.
* __shrink-qcow2.bat__ The snapshot file of an environment can get very big. With this script you can shrink it. 

!!! important
    The enviroment must be stopped. The shrinkig process can take a while - Do NOT abort it.


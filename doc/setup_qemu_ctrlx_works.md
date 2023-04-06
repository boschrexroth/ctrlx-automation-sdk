If in the ctrlX WORKS setup the function 'ctrlX CORE App Build Environment' is enabled ctrlX WORKS allows to create App Build Environments. Internally these are QEMU Virtual Machines running Ubuntu server 20.04 LTS which are configured during first startup. 

From the ctrlX WORKS UI each environment can be started, stopped or deleted. Furthermore a SSH shell can be started to login.

Apart from ctrlX WORKS and px.exe, no other software tools are required.

## Installation of the App Build Environment Function in ctrlX WORKS

During the ctrlX WORKS setup select function 'ctrlX CORE App Build Environment'.

Out of an already installed ctrlX WORKS you can start the setup wizard with Settings - Setup - Add or remove features.

## Handling of App Build Environments

Select App Build Environments on the side bar of ctrlX WORKS. The list of installed App Build Environments will be shown.

### Create an App Build Environment

* Click the `+` icon in the right corner of the page. 
* The dialog 'Add ctrlX CORE App Build Environment' will appear.  Now the new instance can be configured.
* Name: Enter a unique name.
* Storage location: Enter/select a destination directory where the files are stored.
* Port forwarding: Here prot forwardings from the Windows host to the VM guest are defined. Per default for SSH the port 10022 of the Windows host is forwarded to port 22 of the guest VM. Do NOT change 22! You can change 10022 if you want to run several environments concurrently.
* Select 'Use HTTP and HTTPS proxy on localhost:3128' if your host is behind a proxy. In this case install and run [px.exe](px.md)
* Create a new Add Build Environment with OK or abort with Cancel

!!! important
    The new environment has a default user acount __boschrexroth:boschrexroth__

### Start an App Build Environment

* Start a App Build Environment with the play `>` icon.
* On first start the Ubuntu image will be downloaded and the environment will be installed. This will take some time. In this phase no login will be possible. When the initialization phase is finished the VM will shutdown automatically. 
* Click the play icon again to restart the App Build Environment.
* When the environment is up ctrlX WORKS will show 'Online' in column State.

### Working with an App Build Environment

You can open a SSH connection (secure shell) by clicking the according link in the column SSH of ctrlX WORKS 'App Build Environments' view.

For the login a password is required, it is __boschrexroth__.

We recommend to add your public key contained in the __hosts__ id_rsa.pub file to the file authorized_keys in the VMs directory /home/boschrexroth/.ssh/. So password input on each login is obsolete. 

Therefor you can use the bat file ssh-keygen-copy-id.bat in the destination directory of the VM (see below).

Please regard instructions in chapter [Important Install Scripts](install-scripts.md).

### Stop an App Build Environment

* Click the Stop icon.
* If your public key id_rsa.pub is not contained in ~/.ssh/authorized_keys you will be asked for a password.
* If the login succeeded the poweroff command will be send - the environment will shut down safely.

!!! important
    Do NOT stop the environment by clicking the close icon in the title bar of the console window! This can damage the file system of your environment.

### Change an App Build Environment

If the environment has the State Offline a click of the Pencil icon opens the configuration page. 

!!! important
    Once created the parameters 'Storage location' and 'Use HTTP...' cannot be changed.

### Delete an App Build Environment 

If the environment is Offline you can delete it by clicking the Waste Bin icon. The whole folder will be deleted.

### Helper Functions

The storage location folder on your host system for each environment also contains two helper bat files which can be useful:

* __ssh-keygen-copy-id.bat__ Adds your public key id_rsa.pub to the ~/.ssh/authorized_keys file of the environment to avoid password input on each login. You will be asked for the SSHPORT, enter the host port (e.g. 10022) here.

* __shrink-qcow2.bat__ The snapshot file of an environment can get very big. With this script you can shrink it. 

!!! important
    The environment must be stopped. The shrinking process can take a while - Do NOT abort it.


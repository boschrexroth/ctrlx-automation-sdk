If in the ctrlX WORKS setup the function 'ctrlX CORE App Build Environment' is enabled ctrlX WORKS allows to create App Build Environments. Internally these are QEMU Virtual Machines running Ubuntu server LTS which are configured during first startup. 

From the ctrlX WORKS UI each environment can be started, stopped or deleted. Furthermore a SSH shell can be started to login.

Apart from ctrlX WORKS and px.exe, no other software tools are required.

## Checking the Internet Connection

!!! important
    For the installation of an App Build Environment a functioning internet connection is required.

If your host uses a proxy server install and run [px.exe](px.md).

You can test whether your internet connection is working by clicking this link [Ubuntu 22.04 LTS](https://cloud-images.ubuntu.com/releases/jammy/release). A site with Ubuntu server cloud images should appear.

## Installation of the App Build Environment Function in ctrlX WORKS

During the ctrlX WORKS setup select function 'ctrlX CORE App Build Environment'.

Out of an already installed ctrlX WORKS you can start the setup wizard with Settings - Setup - Add or remove features.

## Handling of App Build Environments

Select 'App Build Environments' on the side bar of ctrlX WORKS. The list of installed App Build Environments will be shown.

### Create an App Build Environment

* Click the `+` icon in the right corner of the page. 
* The dialog 'Add ctrlX CORE App Build Environment' will appear.  Now the new instance can be configured.
* Name: Enter a unique name.
* Storage location: Enter/select a destination directory where the files are stored.
* Port forwarding: Here port forwardings from the Windows host to the VM guest are defined. Per default for SSH the port 10022 of the Windows host is forwarded to port 22 of the guest VM. Do NOT change 22! You can change 10022 if you want to run several environments concurrently.
* Select 'Use HTTP and HTTPS proxy on localhost:3128' if your host uses a proxy. In this case install and run [px.exe](px.md)
* Create a new Add Build Environment with OK or abort with Cancel

!!! important
    The new environment has a default user acount __boschrexroth:boschrexroth__

### Start an App Build Environment

* Start a App Build Environment with the play `>` icon.
* On first start the Ubuntu image will be downloaded and the environment will be installed. This will take some time. In this phase no login will be possible. When the initialization phase is finished the VM will shutdown automatically. 
* Click the play icon again to restart the App Build Environment.
* When the environment is up ctrlX WORKS will show 'Online' in column State.

### Working with an App Build Environment

#### Preparations

On your Windows host start cmd.exe and run these commands:

* Delete entry regarding 127.0.0.1 in the file know_hosts

	ssh-keygen.exe -R [127.0.0.1]:10022
	
* Copy your public key (here: id_rsa.pub) to the App Build Environment (on password promt enter: boschrexroth)

	ssh.exe -p 10022 boschrexroth@127.0.0.1 "mkdir -p ~/.ssh"
	scp.exe -P 10022 %USERPROFILE%\.ssh\id_rsa.pub boschrexroth@127.0.0.1:~/.ssh/id_rsa.pub.tmp
	ssh.exe -p 10022 boschrexroth@127.0.0.1 "cat ~/.ssh/id_rsa.pub.tmp >>~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys && rm ~/.ssh/id_rsa.pub.tmp"

#### Start a ssh session

You can open a SSH connection (secure shell) by clicking the according link in the column SSH of ctrlX WORKS 'App Build Environments' view.

If your preparations succeeded no password is needed otherwise enter __boschrexroth__

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
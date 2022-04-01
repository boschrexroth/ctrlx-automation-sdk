# Setting up a QEMU Virtual Machine as App Build Environment

## Overview

This guide shows how to use an [Ubuntu Server operating system](https://ubuntu.com/server) running in a virtual machine using the [QEMU virtualizer](https://www.qemu.org/) to develop ctrlX AUTOMATION Apps.

To simplify the setup of a new QEMU VM we use the [Ubuntu Cloud-Init Technology](https://cloud-init.io/) which installs the default user __boschrexroth/boschrexroth__ and all necessary tools on first boot.

As standard constellation the QEMU software should be installed and run on an __AMD64__ based host. QEMU supports both Windows 10 and Linux host systems.

For the QEMU virtual machine (QEMU VM) we can select

* AMD64 CPU architecture - this provides the best performance.
* AARCH64 (ARM64) CPU architecture - only necessary for special purposes - see below.

The installation of QEMU, further required tools and QEMU virtual machines is described in the next chapters.


## Preparations

Download the archive ctrlX AUTOMATION SDK from [github boschrexroth/ctrlx-automation-sdk/releases](https://github.com/boschrexroth/ctrlx-automation-sdk/releases) to your host computer and unpack at least the folder  __public/scripts/environment__.

This folder contains all necessary files and scripts for both Windows 10 and Linux hosts.

## Install QEMU 

### Linux Host

Change to the SDK folder public/scripts/environment and set x file permissions:

    $ chmod a+x *.sh

#### Install QEMU on Linux Host

Because the newest QEMU software is not provided as Debian package we have to download the source files, compile the project and install the binaries. This is done automatically by the script __install-qemu-on-host.sh__ stored in the SDK folder __public/scripts/environment/__.

To install QEMU do following steps:

* Copy the script install-qemu-on-host.sh into a tempory folder on your Linux host computer.

* Change to the temporary folder and start these commands: 

	$ chmod a+x install-qemu-on-host.sh
    $ sudo ./install-qemu-on-host.sh

This procedure will take some time - at the end QEMU is installed.

#### Install KVM on Linux Host

We recommend to install KVM (Kernel-based Virtual Machine) on your host sytem.

Change to the SDK folder public/scripts/environment and start

    ./install-kvm-on-host.sh

See here for further informations:

[How to Install Kvm on Ubuntu 20.04](https://linuxize.com/post/how-to-install-kvm-on-ubuntu-20-04/)

[linux-kvm](https://www.linux-kvm.org/page/Documents)


### Windows 10 Host

#### Install QEMU on Windows 10 Host

To install QEMU download and install the newes version for Windows from [https://qemu.weilnetz.de/w64/](https://qemu.weilnetz.de/w64/).

!!! important
    Add the installation path of your qemu software to the system environment varaible %PATH%.

    See [How do I set or change the PATH system variable?](https://www.java.com/en/download/help/path.html)

#### Install Windows Hypervisor Platform

We recommend to install __Windows Hypervisor Platform__: 

* Right click on the Windows button and select ‘Apps and Features’.
* Select Programs and Features on the right under related settings.
* Select Turn Windows Features on or off.
* Select Windows Hypervisor Platform - NOT Hyper-V.
* Close dialog with OK.

This feature can also be installed via command line:

* Start cmd.exe as administrator (run as ...)
* Enter 

    __dism /online /Enable-Feature /FeatureName:HypervisorPlatform /All__

__Hints:__ 

* After enabling Windows Hypervisor Platform a system restart may be required.
* The feature Hyper-V is automatically disabled.
* You can disable the feature with:

    dism /online /Disable-Feature /FeatureName:HypervisorPlatform

* If you use Virtual Box also please enable the internal Hyper-V option for each virtual machine.

* If Windows Hypervisor Platform cannot be installed for any reason, we recommend using the Intel Hardware Accelerated Execution Manager (HAXM). The installation is described here [Installing HAXM](https://docs.microsoft.com/en-us/xamarin/android/get-started/installation/android-emulator/hardware-acceleration?pivots=windows#installing-haxm)



#### Install Px.exe as Local Proxy Server 

See [Use PX.exe as Local Proxy on a Windows Host](./px.md).

## Create an Instance of a QEMU Virtual Machine

### AMD64 or AARCH64 QEMU Virtual Machine

On an AMD64 host computer an AMD64 QEMU VM has a very good performance.

__Only if the desired programming language for developing ctrlX apps doesn't support cross compiling an AARCH64 QEMU VM should be used.__

This currently applies to __Python and Node.js__. 

Another reason can be the building of Node-RED packages when Node.js code has to be complied to native CPU machine code.

### Scripts to Create an Instance

A virtual machine should be installed and started within a separate folder on your host computer. To support you the SDK folder __public/scripts/environment__ contains these scripts:

* __create-new-vm-aarch64-noproxy.bat, .sh__: creates an AARCH64 VM with direct internet access
* __create-new-vm-aarch64-proxy.bat, .sh__: creates an AARCH64 VM using a proxy server
* __create-new-vm-amd64-noproxy.bat, .sh__: creates an AMD64 VM with direct internet access
* __create-new-vm-amd64-proxy.bat, .sh__: creates an AMD64 VM using a proxy server

__These scripts are expecting the destination folder of your new VM as argument.__

To create e.g. a new amd64 VM instance with proxy usage on a Windows host do following steps:

* Create a separate folder on your host computer D:\qemuvm\amd64-proxy-1
* Start cmd.exe
* Change to the SDK folder __public/scripts/environment__
* Call create-new-vm-amd64-proxy.bat D:\qemuvm\amd64-proxy-1

All neccessary files for this new VM are copied into the destination folder.

## Running a QEMU Virtual Machine

### Start the QEMU Virtual Machine

To start the QEMU VM instance change to its installation folder.

You should find one of this scripts according your VM properties (architecture, proxy usage or not):

* __launch-aarch64-noproxy.bat, .sh__: AARCH64 VM with direct internet access
* __launch-aarch64-proxy.bat, .sh__: AARCH64 VM using a proxy server
* __launch-amd64-noproxy.bat, .sh__: AMD64 VM with direct internet access
* __launch-amd64-proxy.bat, .sh__: AMD64 VM using a proxy server

In our Windows example start:

    launch-amd64-proxy.bat

The VM is started as console application, you can see the trace output.

We recomment to __not use this console directly__ because a lot of shell functions are not supported.

During first boot a lot of software will be installed - so please be patient.

If this process is finished the VM will shutdown.

### Network Connection to your QEMU Virtual Machine

From the point of view of your host computer, the VM does not have own network interface cards. Services running on the VM are available via __port forwarding__.

This port forwarding is configured in the launch... script, per default only port 22 (SSH) is forwarded:

-netdev user,id=eth0,__hostfwd=tcp::10022-:22__

Port 10022 of the host is forwarded to port 22 (SSH) of the VM. If port 10022 is used on the host the VM will not start. In this case please enter a free host port number in the launch script e.g. 20022.

Further ports can be forwarded just extend this line e.g. -netdev user,id=eth0,hostfwd=tcp::10022-:22,__hostfwd=tcp::502-:502__

!!! Important
    From your host computer a SSH connection can be established using __127.0.0.1:10022__

During the first connection with Visual Studio Code enter: __ssh -p 10022 boschrexroth@127.0.0.1__ password is __boschrexroth__

### Install the ctrlX AUTOMATION SDK and Additional Software After First Boot

Because some necessary software components cannot be installed during first boot they have to be installed manually. 

So restart your VM, login with __boschrexroth/boschrexroth__ and start these scripts:

* install-snapcraft.sh
* optional if you need the dotnet-sdk: install-dotnet-sdk.sh
* optional if you need Node.js and npm: install-nodejs-npm.sh
* __install-sdk-sh__

The script __install-sdk-sh__ downloads, unpacks and preconfigures the ctrlX AUTOMATION SDK archive [github boschrexroth/ctrlx-automation-sdk/releases](https://github.com/boschrexroth/ctrlx-automation-sdk/releases).

Therefor do following steps:

* Create a new folder

    mkdir ctrlx
    
    chdir ctrlx

* Call the install script

    ~/install-sdk.sh

	
### Shutdown

It's very important to shutdown the __QEMU VM__ properly. So initiate a shutdown e.g. with this command:

    sudo poweroff
	

## QEMU Virtual Machine Networking

The QEMU software represents a virtual network between the host operating system and the virtual machine. 

!!! important
    This virtual network is __only seen by the virtual machine__ NOT by the host operating system.

Furthermore QEMU provides a gateway, a DNS and an optional SMB server for its VM. 

Per default this virtual network has the subnet 10.0.2.0/24 and from the perspective of the QEMU VM these IP addresses are assigned:

* 10.0.2.15 IP address of the VM itself
* 10.0.2.2  Gateway and __host operating system__
* 10.0.2.3  DNS server
* 10.0.2.4  SMB server


For deeper information visit [QEMU Documentation/Networking](https://wiki.qemu.org/Documentation/Networking)

## Troubleshooting, Tips

### General

Read error messages carefully, search in the internet for these messages and hints for troubleshooting.

### Performance

Deactivate all other virtual machines on your host to avoid performance disadvantages.

An emulated _AARCH64_ QEMU VM is slow - be patient.

### http/https Access

Check the http/https access of your host to the internet - respectively to the Ubuntu sites e.g. with:
    
    wget -p http://ports.ubuntu.com/ubuntu-ports

If failed check your DNS settings and server(s).
  
### Snap Store Status

Check the [Snap Store Status](https://status.snapcraft.io/) if snaps cannot be installed.
  

### WinSCP and Putty
On your Windows host you can use [WinSCP](https://winscp.net/) combined with [PuTTy](https://www.chiark.greenend.org.uk/~sgtatham/putty/docs.html) instead of the Windows ssh and scp.


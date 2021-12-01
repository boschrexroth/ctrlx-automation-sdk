
# Setup ctrlX AUTOMATION Development Environment using QEMU

## Overview

This guide shows how to use an [Ubuntu Server operating system](https://ubuntu.com/server) running in a virtual machine using the [QEMU virtualizer](https://www.qemu.org/) to develop ctrlX AUTOMATION Apps.

QEMU supports both Windows 10 and Linux/BSD host systems. Furthermore it provides an emulator for a large list of machines and architectures especially AARCH64 (ARM64).

To simplify the setup of a new virtual machine we use the [Ubuntu Cloud-Init Technology](https://cloud-init.io/) which installs the default user __boschrexroth/boschrexroth__ and all necessary tools on first boot.

All necessary files and scripts for Windows 10 and Linux are stored in the SDK folder __public/scripts/environment__

## Install QEMU 

The installation of QEMU varies depending on the host operating system.

### Install QEMU on a Linux/BSD Host Operating System

Detailed information regarding installation and usage of QEMU VM on Linux hosts are describes here: [QEMU - Linux](./scripts/environment/3-README-linux.md).

Because the newest QEMU software is not provided as Debian package we have to download the source files, compile the project and install the binaries. This is done automatically by the script __public/scripts/environment/install-qemu-on-host.sh__

To install QEMU do following steps:

* Copy the script into a tempory folder on your Linux host computer.
* Start the script whith admin rights: $ sudo ./install-qemu-on-host.sh

This procedure will take some time.

### Install QEMU on a Windows 10 Operating System

Detailed information regarding installation and usage of QEMU VM on Windows 10 hosts are describes here: [QEMU - Windows 10](./scripts/environment/2-README-win10.md).

To install QEMU download and install the newes version for Windows from [https://qemu.weilnetz.de/w64/](https://qemu.weilnetz.de/w64/).

We recommend to install Windows Hypervisor Platform: 

* Right click on the Windows button and select ‘Apps and Features’.
* Select Programs and Features on the right under related settings.
* Select Turn Windows Features on or off.
* Select Windows Hypervisor Platform - NOT Hyper-V.
* Close dialog with OK.

__Hints:__ Hyper-V is automatically disabled. If you use Virtual Box also please enable the internal Hyper-V option for each virtual machine.

If your host system is behind a proxy we recommend __Px.exe__ as local proxy running on the Windows host - see [https://github.com/genotrance/px](https://github.com/genotrance/px)

## Create an Instance of a QEMU Virtual Machine

A virtual machine should be installed and started within a separate folder on your host computer. To support you the SDK folder __public/scripts/environment__ contains these scripts:

* __create-new-vm-aarch64-noproxy.bat, .sh__: AARCH64 VM with direct internet access
* __create-new-vm-aarch64-proxy.bat, .sh__: AARCH64 VM using a proxy server
* __create-new-vm-amd64-noproxy.bat, .sh__: AMD64 VM with direct internet access
* __create-new-vm-amd64-proxy.bat, .sh__: AMD64 VM using a proxy server

To create e.g. a new amd64 VM instance with proxy usage on a Windows host do following steps:

* Create a separate folder on your host computer D:\qemuvm\amd64-proxy-1
* Copy the SDK folder __public/scripts/environment__ to a folder on your host computer e.g. D:\ctrlx-sdk\public\scripts\environment
* Start cmd.exe
* Change to this directory D:\ctrlx-sdk\public\scripts\environment
* Call create-new-vm-amd64-proxy.bat D:\qemuvm\amd64-proxy-1

All neccessary files for this new VM are copied into the destination folder.

## Running a QEMU Virtual Machine

### Start the QEMU Virtual Machine
To start the VM instance change to its installation folder and call the according launch script:

* __launch-aarch64-noproxy.bat, .sh__: AARCH64 VM with direct internet access
* __launch-aarch64-proxy.bat, .sh__: AARCH64 VM using a proxy server
* __launch-amd64-noproxy.bat, .sh__: AMD64 VM with direct internet access
* __launch-amd64-proxy.bat, .sh__: AMD64 VM using a proxy server

The VM is started as console application, you can see the trace output.

We recomment to __not use this console directly__ because a lot of shell functions are not supported.

During first boot a lot of software will be installed - so please be patient.

If this process is finished the VM will shutdown.

### Network Connection to your QEMU Virtual Machine

From the point of view of your host computer, the VM does not have its own network interface card. Services running on the VM are available via __port forwarding__.

This port forwarding is configured in the launch... script, per default only port 22 (SSH) is forwarded:

-netdev user,id=eth0,__hostfwd=tcp::10022-:22__

Port 10022 of the host is forwarded to port 22 (SSH) of the VM. If port 10022 is used the VM will not start, so please enter a free host port in the launch script e.g. 20022

Further ports can be forwarded just extend this line e.g. -netdev user,id=eth0,hostfwd=tcp::10022-:22,__hostfwd=tcp::502-:502__

!!! Important
    From your host computer a SSH connection can be established using __127.0.0.1:10022__

During the first connection with VSCode enter: __ssh -p 10022 boschrexroth@127.0.0.1__ password is __boschrexroth__

### Install Additional Software After First Boot

Because some necessary software components cannot be installed during first boot they have to be installed manually. 

So restart your VM, login with __boschrexroth/boschrexroth__ and start these scripts:

* install-snapcraft.sh
* optional if you need the dotnet-sdk: install-dotnet-sdk.sh
* optional if you need node.js and npm: install-nodejs-npm.sh

### Install the SDK on the QEMU Virtual Machine

Download the ctrlX AUTOMATION SDK archive from [github boschrexroth/ctrlx-automation-sdk/releases](https://github.com/boschrexroth/ctrlx-automation-sdk/releases) to your VM.

Here e.g. version 1.12.0:

    wget https://github.com/boschrexroth/ctrlx-automation-sdk/releases/download/1.12.0/ctrlx-automation-sdk-1.12.0.zip

Extract this archive:

    unzip -XK ctrlx-automation-sdk-1.12.0.zip

### QEMU Virtual Machine Networking

The QEMU software represents a virtual network between the host operating system and the virtual machine. 

!!! important
    This virtual network is __only seen by the virtual machine__ NOT by the host operating system.

Furthermor QEMU provides a gateway, DNS and an optional SMB server for its VM. 

Per default this virtual network has the subnet 10.0.2.0/24 and from the perspective of the QEMU VM these IP addresses are assigned:

* 10.0.2.15 IP address of the VM itself
* 10.0.2.2  Gateway and __host operating system__
* 10.0.2.3  DNS server
* 10.0.2.4  SMB server


For deeper information visit [QEMU Documentation/Networking](https://wiki.qemu.org/Documentation/Networking)

# Use QEMU Virtual Machines to work with the ctrlX SDK

## Introduction

To develop and test ctrlX applications with the ctrlX SDK you need a Linux operating system. 

The usage of a virtual machine therefor has some advantages:

- The Linux VM be run on a Windows host.
- The VM does not affect the host operating system.
- More than one VMs can run simultanously.
- Snapshots from a VM can be created and saved.

With the scripts and files provided here you can install and run __Ubuntu20.04 LTS Server__ in a QEMU virtual machine (__QEMU VM__). 

As host operating system Windows 10 and Linux are supported. The host operating specific parts are described in different files - see below.

__Important:__ For both host operating system you need admin rights!


## AMD64 or AARCH64 Guest System

For most development purpuses you can use an AMD64 virtual machine. On an AMD64 host with hardware virtualization extension this is a very performant solution.

__Important__:

An AARCH64 (ARM64) virtual machine on an AMD64 host system will be emulated by QEMU. Because this emulated VM will have lesser performance it should only be used for programming languages where cross build is not supported. For example building a snap for a ctrlX CORE M3/4 from a Python project.

To improve the installation and boot process of an AARCH64 VM it has a reduced scope of installation. For details see cloud-config-aarch64.

## Setup and Start the QEMU VM

If you have a Windows host continue reading [here.](./2-README-win10.md)

If you have a Linux host continue reading [here.](./3-README-linux.md)

The following chapters are relevant for both host systems.

## First Boot of the QEMU VM

During the first boot of your __QEMU VM__ a lot of software components are installed - this takes a while.

When the installation is finished the __QEMU VM__ will automatically shutdown.

Now the __QEMU VM__ can be used - please restart the the VM with the according launch script or bat file.

## Install Snaps after First Boot

During the setup process on first boot the installation of snaps fails but when the VM is restarted it will succeed.

Please install snapcraft as snap using this command:

    cd ~
    ./install-snapcraft.sh
    
The dotnet-sdk is only neccessary for AMD64 VMs using the ctrlX AUTOMATION SDK for developing applications in .NET. Install it as snap using:

    cd ~
    ./install-dotnet-sdk.sh

Optional: Install node.js and npm:

    cd ~
    ./install-nodejs-npm.sh

## Working with your QEMU VM

In the user-data files a default user defined. The credentials are:

User: __boschrexroth__

Password: __boschrexroth__

### Using ssh

Per default __port 10022 of your host__ is forwarded to __port 22 (the ssh port) of your guest system__.

If port 10022 is not free on your host or if you want to use further VMs simultaneously can change this in the launch.bat/launch.sh script in the entries 'hostfwd=tcp::10022-:22'. 

#### Remote ssh Connection

Do not use the VMs console window directly because it is buggy. Use a remote ssh connection instead.

Enter this command on your host you to start a remote ssh connection to your VM:
 
    ssh -p 10022 boschrexroth@localhost


With the scp command on your host you can copy files to and from the guest system.

From host to guest:

    scp -P 10022 host-file boschrexroth@localhost:~/guest-dir/guest-file

From guest to host:

    scp -P 10022 boschrexroth@localhost:~/guest-dir/guest-file host-file 

__Hints__: 
* The option P of the scp command must be a __capital letter__.
* Consider to use a tool combining shell and copy functions e.g. [MobaXterm](https://mobaxterm.mobatek.net/)


#### Install ssh Authentification with Key Pair

To avoid frequently entering the user/password combination we recommend to use a RSA key pair.

The following commands can be used on Linux and also on Windows but starting a bash shell:

If you don't have a private/public key pair create it on your host system:

    ssh-keygen -t rsa

Then copy the public key to your guest system:

    ssh-copy-id -p 10022 boschrexroth@localhost

### Connection to ctrlX CORE controls

Your QEMU VM is running in so called User Networking mode. In this mode QEMU provides internally (between host and guest operating system) a DHCP, a Gateway, a DNS and a SMB server. Further informations see chapter `User Networking (SLIRP)` on [QEMU Documentation/Networking](https://wiki.qemu.org/Documentation/Networking#User_Networking_.28SLIRP.29).

TCP/IP packages from a component within the VM to a ctrlX are forwarded via the Gateway server to the host operating system. So the connection to a hardware based ctrlX COREs as well as to a ctrlX CORE virtual are possible.

Here some examples of connection strings for a Datalayer provider:
1. ctrlX CORE or ctrlX CORE virtual with network adapter: tcp://boschrexroth:boschrexroth@192.168.1.1:2070
2. ctrlX CORE virtual with port forwarding: tcp://boschrexroth:boschrexroth@10.0.2.2:2070

Explanation:

192.168.1.1: The VM routs TCP/IP packages to the gateway server of QEMU. Here they are sent to the host operating system. This will sent them directly or also via its gateway to the control.

10.0.2.2: The VM routs TCP/IP packages to the gateway server of QEMU. It substitutes the IP with 127.0.0.1 and sents them to the host operating system. Via port forwarding (2070:2070) the packages are routed to the ctrlX virtual.


### Shutdown

It's very important to shutdown the __QEMU VM__ properly.
So initiate a shutdown e.g. with this command:

    sudo poweroff


## Troubleshooting, Tips

- Deactivate all other virtual machines on your host to avoid performance disadvantages.
- An emulated _AARCH64_ guest system is slow - be patient.
- Check the http/https access of your host to the internet - respectively to the Ubuntu sites e.g. with:
    
    wget -p http://ports.ubuntu.com/ubuntu-ports

  If failed check your DNS settings and server(s).
  
- Check the [Snap Store Status](https://status.snapcraft.io/) if snaps cannot be installed.
  
- Read error messages carefully, search in the internet for these messages and hints for troubleshooting.

- On your Windows host you can use [WinSCP](https://winscp.net/) combined with [PuTTy](https://www.chiark.greenend.org.uk/~sgtatham/putty/docs.html) instead of the Windows ssh and scp.

Important web sites:
- [QEMU](https://wiki.ubuntu.com/ARM64/QEMU) 

## License

MIT License

Copyright (c) 2021 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

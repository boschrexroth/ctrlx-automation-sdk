This chapter describes how to setup and run App Build Environments without using ctrlX WORKS.

Remember ctrlX WORKS provides all necessary actions required to deal with app build environments.


This is the recommended constellation:

* Despite QEMU supports both Windows and Linux we are recommending to use a Windows host operating system .

* Host and guest machine should use CPU architecture amd64. Snaps for an arm64 target are build via cross build.

## Installation on Windows 10 Host

### QEMU on Windows

Download and install the newest version of QEMU for Windows from [https://qemu.weilnetz.de/w64/](https://qemu.weilnetz.de/w64/).

!!! important
    Add the installation path of your qemu software to the system environment variable %PATH%.

    See [How do I set or change the PATH system variable?](https://www.java.com/en/download/help/path.html)

### Install Windows Hypervisor Platform

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


### Install Px.exe as Local Proxy Server 

See [Use PX.exe as Local Proxy on a Windows Host](./px.md).

## Installation on Linux Host

You have to install both QEMU and KVM on your host system. 

The installation process is descriped in the internet e.g.

[How to Install QEMU on Ubuntu to Set Up a Virtual Machine](https://www.makeuseof.com/how-to-install-qemu-ubuntu-set-up-virtual-machine/). 

See also:

[How to Install Kvm on Ubuntu 20.04](https://linuxize.com/post/how-to-install-kvm-on-ubuntu-20-04/)

[KVM](https://www.linux-kvm.org/page/Documents)

## Running the QEMU Virtual Machine

### Create an Instance of a QEMU Virtual Machine

Do following steps:

* Create a new folder (on a disk with enough free disk space)
* Copy the content of the SDK folder __public/scripts/environment__ into your instance folder.

__Hint:__ You can download the folder from here

[ctrlx-automation-sdk/scripts/environment/](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts/environment)


### Start the QEMU Virtual Machine

To start the QEMU VM instance change to its installation folder and run one of these script files:

* __launch-amd64-noproxy (.bat, .sh) : If the VM has direct internet access
* __launch-amd64-proxy (.bat, .sh):    If the VM has to use a proxy server on the host computer

The VM is started as console application, you can see the trace output.

We recommend to __not use this console directly__ because a lot of shell functions are not supported.

During first boot a lot of software will be installed - so please be patient.

If this process is finished the VM will shutdown. Just run the launch script to restart the VM.

### Network Connection to your QEMU Virtual Machine

From the point of view of your host computer, the VM does not have own network interface cards. Services running on the VM are available via __port forwarding__.

This port forwarding is configured in the launch... script with one or more __hostfwd=__ settings e.g.:

    -netdev user,id=eth0,hostfwd=tcp::10022-:22

Here port 10022 of the host is forwarded to port 22 (SSH) of the VM. If port 10022 is used on the host the VM will not start. In this case please enter a free host port number in the launch script e.g. 20022.

For forwarding further ports e.g. __502 (Modbus)__ just extend this line e.g.:

    -netdev user,id=eth0,hostfwd=tcp::10022-:22,hostfwd=tcp::502-:502

!!! Important
    From your host computer a SSH connection can be established using __127.0.0.1:10022__

During the first connection with Visual Studio Code enter: __ssh -p 10022 boschrexroth@127.0.0.1__ password is __boschrexroth__

### Shutdown

It's very important to shutdown the __QEMU VM__ properly. So initiate a shutdown e.g. with this command on your VM console:

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


## Setup

### Install additional Software on your Windows Host

#### QEMU software for Windows

Download __latest__ setup executable of QEMU for Windows [here](https://qemu.weilnetz.de/w64), copy it into a temporary folder and start it. 

An installation wizard will install the QEMU software for Windows.
  
Because this wizard has a bug, please add the QEMU installation path manually to the _PATH_ environment variable.

__Hint__: 
You can start the System Environment Variables dialog from the "Run" command prompt using this command:

    powershell -command "&{start-process rundll32.exe sysdm.cpl,EditEnvironmentVariables -verb RunAs}

#### Use Hardware Acceleration

We recommend to use Windows Hypervisor Platform (WHPX) for hardware acceleration. Therefore open the Windows Features dialog and check the box 'Windows Hypervisor Platform'.

If your host computer doesn't support WHPX install HAXM (see 
[here](https://docs.microsoft.com/en-us/xamarin/android/get-started/installation/android-emulator/hardware-acceleration?pivots=windows), chapter 'Installing HAXM').

#### Px-Proxy as Proxy Server

If your Windows host runs behind a proxy server, your __QEMU VM__ needs access to this proxy server. 

We recommend to use  [Px-Proxy](https://pypi.org/project/px-proxy/):

- Download the zip file from [here](https://github.com/genotrance/px/releases/latest).
- Install, configure and run _Px-Proxy_. 

The proxy server on your Windows host is accessible from your __QEMU VM__ with __10.0.2.2:3128__

Info: __10.0.2.2__ is the IP address of your host seen by the VM.

This proxy server is also used by download tools on your Windows host. Therefor add these environment variables on your Windows host system - see hint above:

    https_proxy=localhost:3128

    http_proxy=localhost:3128


### Install the Runtime Directory

We recommend to create a separate directory to run your __QEMU VM__.

Open the folder _qemu_ of the ctrlX SDK and start one of the _create....bat_ files. Provide the destination directory as command line parameter. The destination directory will be created and all necessary files to start und run a VM are copied.

The different _create....bat_ files are:

- create-new-vm-amd64-proxy.bat: Create an AMD64 guest system using a proxy server running on the host for internet access
- create-new-vm-amd64-noproxy.bat: Create an AMD64 guest system with direct internet access 
- create-new-vm-aarchd64-proxy.bat: Create an AARCH64 (emulated) guest system using a proxy server running on the host for internet access
- create-new-vm-aarchd64-noproxy.bat: Create an AARCH64 (emulated) with direct internet access

Example:

    create-new-vm-amd64-proxy.bat d:\qemu\amd64-proxy

__Hint:__ The user-data files are containing important settings and commands started on first boot. This is configured in the user-data image files derived from the cloud-config file. If you want to change this settings you need a __Linux__ host with the package 'cloud-image-utils' installed. [Here](./3-README-linux.md)  is described how to change to cloud-config file.

## Start the QEMU Virtual Machine

Just start the __launch...bat__ file for your need:

- launch-amd64-proxy.bat: Launch an AMD64 VM with proxy usage
- launch-amd64-noproxy.bat: Launch an AMD64 VM with direct internet access
- launch-aarch64-proxy.bat: Launch an AARCH64 VM with proxy usage
- launch-aarch64-noproxy.bat: Launch an AARCH64 VM with direct internet access

Continue to read [here](./1-README.md), chapter 'First Boot of the __QEMU VM__'

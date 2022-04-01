# QEMU VM with Ubuntu Server as ctrlX AUTOMATION Development Environment

## Indroduction

This folder contains configuration files and scripts to build and launch [QEMU](https://wiki.ubuntu.com/ARM64/QEMU) virtual machines running Ubuntu Server guest operating system on Windows 10 or Linux host operating systems.

__Important__: The host operating system must be amd64 based.

Installation and configuration of the Ubuntu Server guest operating system are automatically done on first boot by the Ubuntu Cloud-Init Technology. For this purpose, a so-called user image file is specified when the VM is started. This file contains the compiled information of a cloud-config file which is processed by the cloud-init service.

For more informations [cloud-init](https://cloud-init.io/)

## File Overview

Hint: The parameters of .sh/.bat files are explained in comment lines at the beginning of these files.

### Build Files:

* build.bat Windows batch file to download Ubuntu Server image - check/change environment variables 
* build.sh  Linux build script to install packages, to compile a cloud-config file and to download Ubuntu Server image

* build-all.sh  Linux build script to compile all types of user image files (amd64/aarch64, proxy/noproxy).

### cloud-config Files

Cloud-config files are containing settings (user/password, proxy,...) and a list of packages to be installed:

* cloud-config-aarch64  For aarch64 VMs
* cloud-config-amd64    For amd64 VMs

For informations on the content of cloud-config files see [Cloud config examples](https://cloudinit.readthedocs.io/en/latest/topics/examples.html).

### Creating a QEMU Virtual Machine instance

Scripts/bat files starting with create-new... are creating a new folder and coppying all necessary files into it:

* create-new-vm-ARCH-PROXY.bat/sh   Creates a new VM where ARCH is amd64 or aarch64, PROXY is proxy or noproxy
* create-new-vm.bat/sh  Bat/sh file file called as subroutine
 
### Helper Scripts to Install Packages on Linux (Ubuntu) Host Operating Systems

The scripts has to be started on the host system:

* install-kvm-on-host.sh    Install the kvm package
* install-qemu-on-host.sh   Download, build and install QEMU

### Script/bat Files to start a QEMU VM

* launch.bat/sh Generic script/bat file to start an VMFiles to start a QEMU VM
* launch-ARCH-PROXY.bat/sh  Helper scripts to start launch.bat/sh with the right parameters.

### User Image Files

Instead of compiling a cloud-config file __on Windows host systems one of the precompiled user image file has to be used__:

* ubuntu-20.04-server-cloudimg-amd64-user-data-proxy.img    amd64 VM with proxy usage
* ubuntu-20.04-server-cloudimg-amd64-user-data-noproxy.img  amd64 VM without proxy usage
* ubuntu-20.04-server-cloudimg-aarch64-user-data-proxy.img    aarch64 VM with proxy usage
* ubuntu-20.04-server-cloudimg-aarch64-user-data-noproxy.img  aarch64 VM without proxy usage

### Windows Helper Batch 

* wget.bat  Calls PowerShell to dowmload files

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

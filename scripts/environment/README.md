# ctrlX App Build Environment

## Indroduction

This folder contains bat and image files to build and launch an App Builder Environment.

This is a [QEMU](https://wiki.ubuntu.com/ARM64/QEMU) virtual machines running an Ubuntu Server operating system.

__Important__: The host operating system must be Windows 64Bit (amd64).

Installation and configuration of the Ubuntu Server guest operating system are automatically done on first boot by the Ubuntu Cloud-Init Technology. 
For this purpose, a so-called user image file is specified when the VM is started. 
This file contains the compiled information of a cloud-config file which is processed by the cloud-init service.

For more informations see [cloud-init](https://cloud-init.io/)

## Starting and Building the App Builder Environment

* start.bat is created and called by ctrlX WORKS. Here the required parameters are passed.
* start.bat calls launch.bat
* launch.bat calls build.bat. This script downloads the Ubuntu Server image and creates a qemu snapshotfile .qcow2
* The Ubuntu Server image file will kept unchanged, all changes are stored into the snapshot file.
* Furthermore a user-data image file is provided. This file contains the cloud-config informations used during first boot to preinstall the operating system.


## Windows Helper Batchs

### ssh-keygen-copy-id.bat

Enables login without password by adding .ssh\id_rsa.pub of the host to ~/.authorized_keys of the QM VM.

!!! important
    We recommend to use this script instead of always signing in with the password.

### shrink-qcow2.bat

Shrinks the qcow2 snapshot file.

!!! important
    This action takes some time, do not interrupt.

### wget.bat  

Calls PowerShell to download files.

## License

MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

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

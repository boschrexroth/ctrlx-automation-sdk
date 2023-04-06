# Building and Providing a ctrlX Application Build Environment for ctrlX WORKS

__Important__: 

* The host operating system were the App Builder Environments are running must be Windows 64Bit.

* The user-image-files (see below) can only be build on a Linux system (e.g. an App Builder Environment).

## Concept

__cloud-config-amd64__ and __cloud-config-aarch64__ are so called cloud-config files. These files are compiled into user-data-image files. One for proxy usage, the other for none-proxy usage.

With the script __user-data-img-build-all.sh__ all user-data-image files are build.

Remark: The user-data-image files for aarch64 are kept for future usage.

__Important: The user-data-image files for amd64 are used by ctrlX WORKS.__

For more information see [cloud-init](https://cloud-init.io/)

## Files

### cloud-config Files

Cloud-config files are containing settings (user/password, proxy,...) and a list of packages to be installed on the App Builder Environment:

* cloud-config-amd64    For amd64 VMs
* cloud-config-aarch64  For aarch64 VMs (reserved)

For informations on the content of cloud-config files see [Cloud config examples](https://cloudinit.readthedocs.io/en/latest/topics/examples.html).

### User Image Files

cloud-config file are compiled into image files:

* ubuntu-20.04-server-cloudimg-amd64-user-data-proxy.img    amd64 VM with proxy usage
* ubuntu-20.04-server-cloudimg-amd64-user-data-noproxy.img  amd64 VM without proxy usage
* ubuntu-20.04-server-cloudimg-aarch64-user-data-proxy.img    aarch64 VM with proxy usage
* ubuntu-20.04-server-cloudimg-aarch64-user-data-noproxy.img  aarch64 VM without proxy usage

### Linux Scripts

* user-data-img-build.sh, user-data-img-build-all.sh are used to compile the cloud-config files into user-data-image files.

### Windows Batch Files

* __scp_id_rsa.bat__ copies the id_rsa and id_rsa.pub file from the Windows host to the App Builder Environment guest operating system so that entering password on login is obsolete.

* __ssh-keygen-copy-id.bat__ enables login without password by adding .ssh\id_rsa.pub of the host to ~/.authorized_keys of the QM VM.

* __shrink-qcow2.bat__ shrinks the qcow2 snapshot file.

!!! important
    This action takes some time, do not interrupt.

* wget.bat calls PowerShell to download files.

### Folder install-scripts

Contains shell scripts packed into the cloud-config files.

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

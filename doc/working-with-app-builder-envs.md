This document describes how to work with App Build Environments (QEMU based VMs).

## Best Practice working with App Build Environments

We recommend to use App Build Environments (QEMU VMs) as follows:

1. Create a new App Build Environment with ctrlX WORKS and start it (the newest version of Ubuntu 20.04 LTS is automatically downloaded).
2. After first boot login and install only necessary additional software components using the install scripts in /home/boschrexroth/e.g.: install-sdk.sh, install-deb.sh and install-go.sh
3. We recommend to work with the  free and open source distributed version control system __git__: Create a repository for your project and clone it into your App Buiulder Environment.
4. Create a new branch for your working package.
5. Do your work within this branch, commit and push your changes as often as it makes sense.
6. If your work is done and all changes are pushed remove your App Build Environment.

__Working in such a manner ensures that you are always working with the latest software components and avoids problems with the file size of your App Build Environment. Furthermore creating backups is superfluous.__

## Create a Backup of your App Build Environment

The file system of an App Build Environment is loaded from two files.

The first one is the Ubuntu cloud image file __ubuntu-20.04-server-cloudimg-amd64.img__ originally downloaded from which has a file size of about 600MB. This file is static - no changes are made during runtime or shutdown of your VM. So there is no need to handle it. 

The second one is the so called snapshot file __ubuntu-20.04-server-cloudimg-amd64-snapshot-proxy.qcow2__. This file can be very big (30 GB and more). 
All changes are stored into the snapshot files. Our backup procedure handles this file.

!!! important
    Shutdown your App Build Environment before you backup the qcow2 file.


__Prerequisites:__ The tool qemu-ing is installed by ctrlX WORKS. We assume that ctrlX WORKS is installed in "c:\Program Files\Rexroth\ctrlX WORKS\". If not use your specific installation path to call qemu-ing.

Open the storage location of your App Build Environment.

Rename the actual qcow2 file:

    ren ubuntu-20.04-server-cloudimg-amd64-snapshot-noproxy.qcow2 ubuntu-20.04-server-cloudimg-amd64-snapshot-noproxy.qcow2.bak

Convert the bak file to the new qcow2 file:

    "c:\Program Files\Rexroth\ctrlX WORKS\qemu\qemu-img.exe" convert ubuntu-20.04-server-cloudimg-amd64-snapshot-noproxy.qcow2.bak -O qcow2 ubuntu-20.04-server-cloudimg-amd64-snapshot-proxy.qcow2

Be patient this process takes some times.

The convert process also removes unused blocks - so the new qcow2 file should be smaller than the bak file.

Start the App Build Environment to test the new qcow2 file.
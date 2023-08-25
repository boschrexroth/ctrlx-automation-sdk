This guide shows how to use an [Ubuntu Desktop](https://ubuntu.com/desktop/developers) running in a virtual machine using the [VirtualBox](https://www.virtualbox.org/) on a Windows 10 host system as a development environment to develop ctrlX AUTOMATION apps.

### Prerequisites

* Can only be installed on AMD64/Intel64 host computer systems.
* You need to have admin rights on your host to be able to install the software.

## Install VirtualBox

The software can be downloaded from here: <https://www.virtualbox.org/.>

## Setup Virtual Machine and Install Ubuntu Desktop 22.04 LTS

Download an iso-image from [Ubuntu 22.04.2 LTS](https://releases.ubuntu.com/releases/jammy/)

Create a virtual machine, select these settings:

* min 8 GB Ram
* min. 15 GB Disk, VDI Type
* all processors
* insert Ubuntu *.iso into virtual CD Drive
* Map free internet connection via network settings (Adapter 1, Bridged Adapter)
* Map real/virtual control via network settings (Adapter 2, Bridged Adapter)

Start machine and follow installation instructions.

Install Virtual Box GuestAdditions.

Configure IP Address to connect to your control.

## Running the Virtual Machine

Start a SSH session and login into your virtual machine.

### Setup Corporate Proxy (Optional)

If you are in a corporate environment, which uses a http/https-Proxy, then you may want to set the proxy configuration to be able to download software from within your VM.

For this, you need to run a proxy on your Windows environment and redirect the distribution to this proxy running on localhost. If you are directly connected to a router or the internet, then this step can be skipped.

A recommended proxy program to be used on your Windows host system is <https://github.com/genotrance/px>.

To set the environment variable in the distribution use:

    sudo nano /etc/environment

Add the following variables:

    http_proxy="http://10.0.2.2:3128"
    https_proxy="http://10.0.2.2:3128"
    HTTP_PROXY="http://10.0.2.2:3128"
    HTTPS_PROXY="http://10.0.2.2:3128"

You need to set this configuration also for `wget`.

    sudo nano /etc/wgetrc

Search for the http proxy settings and make sure you configure:

    https_proxy = http://10.0.2.2:3128/
    http_proxy = http://10.0.2.2:3128/
    use_proxy = on

You might want to update your distribution and install essential packages for development.

    sudo apt update
    sudo apt upgrade

### Setup Standard Packages

In QEMU based VMs created by ctrlX WORKS, required packages are installed automatically. 

Here you have to install them manually:

1. Open the according user-data-img file e.g. with Windows notepad. In the text part of this file you will find the section packages (here the first lines only):

    packages:
        - zip
        - unzip
        
 
2. Copy the package list, remove the '-' character and create a comand line e.g.

    sudo apt-get install -y zip unzip ...

!!! important
    Install ALL listed packages.

### Dependencies for Crossbuild (Multiarch)

[Multiarch](https://wiki.ubuntu.com/MultiarchSpec) lets you install library packages from multiple architectures on the some machine. In our case we use the architectures amd64 and arm64.

The according commands are also available in [cloud-config-amd64](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/scripts/environment/cloud-config-amd64).

Therefor copy the commands under section runcmd: subtitle '# https://wiki.ubuntu.com/MultiarchSpec' and '# Qualify architecture'

    sudo dpkg --add-architecture arm64
    ...

!!! important
    Run ALL listed commands.

Furthermore run:

    sudo apt-get -y update
    sudo apt-get -y upgrade
    sudo apt-get -y install libsystemd-dev:arm64
    sudo apt-get -y install libsystemd-dev:amd64
    sudo apt-get -y install libssl-dev:amd64
    sudo apt-get -y install libssl-dev:arm64
    sudo apt-get -y install libzmq3-dev:amd64
    sudo apt-get -y install libzmq3-dev:arm64

### Important Install Scripts

In chapter [Important Install Scripts](install-scripts.md) all install scripts of a QEMU VM created by ctrlX WORKS are explained.

These scripts can be copied from the file [cloud-config-amd64](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/scripts/environment/cloud-config-amd64).

Open this file with a web browser by clicking the link, copy the according lines into new script files on your VM, set the x permission and run them.

Each script file in cloud-config-amd64 is identified with a comment e.g. # install-sdk.sh, skip the line `- content:` and copy all lines until `# Create in /tmp ...`

!!! important
    Running install-sdk.sh is mandatory.

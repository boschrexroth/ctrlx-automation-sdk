# Introduction

This guide shows how to use an [Ubuntu Desktop](https://ubuntu.com/desktop/developers) running in a Virtual Machine in [VirtualBox](https://www.virtualbox.org/) on a Windows host system as a development environment for ctrlX AUTOMATION apps.

## Prerequisites

* Can only be installed on AMD64/Intel64 host computer systems.
* You need admin rights on your host to be able to install the software.

## Install VirtualBox

The software can be downloaded from here: <https://www.virtualbox.org/>

## Setup Virtual Machine and Install Ubuntu Desktop LTS

Download an ISO image from [Ubuntu LTS](https://releases.ubuntu.com/releases/).

Create a Virtual Machine with these settings:

* Min. 8 GB RAM
* Min. 15 GB disk, VDI type
* All processors
* Insert the Ubuntu *.iso into the virtual CD drive
* Map a free internet connection via the network settings (Adapter 1, Bridged Adapter)
* Map the real/virtual control via the network settings (Adapter 2, Bridged Adapter)

Start the machine and follow the installation instructions.

Install the VirtualBox Guest Additions.

Configure the IP address to connect to your control.

## Running the Virtual Machine

Start an SSH session and log in to your Virtual Machine.

### Setup Corporate Proxy (Optional)

If you are in a corporate environment that uses an HTTP/HTTPS proxy, you may want to set the proxy configuration to be able to download software from within your VM.

For this, you need to run a proxy on your Windows environment and redirect the distribution to this proxy running on localhost. If you are directly connected to a router or the internet, this step can be skipped.

A recommended proxy program for your Windows host system is <https://github.com/genotrance/px>.

To set the environment variables in the distribution, use:

    sudo nano /etc/environment

Add the following variables:

    http_proxy="http://10.0.2.2:3128"
    https_proxy="http://10.0.2.2:3128"
    HTTP_PROXY="http://10.0.2.2:3128"
    HTTPS_PROXY="http://10.0.2.2:3128"

You also need to set this configuration for `wget`.

    sudo nano /etc/wgetrc

Search for the HTTP proxy settings and make sure you configure:

    https_proxy = http://10.0.2.2:3128/
    http_proxy = http://10.0.2.2:3128/
    use_proxy = on

You might want to update your distribution and install essential packages for development.

    sudo apt update
    sudo apt upgrade

### Setup Standard Packages

In QEMU-based VMs created by ctrlX WORKS, the required packages are installed automatically.

Here you have to install them manually:

1. Open the corresponding user-data-img file, e.g. with Windows Notepad. In the text part of this file you will find the section 'packages' (only the first lines are shown here):

    packages:
        - zip
        - unzip

2. Copy the package list, remove the '-' characters and create a command line, e.g.:

    sudo apt-get install -y zip unzip ...

> !!! important: Install ALL listed packages.

## Install the ctrlX AUTOMATION SDK

See [Install Sources from GitHub](install-sources-from-github.md).
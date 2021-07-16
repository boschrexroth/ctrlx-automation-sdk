
# Setup ctrlX AUTOMATION Development Environment using Windows WSL

## Overview

This guide shows how to use the Windows Subsystem for Linux (WSL) as an development environment to develop ctrlX AUTOMATION Apps. It allows you to use Visual Studio Code for Windows as your IDE and to compile your program in the WSL running on Windows. This setup gives you a seamless development environment for Linux applications from within you Windows environment.
This guide is based on Microsoft WSL1. 

For more information about WSL see: <https://docs.microsoft.com/en-us/windows/wsl/>

### Prerequisites

* You need to have admin rights on your Windows system to be able to install WSL.
* Windows 10 with all updates.

## Install WSL

Enable Admin access and run PowerShell with Administrator privileges.

    Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Windows-Subsystem-Linux

Follow the instructions and reboot. For troubleshooting, see the official Microsoft documentation: <https://docs.microsoft.com/de-de/windows/wsl/install-win10>  

## Install Distribution (Ubuntu 18.04)

Download your desired Distribution (Ubuntu 18.04 recommended) from <https://docs.microsoft.com/en-us/windows/wsl/install-manual> and install the archive by executing it from the windows host.

Wait for the application to install.

### Setup Corporate Proxy (Optional)

If you are in a corporate environment, which uses a http-Proxy, then you may want to set the proxy configuration to be able to download software from within your distribution.
For this, you want to run a proxy on your Windows environment and redirect the distribution to this proxy running on localhost. If you are directly connected to a router or the internet, then this step can be skipped.

A recommended proxy program to be used on your Windows host system is px (<https://github.com/genotrance/px>). Get the latest release of Px for Windows <https://github.com/genotrance/px/releases>. Once the download is finished, run px.exe directly from windows command prompt. Hint: Use flag `--install` to add Px to the Windows registry to run on startup.

To set the environment variable in the distribution use:

    sudo nano /etc/environment

Add the following variables:

    http_proxy="http://127.0.0.1:3128"
    https_proxy="http://127.0.0.1:3128"
    HTTP_PROXY="http://127.0.0.1:3128"
    HTTPS_PROXY="http://127.0.0.1:3128"

You need to set this configuration also for `wget`.

    sudo nano /etc/wgetrc

Search for the http proxy settings and make sure you configure:

    https_proxy = http://127.0.0.1:3128/
    http_proxy = http://127.0.0.1:3128/
    use_proxy = on

### Setup Packages

You might want to update your distribution and install essential packages for development.

    sudo apt update
    sudo apt upgrade
    sudo apt install ssh zip unzip git curl cifs-utils apt-transport-https ca-certificates openssh-client

The following packages are needed for software development of ctrlX AUTOMATION apps.

    sudo apt install crossbuild-essential-arm64 cmake snapcraft gcc gdb

For Python install the additional python packages.

    sudo apt-get install python3
    sudo apt-get install python3-pip

Furthermore, we recommend to use virtual python environments when you are developing python apps. Therefore, you need to install virtualenv:

    sudo apt-get install virtualenv

### Dependencies for Crossbuild (Multiarch)

Multiarch lets you install library packages from multiple architectures on the some machine. This is useful in various ways, but the most common is installing 64 and 32-bit software on the same machine and having dependencies correctly resolved automatically. In our way, we use multiple architectures. In general, you can have libraries of more than one architecture installed together and applications from one architecture or another installed as alternatives. Additional information can be found here [Multiarch](https://wiki.ubuntu.com/MultiarchSpec).

Be sure that apt can connect to the internet.

Run the commands below as root user:


    sudo dpkg --add-architecture arm64
    sudo sh -c 'echo "deb [arch=arm64] http://ports.ubuntu.com/ bionic main restricted universe" >> /etc/apt/sources.list.d/multiarch-libs.list'
    sudo sh -c 'echo "deb [arch=arm64] http://ports.ubuntu.com/ bionic-updates main restricted universe" >> /etc/apt/sources.list.d/multiarch-libs.list'
    sudo apt-get clean
    sudo apt-get update

  
After apt-get update you might get some warnings like: `Failed to fetch http://de.archive.ubuntu.com/ubuntu/dists/bionic/universe/binary-arm64/Packages`.
To avoid it add the vendor with the host architecture `[arch=i386,amd64]` to all sources of your sources list `/etc/apt/sources.list`.

For example:
`deb [arch=i386,amd64] http://de.archive.ubuntu.com/ubuntu/ bionic main restricted`

Now it is possible to download packages for other architectures. For building the examples, you need to install additional libraries:


    sudo apt-get install libsystemd-dev:arm64
    sudo apt-get install libsystemd-dev:amd64
    sudo apt-get install libssl-dev:amd64
    sudo apt-get install libssl-dev:arm64


## Install Visual Studio Code

Install Visual Studio Code on your windows environment as described here: https://code.visualstudio.com/

### Setup WSL Remote Compilation

In order to be able to develop in Windows, but to compile in the WSL you need to install the following extension locally in VS Code: 
https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack

To compile all examples of the SDK you will need to install additional extensions on the remote backend. 
This can be done easily from within VS Code as soon as you open a project. In the extension windows you can press `install in WSL`.

![vscode_remote_extensions](./images/vscode_remote_extensions.png)

The extensions you need to install depending on the programming language you want to use. 
Go to the sample subpage of this documentation to find the required extensions of your preferred programming language.

## Install the SDK

Copy the complete contents of this SDK into your WSL system. It is recommended to copy the SDK to the `home` directory of your user. For example: `cp -r /mnt/c/sdk.control ~/`

## Open a Project

### Open from the WSL terminal 

Opening a folder inside the Windows Subsystem for Linux in VS Code is very similar to opening up a Windows folder from the command prompt or PowerShell.

1. Open a WSL terminal window (using the start menu item or by typing `wsl` from a command prompt / PowerShell).
2. Navigate to a folder you'd like to open in VS Code. For example: `cd ~/sdk.control/public/samples-cpp/hello.world` 
3. Type `code` in the terminal. When doing this for the first time, you should see VS Code fetching components needed to run in WSL. This should only take a short while and is only needed once.

### Open VS Code

1. Start VS Code.
2. Press F1, select Remote-WSL: New Window for the default distro or Remote-WSL: New Window using Distro for a specific distro.
3. Use the File menu to open your folder.

# Troubleshooting

## PermissionError: [Errno 13] Permission denied when calling snapcraft

When calling snapcraft in WSL1, there might occur errors regarding file permissions. This happens, when snapcraft is called from a directory which is mounted from the windows host file system (e.g. `mnt/c/`).
This is a known problem with the WSL1 files system implementation and will probably be fixed with Microsoft WSL2. Meanwhile, the only solution is to call snapcraft from a subdirectory within your home directory. Therefore
you might want to copy the sdk to your home directory and compile the projects from there.

    mkdir -p ~/ctrlx_sdk && cp -r /mnt/c/path/to/sdk ~/ctrlx_sdk

## No connection using pip3 install behind proxy

Within the WSL there is a bug using the environment Variables like http_proxy and https_proxy.

It is possible to solve this with a modified ~/.bashrc
Add the proxy variables to the .bashrc file in your home directory:

    export http_proxy="http://127.0.0.1:3128"
    export https_proxy="http://127.0.0.1:3128"
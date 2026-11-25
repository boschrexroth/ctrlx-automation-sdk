# Introduction

This chapter explains how to set up and run an App Build Environment without using ctrlX WORKS.

Please note that ctrlX WORKS already provides the tools and steps needed to manage App Build Environments.

This is the recommended setup:

* Although QEMU works on both Windows and Linux, we recommend using a Windows host operating system.
* The host and guest systems should use the amd64 CPU architecture. Snap packages for arm64 targets are created using cross-build.
* The instructions below assume that the ctrlX AUTOMATION SDK is installed in the directory **/home/boschrexroth/ctrlx-automation-sdk** or in the shorter form **~/ctrlx-automation-sdk**.

## Installation on Windows

### QEMU on Windows

Download and install the latest version of QEMU for Windows from [https://qemu.weilnetz.de/w64/](https://qemu.weilnetz.de/w64/).

>!!! important
> Add the QEMU installation folder to the system PATH environment variable.
>
> See [How do I set or change the PATH system variable?](https://www.java.com/en/download/help/path.html)

### Install Windows Hypervisor Platform

We recommend enabling **Windows Hypervisor Platform**:

* Press **Windows + S** and search for **Turn Windows features on or off**.
* Select **Windows Hypervisor Platform**. Do not select **Hyper-V**.
* Click **OK**.
* After enabling this feature, restart the system.

**Hints:**

* You can also enable or disable **Windows Hypervisor Platform** from the command line:
    * Open **Command Prompt** as an administrator.

            dism /online /Enable-Feature /FeatureName:HypervisorPlatform /All
            dism /online /Disable-Feature /FeatureName:HypervisorPlatform

* If you use VirtualBox, enable the internal Hyper-V option for each Virtual Machine.

### Install Px Proxy

See [Px Proxy](./px.md).

## Installation on Linux

You must install both QEMU and KVM on your host system.
You can do this by running the following script:

    ~/ctrlx-automation-sdk/scripts/environment/install-qemu.sh

## Running the QEMU Virtual Machine

### Create an Instance of a QEMU Virtual Machine

Follow these steps:

* Create a new folder on a disk that has enough free space.
* Copy the contents of the SDK folder **~/ctrlx-automation-sdk/scripts/environment** into your instance folder.

**Hint:** You can also download the folder here:

[~/ctrlx-automation-sdk/scripts/environment/](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts/environment)

### Start the QEMU Virtual Machine

To start the QEMU VM instance, open the installation folder and run one of these scripts:

* **launch-amd64-noproxy (.bat, .sh)**: use this if the VM has direct internet access
* **launch-amd64-proxy (.bat, .sh)**: use this if the VM must use a proxy server on the host computer

The VM starts in a console window, where you can see the startup output.

We recommend not using this console directly because many shell functions are not supported there.

During the first startup, a lot of software is installed, so please be patient.

When the setup is complete, the VM shuts down automatically. Start it again by running the launch script.

### Network Connection to your QEMU Virtual Machine

From the host computer's point of view, the VM does not have its own network interface card. Services running on the VM are available through **port forwarding**.

This port forwarding is configured in the launch script with one or more **hostfwd=** entries, for example:

    -netdev user,id=eth0,hostfwd=tcp::10022-:22

This forwards port 10022 on the host to port 22 (SSH) on the VM. If port 10022 is already in use on the host, the VM will not start. In that case, choose a free host port number in the launch script, for example 20022.

To forward additional ports, such as **502 (Modbus)**, extend the line as follows:

    -netdev user,id=eth0,hostfwd=tcp::10022-:22,hostfwd=tcp::502-:502

> !!! Important: From the host computer, you can connect to the VM over SSH using **127.0.0.1:10022**.

For the first connection in Visual Studio Code, enter **ssh -p 10022 boschrexroth@127.0.0.1**. The password is **boschrexroth**.

### Shutdown

It is important to shut down the **QEMU VM** correctly. Use the VM console and run the following command:

    sudo poweroff

## QEMU Virtual Machine Networking

QEMU creates a virtual network between the host operating system and the Virtual Machine.

> !!! important: This virtual network is visible only to the Virtual Machine, not to the host operating system.

QEMU also provides a gateway, a DNS server, and an optional SMB server for the VM.

By default, this virtual network uses the subnet 10.0.2.0/24. From the QEMU VM's point of view, the following IP addresses are assigned:

* 10.0.2.15: the VM itself
* 10.0.2.2: the gateway and **host operating system**
* 10.0.2.3: the DNS server
* 10.0.2.4: the SMB server

For more information, see [QEMU Documentation/Networking](https://wiki.qemu.org/Documentation/Networking).

## Troubleshooting and Tips

### Performance

* On Windows hosts, make sure **Windows Hypervisor Platform** is enabled.
* Close other Virtual Machines on your host to avoid performance issues.
* An emulated _AARCH64_ QEMU VM is slow, so please be patient.

### HTTP/HTTPS Access

Check whether your host can access the internet over HTTP/HTTPS, especially the Ubuntu sites. For example:

    wget -p http://ports.ubuntu.com/ubuntu-ports

If this fails, check your DNS settings and server configuration.

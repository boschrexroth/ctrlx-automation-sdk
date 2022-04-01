# Quick Start Guide To Building a Snap from a SDK Sample Project 

## Introduction

With the help of this guide you will be able to build your first snap on one of the example projects of this SDK in just a few steps.

## Prerequisites

* Install ctrlX WORKS with function 'App Build Environment' on Windows your host computer
 
* If your Windows your host computer is behind a proxy server, install and run [PX.exe](px.md). 

## Building a Snap

Building a snap from a SDK sample project can be done in a few steps:
  
* __From ctrlX WORKS create and start a new App Build Environment (QEMU VM) instance.__

Wait until the VM is shutting down automatically after the initialization phase. Then restart your VM.
For more information see [ctrlX WORKS Feature App Build Environment](setup_qemu_ctrlx_works.md).

* __When ctrlX WORKS enables the SSH link click this link to start a SSH session.__

On password prompt enter boschrexroth

* __Install the latest ctrlX AUTOMATION SDK from github__

From the home directory call this script:

    ./install-sdk.sh

* __Change to project folder /home/boschrexroth/ctrlx-automation-sdk/samples-cpp/datalayer.register.node__

* __Build one or both snaps__

To build a snap for a ctrlX CORE <sup>virtual</sup> enter:

    ./build-snap-amd64.sh

To build a snap for a ctrlX CORE M3/4 enter:

    ./build-snap-arm64.sh

* __From your Windows host copy the snaps to your local file system__

Start cmd.exe and enter this command:

    scp -P 10022 boschrexroth@localhost:~/ctrlx-automation-sdk/samples-cpp/datalayer.register.node/*.snap .

* __Login into your ctrlX CORE and install the according snap__

Don't forget to set option 'Allow installation from unknown source' under Apps --> Settings

* __Select Settings --> Data Layer and check the nodes under path sdk-cpp-registernode/__

There should appear several nodes created by the snap.

!!! important
    To setup a complete tool set to develop own applications (snaps) see following chapters:

* [Visual Studio Code](vscode.md)

* [SSH Connection](ssh_connection.md)

* [All sample projects](samples.md)


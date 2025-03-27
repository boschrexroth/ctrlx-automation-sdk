# ctrlX App Build Environment Ubuntu 22.04

## Introduction

This folder contains bat and image files to launch an App Build Environment with ctrlX WORKS under Windows operating system.
An App Build Environment is a Ubuntu Server guest operating running within a [QEMU](https://www.qemu.org/) virtual machine.

The installation and configuration of the Ubuntu Server guest operating system is automatically done
on first boot by the build-in Ubuntu Cloud-Init Technology. For this purpose, a so-called user image file is specified when the App Build Environment is started.
This file contains the compiled information of a cloud-config file which is processed by the cloud-init service.

For more informations see [cloud-init](https://cloud-init.io/)

## Creating an App Build Environment Instance

An instance of an App Build Environment is created by ctrlX WORKS.

ctrlX WORKS copies a set of files into the specific destination directory. Depending on the option __Use HTTP and HTTPS proxy on localhost:3128__ a user-data-image file is used with or without proxy server information.

## Working with an App Build Environment

### Starting an App Build Environment for the first time

An new created App Build Environment instance can be launched by clicking the '>' icon in ctrlX WORKS.

Each time ctrlX WORKS generates the batch file start.bat.
Here the Windows environment variable PATH is extended so that the ctrlX WORKS internal QEMU installation will be used. Then launch.bat is called.

launch.bat downloads the Ubuntu Server image, creates a snapshot file (.qcow2) and launches the QEMU executable. The image file remains unchanged, all changes are saved in the snapshot file.

During startup the App Build Environment will be configured by the Ubuntu Cloud-Init Technology. Therefor the cloud-config file contained in the user-data-image file is used.

In this initialization phase login is not possible.

When this phase is finished, the guest operating system will automatically shut down and has to be restarted.

### Restarting an App Build Environment

The restart is initiated by clicking the '>' icon on the ctrlX WORKS UI.

start.bat is called and the App Build Environment will start directly into the run phase.

Logging in - directly or via SSH - is enabled.

### Logging in

We recommend to login in via SSH - using a RSA public / private key pair.

A SSH session can be started by clicking the SSH link in ctrlX WORKS. Then open.bat will be created and called.

### Shutdown

The shutdown of the App Build Enviroment can triggered externally by clicking the stop icon in ctrlX WORKS. Then stop.bat is called, the guest operating system will shutdown and the App Build Environment will stop.

### Deleting an App Build Environment

Stop the App Build Enviroment and click the waste bin icon in ctrlX WORKS.

### Working with Snapshots

An App Build Enviroment always works with a base file and a snapshot file. The base file is never changed, all changes are stored into the snapshot file.

When a new App Build Enviroment was created the ubuntu server image file (e.g. ubuntu-22.04-server-cloudimg-amd64.img) is the base file.

`ubuntu-22.04-server-cloudimg-amd64.qcow2` is always the snap shot file.

#### Creating a new Snapshot

Creating a new snapshot file makes sense if you want to save a certain status of your App Build Environment.

To do this just call:

    snapshot.bat

The bat file converts the actual snapshot file into a base file `ubuntu-22.04-server-cloudimg-amd64-0.qcow2` and creates a new snapshot file `ubuntu-22.04-server-cloudimg-amd64.qcow2` will be created.

This can be done several times, for each repetition the number in the base file name will be increased: 0, 1, 2, etc.

#### Restoring a Snapshot

You can restore an older snapshot manually:

* Delete the actual snapshot file (or move to another direcory): `ubuntu-22.04-server-cloudimg-amd64.qcow2`
* Rename the base file you want to use into `ubuntu-22.04-server-cloudimg-amd64.qcow2`

#### Increase the Virtual Size of the Snapshot File

Per default a snapshot file has the virtual size of 32GB.
In case you want to increase this just call:

	increase-virtual-size.bat

The virtual size is increased by 10GB.

You can repeat this procudure if needed.

## Windows Helper File wget.bat

This batch file is used to download the Ubuntu Server image. Internally is uses the Windows PowerShell.

!!! important
    If using PowerShell is not possible this bat file can alternatively use Wget for Windows - see further instructions in [wget.bat](wget.bat).

## Hints

For the full set of files to run App Build Enviroments see the SDK folder [scripts/evironment](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts/environment)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

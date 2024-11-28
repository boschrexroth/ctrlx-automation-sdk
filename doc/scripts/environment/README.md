# Building and Providing a ctrlX Application Build Environment for ctrlX WORKS

__Important__:

* We recommend to use __Windows amd64__ as host operating system to create and run App Build Environments.

* The user-image-files (see below) can only be build on a Linux system (e.g. within an App Build Environment).

## Concept

Four different types of App Build Environments are supported:

1. amd64 architecture with proxy
2. amd64 architecture with NO proxy
3. aarch64 architecture with proxy
4. aarch64 architecture with NO proxy

An App Build Environment is a Ubuntu Server operating system installed with the help of the build-in cloud-init service. For more information see [cloud-init](https://cloud-init.io/).

The configuration is provided by a so called cloud-config file.

For informations on the content of cloud-config files see [Cloud config examples](https://cloudinit.readthedocs.io/en/latest/topics/examples.html).

For all types of App Build Environments one single configuration file is used: __cloud-config__.

Type specific lines within this file are marked by these patterns:

* #amd64-ONLY: Only relevant for amd64 architecture
* #aarch64-ONLY: Only relevant for aarch64 architecture
* #proxy-ONLY: Only if a proxy server is used
* insert-????-here: Indicates a text passage where during the build process a special text is inserted

## Building User Data Image Files

During the build process the type specific configuration file is generated, compiled (by the tool cloud-localds) and provided in a so called user-data-image file:

* builder/user-data-amd64-proxy.img     amd64 VM with proxy usage
* builder/user-data-amd64-noproxy.img   amd64 VM without proxy usage
* user-data-aarch64-proxy.img           aarch64 VM with proxy usage
* user-data-aarch64-noproxy.img         aarch64 VM without proxy usage

__Important:__

The user-data-image files for amd64 are stored in the sub folder `builder`. This folder contains also other files needed by ctrlX WORKS to create and start App Build Enviroments.

Hint: The user-data-image files for aarch64 are kept for future usage.

### Build Scripts

* __build-0-user-data-img.sh__ is the build over all script. __Start this script to run the whole build process.__
* build-1-cloud-configs.sh generates the architecture specific config files: cloud-config-aarch64-generated, cloud-config-amd64-generated
* build-2-user-data-img-all.sh initiates the build off all user data images files calling calls build-9-user-data-img.sh
* calls build-9-user-data-img.sh: Creates a single regarding the provided architecture and proxy settings

__replace-text.py__ is as simple text search and replace Python script which is able to handle special characters in both search and replace pattern.

### Helper Scripts

These helper scripts are packed into the user-data-image files. They are started within the (new) App Build Environmant:

* __check-cloud-init-log.sh__ is called from .bashrc and checks the cloud-init log for errors
* __install-sdk-from-github.sh__ should be used to download and install the ctrlX AUTOMATION SDK from github

__Important:__
During the installation process helper scripts of the SDK are installed into the folder __/home/boschrexroth/scripts/__.

## Using Ubuntu based Linux as Host Operating system

### Install QEMU and KVM

Start the script:

	install-qemu.sh

Ensure your host has internet access.

### Create an App Build Environment

Copy these files into a separate working directory:

* launch.sh

If your host has direct internet access:

* launch-amd64-noproxy.sh
* builder/user-data-amd64-noproxy.img

If your host uses a proxy (port must be 3128)

* launch-amd64-proxy.sh
* builder/user-data-amd64-proxy.img

### Start an App Build Environment

Change into your working directory.

If your host has direct internet access, enter:

	launch-amd64-noproxy.sh

If your host uses a proxy, enter:

	launch-amd64-proxy.sh

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

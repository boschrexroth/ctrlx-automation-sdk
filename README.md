# Software Development Kit for ctrlX AUTOMATION

This is the software development kit (SDK) for [ctrlX AUTOMATION](https://www.ctrlx-automation.com). It can be used to program Apps for ctrlX CORE. 
Browse through the manual via: <https://boschrexroth.github.io/ctrlx-automation-sdk>

## Important directions for use

### General Terms of Use

In order to download and use the binary packages of the ctrlX AUTOMATION Software Development Kit you have to accept the [Terms and Conditions for the Provision of Products of Bosch Rexroth AG Free of Charge](https://dc-corp.resource.bosch.com/media/xc/homepage/TC_for_provision_of_products_free_of_charge.pdf)

### Areas of use and application

The content (e.g. source code and related documents) of this repository is intended to be used for configuration, parameterization, programming or diagnostics in combination with selected Bosch Rexroth ctrlX AUTOMATION devices.
Additionally, the specifications given in the "Areas of Use and Application" for ctrlX AUTOMATION devices used with the content of this repository do also apply.

### Unintended use

Any use of the source code and related documents of this repository in applications other than those specified above or under operating conditions other than those described in the documentation and the technical specifications is considered as "unintended". Furthermore, this software must not be used in any application areas not expressly approved by Bosch Rexroth.

## Installation

__To develop ctrlX Apps we recommend to use a ctrlX App Build Enviroment.__ Otherwise a Ubuntu Server or Desktop system is needed.
### Using a ctrlX App Build Enviroment

How to create and start a ctrlX App Build Enviroment is described in [ctrlX WORKS App Build Environment](https://boschrexroth.github.io/ctrlx-automation-sdk/setup_qemu_ctrlx_works.html)

If your ctrlX App Build Enviroment is running, you can log in and install the ctrlX AUTOMATION SDK - see below.

### Using Ubuntu Server or Desktop system as development environment

If you do not want to use the ctrlX App Build Enviroment, you will need a __Ubuntu Server or Desktop system.__

In this case two important preparations steps have to be done, before you are able to install and use the ctrlX AUTOMATION SDK.

__Step 1, if your system has no direct internet access:__
Register HTTP and HTTPS proxy server URLs in /etc/wgetrc and /etc/environment

This can be done manually, but we recommend to use a script provided by the ctrlX AUTOMATION SDK:

Start a web browser (outside of your development environment), download [install-proxy.sh](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/scripts/install-proxy.sh) and copy the script to your development environment.

On your development environment, make it executable and start it:

    chmod a+x install-proxy.sh
    ./install-proxy.sh

The script will ask for the according URLs and make entries into the files mentioned above.

After registering the proxy URLs log out and log in again to make the settings effective.

__Step 2 Install required Debian packages:__

Open a shell within your Ubuntu system (be shure it has internet access - with or without proxy) and enter:

    curl -s https://raw.githubusercontent.com/boschrexroth/ctrlx-automation-sdk/main/scripts/environment/install-packages.sh | bash

### Installation of the ctrlX AUTOMATION SDK

The SDK should be installed using the provided script 'clone-install-sdk.sh'.

The script will clone this repository, checkout a specific version and create a branch. So during the script processing you will be asked for these values. 

After cloning, the appropriate zip archive will be downloaded and all required files will be copied into the local repository.

To start the installation open a shell on your development system, change to the directory into which the SDK should be installed and enter:

    curl -s https://raw.githubusercontent.com/boschrexroth/ctrlx-automation-sdk/main/scripts/clone-install-sdk.sh | bash 

As a result, your local copy of the github repo is stored within the directory ctrlx-automation-sdk/

Now you are able to build the sample projects or develop Apps with own projects.

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

## About

Please note that any trademarks, logos and pictures contained or linked to in this Software are owned by or copyright © Bosch Rexroth AG 2021 and not licensed under the Software's license terms.

<https://www.boschrexroth.com>

Bosch Rexroth AG  
Bgm.-Dr.-Nebel-Str. 2  
97816 Lohr am Main  
GERMANY

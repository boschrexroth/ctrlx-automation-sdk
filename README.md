# Software Development Kit for ctrlX AUTOMATION

This is the software development kit (SDK) for [ctrlX AUTOMATION](https://www.ctrlx-automation.com). It can be used to program Apps for ctrlX CORE. 

Browse through the manual via: [ctrlX AUTOMATION Software Development Kit](https://boschrexroth.github.io/ctrlx-automation-sdk)

## Important directions for use

### General Terms of Use

In order to download and use the binary packages of the ctrlX AUTOMATION Software Development Kit you have to accept the [Terms and Conditions for the Provision of Products of Bosch Rexroth AG Free of Charge](https://dc-corp.resource.bosch.com/media/xc/homepage/TC_for_provision_of_products_free_of_charge.pdf)

### Areas of use and application

The content (e.g. source code and related documents) of this repository is intended to be used for configuration, parameterization, programming or diagnostics in combination with selected Bosch Rexroth ctrlX AUTOMATION devices.
Additionally, the specifications given in the "Areas of Use and Application" for ctrlX AUTOMATION devices used with the content of this repository do also apply.

### Unintended use

Any use of the source code and related documents of this repository in applications other than those specified above or under operating conditions other than those described in the documentation and the technical specifications is considered as "unintended". Furthermore, this software must not be used in any application areas not expressly approved by Bosch Rexroth.

## Installation of the App Build Environment

__To develop ctrlX Apps we recommend to use a ctrlX App Build Environment.__ Otherwise a Ubuntu Server or Desktop system is needed.

How to create and start a ctrlX App Build Environment is described in [ctrlX WORKS App Build Environment](https://boschrexroth.github.io/ctrlx-automation-sdk/setup_qemu_ctrlx_works.html)

If your ctrlX App Build Environment is running, you can log in and install the ctrlX AUTOMATION SDK - see below.

## Installation of the ctrlX AUTOMATION SDK

__These installation steps are required on both an App Build Environment and an Ubuntu Server or Desktop System.__

### Clone and Install the ctrlX AUTOMATION SDK

Start a console session, change to your destination directory and enter:

	wget https://raw.githubusercontent.com/boschrexroth/ctrlx-automation-sdk/main/scripts/clone-install-sdk.sh && chmod a+x *.sh && ./clone-install-sdk.sh

As a result, your local copy of the github repo is stored within the directory ctrlx-automation-sdk/

### Install required Debian packages

Stay in the directory and enter:

	ctrlx-automation-sdk/scripts/install-required-packages.sh

### Install snapcraft

Stay in the directory and enter:

	ctrlx-automation-sdk/scripts/install-snapcraft.sh

### Install ctrlx-datalayer Debian package

Stay in the directory and enter:

	ctrlx-automation-sdk/scripts/install-ctrlx-datalayer.sh ctrlx-automation-sdk/deb

### Building Sample Projects

Now you are able to build the C++ and Python sample projects.

To build sample projects in other programming languages further installation steps are required.

Change to the directory `ctrlx-automation-sdk/scripts` and start the according install script.

For go samples:

	install-go.sh

For .NET samples:

	install-dotnet-sdk.sh

For nodejs samples:

	install-nodejs-npm.sh

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

## About

Please note that any trademarks, logos and pictures contained or linked to in this Software are owned by or copyright © Bosch Rexroth AG 2021-2024 and not licensed under the Software's license terms.

<https://www.boschrexroth.com>

Bosch Rexroth AG  
Bgm.-Dr.-Nebel-Str. 2  
97816 Lohr am Main  
GERMANY

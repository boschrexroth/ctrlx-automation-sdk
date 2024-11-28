# Sub folder 'scripts'

This folder contains several helper scripts to install the ctrlX AUTOMATION SDK, required Debian packages and to build sample projects.

## SDK Installation Script Files

* __clone-install-sdk.sh__ clones the ctrlX AUTOMATION SDK git repository from github, downloads the according zip archiv and installs the required files into the cloned github repository.

## Installation of Required Packages

* __install-required-packages.sh__  installs required packages to develop apps with the ctrlX AUTOMATION SDK
* __install-snapcraft.sh__ installs snapcraft to build snaps
* __install-ctrlx-datalayer.sh__ installs the ctrlx-datalayer Debian package and registers its storage directory as local package source

## Installation Scripts for Programming Languages

* __install-go.sh__ downloads and installs required packages to develop snaps with go
* __install-dotnet-sdk.sh__ downloads and installs required packages to develop snaps with dotnet
* __install-nodejs-npm.sh__ downloads and installs required packages to develop snaps with Node.js

## ctrlX OS Development Tools

* __install-ctrlx-os-dev-tools.sh__ downloads and installs required packages to build a ctrlX OS image

## Building a Sample Project and uploading the Snap

* __build-upload-log-snap.sh__ Build a sample project, upload the snap and redirect log printout of the snap

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MITREADME.md

# Sub folder 'scripts'

This folder contains several helper scripts to install the ctrlX AUTOMATION SDK, programming language dependend packages and to build sample projects.

## SDK Installation Script Files 

* __clone-install-sdk.sh__ clones the ctrlX AUTOMATION SDK git repository from github, downloads the according zip archiv and installs the required files into the cloned github repository. 

* __install-snapcraft.sh__ installs snapcraft to build snaps

## Installation Scripts for Programming Languages

* __install-go.sh__ downloads and installs required packages to develop snaps with go    
* __install-dotnet-sdk.sh__ downloads and installs required packages to develop snaps with dotnet
* __install-nodejs-npm.sh__ downloads and installs required packages to develop snaps with Node.js
* __install-cpp-aarch64-libs.sh__ downloads and installs required aarch64 libraries for cross build - __for C++ projects only.__ 

## ctrlX OS Development Tools

* __install-ctrlx-os-dev-tools.sh__ downloads and installs required packages to build a ctrlX OS image

## Building a Sample Project and uploading the Snap

* __build-upload-log-snap.sh__ Build a sample project, upload the snap and redirect log printout of the snap


## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MITREADME.md

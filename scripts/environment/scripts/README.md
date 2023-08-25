# Sub folder 'scripts'

This folder contains scripts to install the ctrlX AUTOMATION SDK and programming language dependend packages to develop apps.

## Script Files

* __install-sdk.sh__ downloads ctrlX AUTOMATION SDK from github and installs it. Calls internally install-snapcraft.sh and uses download-sdk.sh
* __install-go.sh__ downloads and installs required packages to develop snaps with go    
* __install-dotnet-sdk.sh__ downloads and installs required packages to develop snaps with dotnet
* __install-nodejs-npm.sh__ downloads and installs required packages to develop snaps with Node.js
* __install-snapcraft.sh__ installs snapcraft to build snaps
* __install-ctrlx-datalayer-deb.sh__ installs the content of the package ctrlx-datalayer.deb which is used for ctrlX Data Layer connections and registers the folder from which the script is called as local source. 
* __install-cpp-aarch64-libs.sh__ downloads and installs required aarch64 libraries for cross build - __for cpp projects only.__ 
* __install-ctrlx-os-dev-tools.sh__ downloads and installs required packages to build a ctrlX OS image
* download-sdk.sh Helper script to download the ctrlX AUTOMATION SDK zip archive

## Sub folder 'cloud-config'

This folder contains the origin cloud-config file. The information provided here can be used to setup a custom specific build environment.

__Important__: Settings/packages for amd64 based environments only are marked with '#amd64-ONLY', for arm64 (aarch64) based only with '#aarch64-ONLY'.


## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
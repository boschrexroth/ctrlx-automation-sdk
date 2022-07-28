## Software Development Kit for ctrlX AUTOMATION

### Version 1.16.0 <small> July 15, 2022</small>


!!! important
    The newest App Builder Environment is always provided with ctrlX WORKS. 

Only the AMD64 version is supported because for every programing language supported by the ctrlX AUTOMATION SDK cross build capability is provided.

### Common

* hostname: app-builder-amd64
* User boschrexroth is created in the last step of the setup workflow
* Poweroff instead of reboot if setup is finished

### Installation scripts

* __install-dotnet-sdk.sh__: Install as Debian package
* __install-nodejs-npm.sh__: Add proxy infos to ~/.npmrc
* __install-sdk.sh__: Install Debian package ctrlx-datalayer-*.deb


### Additionally installed packages

* __ssh, curl__: Removed because they are installed in the base image.
* __build-essential__: New

### Version 1.14.0 <small> March 15, 2022</small>
### Installation scripts

* __install-snapcraft.sh__: Improve robustness
* __install-nodejs-npm.sh__: Install as snap
* __install-go.sh__: Install as snap
* __install-go.sh__: New

### Additionally installed packages

* __pkg-config__: New
* __libzmq3-dev:amd64__: New
* __libzmq3-dev:arm64__: New

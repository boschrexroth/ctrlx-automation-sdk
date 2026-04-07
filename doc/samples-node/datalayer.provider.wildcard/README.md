# README Node.js datalayer.provider.wildcard

This Node.js sample demonstrates dealing with a _large_ set of nodes (big data provider) or any _dynamic node hierarchy by listening to any _wildcarded address_ ( e.g. myRoot/**) and using _self-managed virtual nodes_, just returned in _OnBrowse()_ method, which is efficient and light-weight.

__IMPORTANT__:
Please read the _Best Practise_ section in common Node.js documentation for more details and recommended samples.

## Introduction

The sample demonstrates how to provide a large set of light-weight self-managed nodes by registering a wildcarded address to __ctrlX Data Layer__.
The provider creates nodes with types of int/double/string.

## Description

The sample demonstrates

+ How to provide nodes on a wildcarded address to __ctrlX Data Layer__.

+ How to create and provide meta data to nodes.

+ How to provide nodes with read-only or read-write support.
  
## Prerequisites

Please read the common Node.js docs [here](./../README.md), first.

## Debug and Run

1. Run 'npm install' in the terminal.  
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder in QEMU VM__.
4. Open the directory __datalayer.provider.wildcard__.
5. Build and install snap as described [here](./../README.md).
6. Checkout __ctrlX Data Layer__ web frontend (Settings | Data Layer)
7. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.sdk-node-provider-wildcard.app.service__
   + Now you should the see the app output diagnostics.
   + Press __Refresh__ to update.

8. If you have root permissions, you can also watch the output on any ssh console with the command:

   ```bash
   sudo snap logs sdk-node-provider-wildcard.app -f | more
   ```

## Install the App

Login into ctrlX and install the App (Apps).

## Gratulations - We're finished - Let's start coding

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/)

### SDK Forum

Please visit the [SDK Forum](https://developer.community.boschrexroth.com/t5/ctrlX-AUTOMATION/ct-p/dcdev_community-bunit-dcae/)

### Issues

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

# README datalayer.easy.client

## Introduction

The sample app reads the values of the ctrlX Data Layer nodes of a ctrlX CORE.

## Prerequisite

A running ctrlX WORKS App Build Environment like described [here](https://boschrexroth.github.io/ctrlx-automation-sdk/setup_qemu_ctrlx_works.html)

## Function Description

The app demonstrates how values of the ctrlX Data Layer nodes can be read.

## Implementation Description

In main.cpp the ctrlX Data Layer system is started and a client connection is established.

Then an endless loop is running:

* Some ctrlX Data Layer values are read and their values are printed.
* After a sleep of the loop is repeated.

The loop ends and the main method exits when the ctrlX Data Layer client connection was broken.

With the help of the entry 'restart-condition: always' in snapcraft.yaml the snap is automatically restarted.

## Debug the Snap

1. Start your ctrlX CORE
2. Make sure you can connect from your Host PC
3. Fill in your connection settings
4. Run the script build-debug.sh
5. Press F5

Prerequisite to debug is the VS Code extension: C/C++.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap running on a ctrlX CORE

* Install the snap
* Open a SSH connection to your ctrlX CORE
* Check trace output: `sudo snap logs -f sdk-simple-client`

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

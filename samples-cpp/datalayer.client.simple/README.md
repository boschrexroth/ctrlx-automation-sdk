# README datalayer.client.simple

## Introduction

The sample app reads the values of some __ctrlX Data Layer__ nodes of a ctrlX CORE.

## Prerequisite

A running ctrlX WORKS App Build Environment like described [here](https://boschrexroth.github.io/ctrlx-automation-sdk/setup_qemu_ctrlx_works.html)

## Function Description

The app demonstrates how values of the __ctrlX Data Layer__ nodes can be read.

## Implementation Description

In main.cpp the __ctrlX Data Layer__ system is started and a client connection is established.

Then an endless loop is running:

* Some __ctrlX Data Layer__ values are read and their values are printed.
* After a sleep of the loop is repeated.

The loop ends and the main method exits when the __ctrlX Data Layer__ client connection was broken.

With the help of the entry 'restart-condition: always' in snapcraft.yaml the snap is automatically restarted.

## Debug and Run

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

Prerequisite to debug is the VS Code extension: C/C++.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap running on a ctrlX CORE

__Only if SSH access is available!__

* Install the snap
* Open a SSH connection to your ctrlX CORE
* Check trace output: `sudo snap logs -f sdk-cpp-client-simple`

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

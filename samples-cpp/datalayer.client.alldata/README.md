# README datalayer.client.alldata

## Introduction

This sample app performs __ctrlX Data Layer__ operations in cpp.

## Prerequisites

The sample requires that the app datalayer.provider.alldata is running in the SDK build environment or as snap on the crlX CORE.

## Function Description

The app demonstrates how values of existing nodes of the __ctrlX Data Layer__ can be accessed.

## Implementation Description

ctrlx_datalayer_helper.h Cpontains helper methods for creating ctrlX Datalayer Client and/or Provider instances.

datalayerclient.cpp und datalayerclient.h are defining a class which manages the __ctrlX Data Layer__ client access:

* Starting/stopping the __ctrlX Data Layer__ access
* Access of __ctrlX Data Layer__ nodes of different data type
* Callback methods for __ctrlX Data Layer__ access

In main.cpp an endless loop is running:

* An instance of the class DataLayerClient is created
* The instance is called to do __ctrlX Data Layer__ access
* The instance is deleted
* Sleep some time

## Debug and Run

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open a SSH connection to your ctrlX CORE
* Check trace output: `sudo snap logs -f sdk-cpp-client-alldata`

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

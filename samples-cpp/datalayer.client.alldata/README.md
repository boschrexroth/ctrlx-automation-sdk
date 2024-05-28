# README datalayer.client.alldata

## Introduction

This sample app performs ctrlX Data Layer operations in cpp.

## Prerequisites

The sample requires that the app datalayer.provider.alldata is running in the SDK build environment or as snap on the crlX CORE.

## Function Description

The app demonstrates how values of existing nodes of the ctrlX Data Layer can be accessed.

## Implementation Description

ctrlx_datalayer_helper.h Cpontains helper methods for creating ctrlX Datalayer Client and/or Provider instances.

datalayerclient.cpp und datalayerclient.h are defining a class which manages the ctrlX Data Layer client access:

* Starting/stopping the ctrlX Data Layer access
* Access of ctrlX Data Layer nodes of different data type
* Callback methods for ctrlX Data Layer access

In main.cpp an endless loop is running:

* An instance of the class DataLayerClient is created
* The instance is called to do ctrlX Data Layer access
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

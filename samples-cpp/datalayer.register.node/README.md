# README datalayer.register.node

## Introduction

The sample app __datalayer.register.node__ creates a new ctrlX Data Layer folder '__sdk-cpp-registernode__' with nodes of different datatypes.

## Functional Description

It demonstrates how you can create ctrlX Data Layer nodes with different data types.
These nodes can be accessed by a ctrlX Data Layer client app e.g. ctrlX-Node-RED.
The metadata of the nodes are provides by a metadata database.

## Implementation Description

All actions are implemented in main.cpp:

* Start ctrlX Data Layer system
* Connect as ctrlX Data Layer provider
* Register a flatbuffers schema
* Register metadata database
* Register several ctrlX Data Layer nodes
* Endless loop where the ctrlX Data Layer connection is observed
* Shutdown if the ctrlX Data Layer connection is broken


!!! important
    If the app is running as snap on a ctrlX CORE it will be restarted if it has exited on error.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open the web interface of your ctrlX CORE
* Check output in the web interface

![Output in diagnosis logbook](docs/images/datalayer.register.node/messages.in.logbook.png)
![ctrlX Data Layer tree](docs/images/datalayer.register.node/datalayer.register.node.png)

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

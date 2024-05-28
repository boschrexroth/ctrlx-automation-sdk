# Sample app sdk-cpp-provider-alldata

This cpp sample app is a ctrlX Data Layer Provider which can be used in two wayes:

1. As a sample how in cpp variables of different data types can be involved into ctrlX Data Layer Variants.
2. As a test ctrlX Data Layer Provider during development of a ctrlX Data Layer Client. These client apps can be written in one of the SDK supported languages or can be Node-RED or REST API applications.

## Introduction

Two ctrlX Data Layer branches are provided: `sdk/cpp/datalayer/provider/alldata/dynamic` and `sdk/cpp/datalayer/provider/alldata/static`.

The branches are providing sub nodes for most of the ctrlX Data Layer Variant types.

The static branch and its nodes cannot be changed by ctrlX Data Layer Client interactions. It can be used to read data and to test the min and max data values within the own application.

The dynamic branch however is changeable. Nodes can be written or deleted or created by clients. Furthermore new nodes or whole sub branches can be created.

## Implementation

### main.cpp

The main function establishes the ctrlX Data Layer connection. If the app is running within a snap on the ctrlX a IPC connection is used automatically. This connection doesn't need user credentials.

For a debugging session or in remote mode a tcp connection which need user credentials and an address is used. Therefore in the code user, password and address are predefined with default values.

### DataContainer Class

DataContainer is a simple data container class representing the data and error state of a node seen in the dynamic and/or static branch.

### ProviderNodeAllData Class

ProviderNodeAllData is a IProviderNode implementation and provides a ctrlX Data Layer sub branch. The sub branch can be static or dynamic.
Static means that the values of the nodes and the structure of he branch  cannot be changed by ctrlX Data Layer clients. In a dynamic branch everything can be changed.

All nodes of such a sub branch are 'virtual' because the node behind is always the same ProviderNodeAllData instance. Therefore it registers itself with a wildcard address (e.g. .../dynamic/\*\*) at the ctrlX Data Layer. The 'seen' nodes are DataContainer instances managed in a list and accessed by the on... callback functions of the ProviderNodeAllData class.

## Debug and Run within the App Build Environment

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open the web interface of your ctrlX CORE
* Check output in the web interface

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

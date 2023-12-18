# README datalayer.provider.simple

## Introduction

This sample app creates a new ctrlX Data Layer folder 'sdk/cpp/datalayer/provider/simple/' with nodes of different datatypes.

## Functional Description

It demonstrates how you can create ctrlX Data Layer nodes with different data types.
These nodes can be accessed by any ctrlX Data Layer client app.
The metadata of the nodes are provides by a metadata database.

## Implementation Description

All actions are implemented in main.cpp:

* Start ctrlX Data Layer system
* Connect as ctrlX Data Layer provider
* Register a flatbuffers schema
* Register metadata database
* Register several ctrlX Data Layer nodes: sdk/cpp/datalayer/provider/simple/
* Endless loop where the ctrlX Data Layer connection is observed
* Shutdown if the ctrlX Data Layer connection is broken


!!! important
    If the app is running as snap on a ctrlX CORE it will be restarted if it has exited on error.
	
___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

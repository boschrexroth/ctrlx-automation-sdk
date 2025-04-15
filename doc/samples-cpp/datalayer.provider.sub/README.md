# README datalayer.provider.sub

## Introduction

This sample provider app creates a new ctrlX Data Layer folder 'sdk/cpp/datalayer/provider/sub/' and handles all client subscriptions selfs.

## Functional Description

It demonstrates how you can create ctrlX Data Layer nodes and handle all client subscriptions selfs.
These nodes can be accessed by any ctrlX Data Layer client app.

## Implementation Description

All actions are implemented in src/provider/*.cpp:

* Start ctrlX Data Layer system
* Connect as ctrlX Data Layer provider
* Register several ctrlX Data Layer nodes: sdk/cpp/datalayer/provider/sub/
* Endless loop where the ctrlX Data Layer connection is observed
* Shutdown if the ctrlX Data Layer connection is broken

!!! important
    If the app is running as snap on a ctrlX CORE it will be restarted if it has exited on error.
___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

# README datalayer.provider.sub

## Introduction

This sample provider app creates a new __ctrlX Data Layer__ folder 'sdk/cpp/datalayer/provider/sub/' and handles all client subscriptions selfs.

## Functional Description

It demonstrates how you can create __ctrlX Data Layer__ nodes and handle all client subscriptions selfs.
These nodes can be accessed by any __ctrlX Data Layer__ client app.

## Implementation Description

All actions are implemented in src/provider/*.cpp:

* Start __ctrlX Data Layer__ system
* Connect as __ctrlX Data Layer__ provider
* Register several __ctrlX Data Layer__ nodes: sdk/cpp/datalayer/provider/sub/
* Endless loop where the __ctrlX Data Layer__ connection is observed
* Shutdown if the __ctrlX Data Layer__ connection is broken

!!! important
    If the app is running as snap on a ctrlX CORE it will be restarted if it has exited on error.
___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

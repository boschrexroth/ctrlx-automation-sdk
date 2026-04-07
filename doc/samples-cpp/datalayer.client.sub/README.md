# README datalayer.client.sub

This sample app demonstrates how __ctrlX Data Layer__ nodes are subscribed in single and multi mode.

## Functional Description

The sample app subscribes to two different __ctrlX Data Layer__ nodes:
    - framework/metrics/system/cpu-utilisation-percent
    - framework/metrics/system/memavailable-mb
If a value changes the app writes it to the terminal and also the size, type, address and timestamp.

## Implementation Description

ctrlx_datalayer_helper.h contains helper functions the create __ctrlX Data Layer__ provider and/or client instances.

In datalayerclientsub.cpp the class DataLayerClientSub is implemented. Here all activities for the __ctrlX Data Layer__ access are handled.

In main.cpp the main function first creates an instance of DataLayerClientSub and then runs an endless loop. If an error occurs the application is exited.

Running as a snap in the ctrlX CORE it will be restarted by snapd because of the `restart-condition: always` entry in snap/snapcraft.yaml.

## Debug and Run

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open a SSH connection to your ctrlX CORE
* Check trace output: `sudo snap logs -f sdk-cpp-sub`

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

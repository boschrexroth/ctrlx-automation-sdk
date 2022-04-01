# README datalayer.client.sub

This sample app demonstrates how Data Layer nodes are subscribed in single and multi mode.

## Functional Description

The sample app subscribes to two different Data Layer nodes:
    - framework/metrics/system/cpu-utilisation-percent
    - framework/metrics/system/memavailable-mb
If a value changes the app writes it to the terminal and also the size, type, address and timestamp. 

## Implementation Description

ctrlx_datalayer_helper.h contains helper functions the create ctrlX Data Layer provider and/or client instances.

In datalayerclientsub.cpp the class DataLayerClientSub is implemented. Here all activities for the Data Layer access are handled.

In main.cpp the main function first creates an instance of DataLayerClientSub and then runs an endless loop. If an error occurs the application is exited.

Running as a snap in the ctrlX CORE it will be restarted by snapd because of the `restart-condition: always` entry in snap/snapcraft.yaml.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open a SSH connection to your ctrlX CORE
* Check trace output: `sudo snap logs -f sdk-cpp-sub`

___

## License

MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

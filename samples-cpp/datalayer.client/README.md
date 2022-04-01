# README datalayer.client

## Introduction

The sample app __datalayer.client__ performs Data Layer operations in cpp. 

## Prerequisites

The sample requires that the app datalayer.provider.all-data is running in the SDK build environment or as snap on the crlX CORE. 

## Function Description

The app reads demonstrates how values of existing nodes of the ctrlX Data Layer can be accessed.

## Implementation Description 

ctrlx_datalayer_helper.h Cpontains helper methods for creating ctrlX Datalayer Client and/or Provider instances.

datalayerclient.cpp und datalayerclient.h are defining a class which manages the Data Layer client access:

* Starting/stopping the Data Layer access
* Access of Data Layer nodes of different data type
* Callback methods for Data Layer access


In main.cpp an endless loop is running:

* An instance of the class DataLayerClient is created
* The instance is called to do Data Layer access
* The instance is deleted
* Sleep some time

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open a SSH connection to your ctrlX CORE
* Check trace output: `sudo snap logs -f sdk-cpp-client`

___

## License

MIT License

Copyright (c) 2020-2022 Bosch Rexroth AG

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

# README Node.js datalayer.client.bulkread

This Node.js sample reads values in bulk mode from ctrlX Data Layer. 

## Introduction

The sample demonstrates how to bulk read values from ctrlX Data Layer and write out the values to console. 
The bulk read is performed every 10 seconds in an endless loop.

## Description

The sample __datalayer.client.bulkread__ demonstrates 

+ Create and start the 'DatalayerSystem' service
+ Create a client
+ Bulk Read values with method client.BulkRead(addresses) 
+ Bulk Read methods are called in an endless loop every 10s.

## Prerequisites

Please read the common Node.js docs [here](./../README.md), first.

## Debug and Run
1. Run 'npm install' in the terminal.  
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder in QEMU VM__.
4. Open the directory __datalayer.client.bulkread__.
5. Build and install snap as described [here](./../README.md).
6. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__ 
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.sdk-node-client-bulkread.app.service__
   + Now you should the see the app output diagnostics. 
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:
    ```bash
    sudo snap logs sdk-node-client-bulkread.app -f | more
    ```
## Console Output
```bash
connection string: ipc://
address: framework/metrics/system/cpu-utilisation-percent, value: 27.6, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
address: framework/metrics/system/memavailable-mb, value: 401.9296875, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
address: framework/metrics/system/membuffers-mb, value: 12.60546875, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
address: framework/metrics/system/memcache-mb, value: 400.5859375, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
address: framework/metrics/system/memfree-mb, value: 25.640625, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
address: framework/metrics/system/memtotal-mb, value: 975.8125, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
address: framework/metrics/system/memused-percent, value: 56.3, timestamp: 2022-10-18T14:40:15.440Z, result: DL_OK
```

## Install the App

Login into ctrlX and install the App (Apps).

## Gratulations - We're finished - Let's start coding!


## Support
### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/) 

### SDK Forum

Please visit the [SDK Forum](https://developer.community.boschrexroth.com/t5/ctrlX-AUTOMATION/ct-p/dcdev_community-bunit-dcae/) 

### Issues

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

# README Node.js datalayer.client

This Node.js sample reads values from ctrlX Data Layer.

## Introduction

The sample demonstrates how to read values from ctrlX Data Layer tree and write out the values to console.

## Description

The sample __datalayer.client__ demonstrates

+ How easy to create a client for ctrlX Data Layer

+ How to read values from ctrlX Data Layer nodes

+ How to create a subscription to a ctrlX Data Layer node

## Prerequisites

Please read the common Node.js docs [here](./../README.md), first.

## Debug and Run

1. Run 'npm install' in the terminal.  
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder in QEMU VM__.
4. Open the directory __datalayer.client__.
5. Build and install snap as described [here](./../README.md).
6. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.sdk-node-client-simple.app.service__
   + Now you should the see the app output diagnostics.
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:

    ```bash
    sudo snap logs sdk-node-client-simple.app -f | more
    ```

## Console Output

```bash
running inside snap: false
client remote address: tcp://boschrexroth:boschrexroth@10.0.2.2
framework/metrics/system/cpu-utilisation-percent:{"value":1.9,"type":"double","timestamp":"2021-10-20T13:40:45.892Z"}
framework/metrics/system/memavailable-mb:{"value":680.578125,"type":"double","timestamp":"2021-10-20T13:40:45.896Z"}
framework/metrics/system/membuffers-mb:{"value":119.00390625,"type":"double","timestamp":"2021-10-20T13:40:45.902Z"}
framework/metrics/system/memcache-mb:{"value":514.56640625,"type":"double","timestamp":"2021-10-20T13:40:45.905Z"}
framework/metrics/system/memfree-mb:{"value":164.43359375,"type":"double","timestamp":"2021-10-20T13:40:45.909Z"}
framework/metrics/system/memtotal-mb:{"value":985.0078125,"type":"double","timestamp":"2021-10-20T13:40:45.913Z"}
framework/metrics/system/memused-mb:{"value":187.00390625,"type":"double","timestamp":"2021-10-20T13:40:45.918Z"}
framework/metrics/system/memused-percent:{"value":19.4,"type":"double","timestamp":"2021-10-20T13:40:45.923Z"}
```

## Install the App

Login into ctrlX and install the App (Apps).

## Gratulations - We're finished - Let's start coding

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

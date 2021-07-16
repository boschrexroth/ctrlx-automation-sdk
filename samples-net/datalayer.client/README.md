# README .NET datalayer.client

This .NET sample reads values from ctrlX Data Layer. 

## Introduction

The sample demonstrates how to read values from ctrlX Data Layer tree and write out the values to console. A subscription is used to get values by data change event in a deterministic publish interval. The single read is performed every 10 seconds in an endless loop.

## Description

The following code shows the main method with inline documentation:

[Program.cs](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/develop/samples-net/datalayer.client/Program.cs)

## Prerequisites

Please read the common .NET docs [here](./../README.md), first.

## Getting Started

1. Launch VSCode
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder in WSL__.
4. Open the directory __datalayer.client__.
5. Build and install snap as described [here](./../README.md).
6. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__ 
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.ctrlx-dotnet-datalayer-client.app.service__
   + Now you should the see the app output diagnostics. 
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:
    ```bash
    sudo snap logs ctrlx-dotnet-datalayer-client.app -f | more
    ```
## Console Output
```bash
Running inside snap: True
ctrlX Data Layer system started.
ctrlX Data Layer client created.
Client connected: True
10.03.2021 09:15:10, framework/metrics/system/cpu-utilisation-percent: 6 (subscription)
10.03.2021 10:15:12, framework/metrics/system/memused-percent: 14 (single read)
10.03.2021 09:15:11, framework/metrics/system/cpu-utilisation-percent: 13,6 (subscription)
10.03.2021 09:15:12, framework/metrics/system/cpu-utilisation-percent: 2,5 (subscription)
10.03.2021 09:15:13, framework/metrics/system/cpu-utilisation-percent: 7,4 (subscription)
10.03.2021 09:15:14, framework/metrics/system/cpu-utilisation-percent: 6 (subscription)
10.03.2021 09:15:15, framework/metrics/system/cpu-utilisation-percent: 3,7 (subscription)
10.03.2021 09:15:16, framework/metrics/system/cpu-utilisation-percent: 4,9 (subscription)
10.03.2021 09:15:18, framework/metrics/system/cpu-utilisation-percent: 6 (subscription)
10.03.2021 09:15:19, framework/metrics/system/cpu-utilisation-percent: 7,2 (subscription)
10.03.2021 09:15:20, framework/metrics/system/cpu-utilisation-percent: 6 (subscription)
10.03.2021 10:15:22, framework/metrics/system/memused-percent: 14 (single read)
10.03.2021 09:15:21, framework/metrics/system/cpu-utilisation-percent: 5 (subscription)
10.03.2021 09:15:22, framework/metrics/system/cpu-utilisation-percent: 7,3 (subscription)
10.03.2021 09:15:23, framework/metrics/system/cpu-utilisation-percent: 4,9 (subscription)
10.03.2021 09:15:24, framework/metrics/system/cpu-utilisation-percent: 2,5 (subscription)
10.03.2021 09:15:25, framework/metrics/system/cpu-utilisation-percent: 6,1 (subscription)
10.03.2021 09:15:26, framework/metrics/system/cpu-utilisation-percent: 6 (subscription)
10.03.2021 09:15:27, framework/metrics/system/cpu-utilisation-percent: 8,8 (subscription)
10.03.2021 09:15:28, framework/metrics/system/cpu-utilisation-percent: 10,3 (subscription)
10.03.2021 09:15:29, framework/metrics/system/cpu-utilisation-percent: 4,9 (subscription)
10.03.2021 09:15:30, framework/metrics/system/cpu-utilisation-percent: 7,2 (subscription)
10.03.2021 10:15:32, framework/metrics/system/memused-percent: 14 (single read)
10.03.2021 09:15:31, framework/metrics/system/cpu-utilisation-percent: 6,1 (subscription)
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

MIT License

Copyright (c) 2021 Bosch Rexroth AG

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

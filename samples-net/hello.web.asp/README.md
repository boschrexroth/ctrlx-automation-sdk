# README .NET hello asp.net

This .NET sample is a simple ASP.NET application.

## Introduction

Programm shows how to write a ASP.NET application running on ctrlX CORE.

## Description

The following code shows the main method with inline documentation:

[Program.cs](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/samples-net/hello.web.asp/Program.cs)

## Prerequisites

Please read the common .NET docs [here](./../../doc/dotnet.md), first.

## Debug and Run
Launch the app with the debugger attached by pressing __F5__.

## Build and Install

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder__.
4. Open the directory __hello.web.asp__.
5. Build and install snap as described [here](./../README.md).
6. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__ 
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.sdk-net-hello-web-asp.app.service__
   + Now you should the see the app output diagnostics. 
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:
   
    ```bash
    sudo snap logs sdk-net-hello-web-asp.app -f | more
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

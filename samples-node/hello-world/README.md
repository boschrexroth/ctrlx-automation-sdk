# README node hello.world

This node sample prints out "Hello World from Node.js!" to the console.

## Introduction

Programm is prompting "Hello World from Node.js!" to standard output.

## Description

The samples demonstrates how to snap a simple Node.js script with the snapcraft [npm plugin](https://snapcraft.io/docs/npm-plugin).

## Prerequisites

Please read the common node docs [here](./../README.md), first.

## Debug and Run
1. Run 'npm install' in the terminal.  
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install

1. Launch Visual Studio Code
2. Click on the __Remote Explorer__ from the menu.
3. Choose __Open Folder__.
4. Open the directory __datalayer.browse__.
5. Build and install snap as described [here](./../README.md).
6. Check the output using __Diagnostics__:

   + Login into your ctrlX
   + Navigate to __Diagnostics -> Logbook__ 
   + Click __Settings__ on the right top corner
   + Enable __Show system messages__
   + Navigate to __Filter -> Units__ and check your __snap.sdk-node-hello-world.app.service__
   + Now you should the see the app output diagnostics. 
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:
    ```bash
    sudo snap logs sdk-node-hello-world.app -f | more
    ```
## Console Output
```bash
Hello World from Node.js! Thu, 07 Oct 2021 11:35:08 GMT
Hello World from Node.js! Thu, 07 Oct 2021 11:35:09 GMT
Hello World from Node.js! Thu, 07 Oct 2021 11:35:10 GMT
Hello World from Node.js! Thu, 07 Oct 2021 11:35:11 GMT
Hello World from Node.js! Thu, 07 Oct 2021 11:35:12 GMT
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

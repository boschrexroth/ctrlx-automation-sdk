# README .NET hello.world

This .NET sample prints out "Hello World" to the console.

## Introduction

The app is prompting "Hello World" to standard output.

## Description

The implementation include the following steps:

+ Write 'Hello World' to console in an endless loop every second.

## Prerequisites

Please read the common .NET docs [here](./../../doc/dotnet.md), first.

## Getting Started

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
   + Navigate to __Filter -> Units__ and check your __snap.ctrlx-dotnet-hello-world.app.service__
   + Now you should the see the app output diagnostics. 
   + Press __Refresh__ to update.

7. If you have root permissions, you can also watch the output on any ssh console with the command:
    ```bash
    sudo snap logs ctrlx-dotnet-hello-world.app -f | more
    ```
## Console Output
```bash
Hello World! 11.03.2021 09:45:05
Hello World! 11.03.2021 09:45:06
Hello World! 11.03.2021 09:45:07
Hello World! 11.03.2021 09:45:08
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

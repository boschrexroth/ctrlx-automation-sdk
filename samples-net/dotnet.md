# README .NET 5.0 Apps @ ctrlX

![.NET5](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a3/.NET_Logo.svg/120px-.NET_Logo.svg.png)

## Introduction

This document describes how to setup up your development environment on Windows for building __self-contained ctrlX Apps based on Microsoft .NET 5.0 (dotnet5) runtime__.

__Self-contained__ means, your App contains an already matching and optimized .NET 5.0 runtime for your target architecture, so there's no need to install any additional .NET 5.0 runtime.

All samples generate Apps (snaps) for the targets:

+ ctrlXVirtual (amd64)
+ ctrlX (arm64)

## Pre-requisites

### Install/Update Microsoft Visual C++ Redistributable for Visual Studio
If you have latest Microsoft Visual Studio including C++ components installed, you can skip this step.

Please install the latest __Microsoft Visual C++ Redistributable for Visual Studio__ from [here](
https://support.microsoft.com/en-us/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0).

### Install/Update Visual Studio Code (VS Code)

The ctrlX App IDE is __Microsoft Visual Studio Code__. Please ensure having latest version installed on your system from [here](https://code.visualstudio.com/).

### Install and Update Windows Subsystem for Linux (WSL) and Ubuntu 18.04

The samples make use of __Windows Subsystem for Linux (WSL) Version 18.04__. Please read common SDK help to setup. Also ensure your __proxy settings__ if you're behind a corporate proxy (Settings -> Proxy).

After done, please ensure you have the latest Linux environment, by updating your WSL:

1. Open Visual Studio Code
2. Open the Remote Explorer from the menu
3. Connect to your WSL Target __Ubuntu-18.04__.
4. Select __Open Folder in WSL__ and navigate to any .NET sample e.g. _hello.world_
5. Open a new __Terminal__ (Terminal -> New Terminal)
6. Run the following commands in your WSL console

            sudo apt update
            sudo apt upgrade

### Prepare WSL

First, we have to prepare WSL to fetch packages of all supported platform architectures.
Please search for __Dependencies for Crossbuild (Multiarch)__ [here](./setup_windows_wsl_ubuntu.md) or use the template file provided (please rename your existing to be restored on demand):

1. Open WSL by Windows -> Start

            wsl

2. Ensure the following content at the end of file _/etc/apt/sources.list_

            deb [arch=i386,amd64] http://security.ubuntu.com/ubuntu/ bionic-security main restricted
            # deb-src http://security.ubuntu.com/ubuntu/ bionic-security main restricted
            deb [arch=i386,amd64] http://security.ubuntu.com/ubuntu/ bionic-security universe
            # deb-src http://security.ubuntu.com/ubuntu/ bionic-security universe
            deb [arch=i386,amd64] http://security.ubuntu.com/ubuntu/ bionic-security multiverse
            # deb-src http://security.ubuntu.com/ubuntu/ bionic-security multiverse


Alternatively you can use the provided  template  _sources.list_ file and copy it from

            ./samples.net/dotnet5/docs/files/sources.list

to any directory e.g. _c:/temp/sources.list_

After that copy the file from there into WSL

            sudo cp /mnt/c/temp/sources.list  /etc/apt

Update the VS Code Server on WSL:

      Run the following commands in your WSL console

            code

## Installation of .NET 5 into WSL
The following steps are based on [this](https://docs.microsoft.com/de-de/dotnet/core/install/linux-ubuntu#1804-) description

1. Open WSL by Windows -> Start

            wsl

2. Add the Microsoft package signing key to your list of trusted keys

      Run the following commands in your WSL console

            wget https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb

            sudo dpkg -i packages-microsoft-prod.deb

3. Install the .NET SDK

      Run the following commands in your WSL console

            sudo apt-get update; \
                  sudo apt-get install -y apt-transport-https && \
                  sudo apt-get update && \
                  sudo apt-get install -y dotnet-sdk-5.0

If you're using __ASP.NET__, install the ASP.NET Runtime  

      Run the following commands in your WSL console

          sudo apt-get install -y aspnetcore-runtime-5.0

1. Check your installed SDK's

      Run the following command in your WSL console

            dotnet --list-sdks

      The output should be something like

            5.0.102 [/usr/share/dotnet/sdk]

2. Check your Runtime
      Run the following command in your WSL console

            dotnet --list-runtimes

      The output should be something like

            Microsoft.NETCore.App 5.0.2 [/usr/share/dotnet/shared/Microsoft.NETCore.App]

### Prepare NuGet

If you're behing a corporate proxy, you have to configure the __NuGet Proxy__ in configuration file:

      /home/{USERNAME}/.nuget/NuGet/NuGet.Config

Edit the NuGet configuration file with your preferred editor or with VS Code and add a _config_ section containing the __http_proxy__ and __https_proxy__ inside the _configuration_ section like shown below.
Alternatively you can copy the provided file

1. Open WSL by Windows -> Start

            wsl

2. Copy the provided file from

            ./samples.net/dotnet5/docs/files/NuGet.Config

      to folder 
    
            /home/{USERNAME}/.nuget/NuGet

You can copy it to a temp directory e.g. _c:/temp/NuGet.Config_

After that copy the file from there into WSL

            sudo cp /mnt/c/temp/NuGet.Config  /home/{USERNAME}/.nuget/NuGet

__Example:__

The following sample uses the [Px Proxy](https://pypi.org/project/px-proxy) at address http://127.0.0.1:3128.
for example:

      <configuration>
            <config>
                  <add key="http_proxy" value="http://127.0.0.1:3128" />
                  <add key="https_proxy" value="http://127.0.0.1:3128" />
            </config>
      ...
      </configuration>

## Install the VS Code extensions from Marketplace
Finally we have to install some extensions for .NET to be ready for coding.

![C# extension](./images/csharpextension.png)

+ Click on the __Remote Explorer__ from the menu.
+ Choose __Open Folder in WSL__.
+ Open any __.NET sample directory (e.g. hello.world).__

![.NET Sample](./images/wsl-helloworld.png)

+ Open __Extensions__ from the menu.
+ Install the C# Extension and press __Install on Windows Subsystem for Linux__.

## Building
Each sample provides a rich set of tasks for your convenience.

![Tasks](./images/tasks.png)

+ Click on Terminal -> __Run Task__.
+ Choose your preferred task to run.

### Build (compile) your code

+ Click on Terminal -> __Run Task__.
+ Choose Task __build__.

### Clean your code

+ Click on Terminal -> __Run Task__.
+ Choose Task __clean__.
  
### Publish your (self-contained) application 

+ Click on Terminal -> __Run Task__.
+ Choose any Task __publish *__ matching your architecture and mode.

### make snap

+ Click on Terminal -> __Run Task__.
+ Choose any Task __make snap *__ matching your architecture and mode.

## Debugging

### Debug on WSL

Just press __F5__ or

+ Click on the __Run__ from the menu.
+ Choose Run -> __.NET Core Launch (console)__.
+ Press the green  __Play__ button.

### Debug snap on ctrlXVirtual

+ Click on Terminal -> __Run Task__.
+ Choose any Task __make snap amd64__.
+ Install snap on ctrlXVirtual via ctrlX web interface
+ For debugging snaps see [here](https://snapcraft.io/docs/debug-snaps)

### Debug snap on ctrlX

+ Click on Terminal -> __Run Task__.
+ Choose any Task __make snap arm64__.
+ Install snap on ctrlX via ctrlX web interface
+ For debugging snaps see [here](https://snapcraft.io/docs/debug-snaps)

Now you're ready to code .NET on ctrlX! Please open any sample directory in WSL using the Remote Explorer and see documentations how to 

## Best Practise

### Provider Concepts

There different concepts to write a Data Layer provider:

#### Create and register nodes with individual node handlers

Create a provider, which creates and registers _all_ nodes with an _individual_ node handler 

            n x CreateNode
            n x Register
            Node : Handler = 1:1

__Recommendation:__

+ Use for a _small/medium_ of nodes with _individual_ node handling.
+ Use for a logical grouping and managing of nodes.

__Pros:__

+ The Data Layer is managaging your node hierarchy: No implementation of _OnBrowse()_ required (return DL_UNSUPPORTED or DL_OK with an empty list).

__Contras:__

+ Not efficent for a large/very large set of nodes to handle.

__Samples:__

+ datalayer.provider

#### Create and register nodes with same node handler

Create a provider, which creates _individual_ nodes and registers all nodes individually with _same_ node handler.

            n x CreateNode
            n x Register
            Node : Handler = n:1

__Recommendation:__

+ Use for a _small/medium_ of nodes with _common_ node handling
+ Use for a logical grouping and managing of nodes.

__Pros:__

+ The Data Layer is managaging your node hierarchy: No Implementation of _OnBrowse()_ required (return DL_UNSUPPORTED or DL_OK with an empty list).

__Contras:__

+ Not efficent for a large/very large set of nodes to handle.

__Samples:__

+ datalayer.mqtt
+ datalayer.provider.alldata

#### Virtual provider (lightweight): Register on wildcard and manage virtual nodes

Create just _one_ node with a single node handler, registered to a _wildcard_ address  (e.g. myroot/**) and managing the nodes for your own. This _lightweight_ handler returns _virtual_ nodes in _OnBrowse()_ method. 
See Data Layer documentation for details.

            1 x CreateNode
            1 x Register
            Node : Handler = n:1

__Recommendation:__

+ Use for a _large/very large_ or _dynamic_ set of nodes (big data provider).
+ Use if you want to manage the nodes in your application code.

__Pros:__

+ Very efficient.

__Contras:__

+ The Data Layer is _not_ managaging your node hierarchy: You have to return the valid data in _OnBrowse()_.

__Samples:__

+ datalayer.provider.virtual

## Pitfalls

### Arithmetic operators of different integral or foating point types

The following operators perform arithmetic operations with operands of numeric types:

+ __Unary__: ++ (increment), -- (decrement), + (plus), and - (minus) operators
+ __Binary__: * (multiplication), / (division), % (remainder), + (addition), and - (subtraction) operators

Those operators are supported by all integral and floating-point numeric types.

In the case of integral types, those operators (except the ++ and -- operators) are defined for the __int__, __uint__, __long__, and __ulong__ types. 

When operands are of other integral types (__sbyte__, __byte__, __short__, __ushort__, or __char__), their values are __converted to the int type__, which is also the __result type__ of an operation. 

When operands are of __different integral or floating-point__ types, their values are __converted to the closest containing type__, if such a type exists. For more information, see the Numeric promotions section of the C# language specification. 

The ++ and -- operators are defined for all integral and floating-point numeric types and the char type.

__Example__:

Let' say we just wan't to increment a __sbyte__ Variant value by keeping the data type, we have cast the result after performing the arithmetic operation:

      sbyte value = 42;
      var oldVariant = new Variant(value)
      var newVariant = new Variant((sbyte)(oldVariant.ToSByte() + 1));

+ Please see [here](https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators) for more informations.

 
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




# README datalayer.client.sub

This sample app demonstrates how Data Layer nodes are subscribed in single and multi mode.

## Introduction

The sample app subscribes to two different Data Layer nodes:
    - framework/metrics/system/cpu-utilisation-percent
    - framework/metrics/system/memavailable-mb
If a value changes the app writes it to the terminal and also the size, type, address and timestamp. 

## Prerequisites

* Build environment - we recommend a QEMU virtual machine
* ctrlX AUTOMATION SDK archive
* ctrlX CORE<sup>virtual</sup> or ctrlX CORE

### Prerequisites for Building the cpp Samples on the Build Environment

* Start your build environment
* Copy the ctrlX AUTOMATION SDK archive from your host computer to your build environment:

    scp -P 10022 com.boschrexroth.sdk.control-*.zip boschrexroth@localhost:~

* Open a SSH shell on your build environment:

    ssh -p 10022 boschrexroth@localhost

* Unzip the archive

    unzip -XK com.boschrexroth.sdk.control-*.zip

* Change to SDK directory public
* Set x flag: 

    chmod +x bin\oss.flatbuffers\u*\release\flatc

* If installed remove older ctrlx-datalayer package

    sudo dpkg -r ctrlx-datalayer 

* Install newest ctrlx-datalayer package

    sudo dpkg -i deb/ctrlx-datalayer*.deb

## Getting Started

- Launch VisualStudio Code (VS Code) as your IDE
- If VS Code is started remote connect via Remote Explorer extension to your builder environment (SSH target)
- Open SDK folder `public/samples-cpp/datalayer.client.sub`

### Start VS Code Debug Session

Select the CMake symbol in the sidebar, click '...' at the top of the explorer window and select 'Clean Reconfigure All Projects'.

Then make these settings in the status bar (blue area) :
- CMake (Current build variant): Select 'Debug'
- Active kit: Select 'GCC ... x86...'
- Select 'datalayerClientSub' as build target

Click Build and observe the progress in the output window - the build process should finish without errors (exit code 0).

Now the executable for debugging is available: generated/ubuntu20-gcc-x64/Debug/__datalayerClientSub__

To start debugging select 'Run and Debug' in the side bar, select 'x64...' as launch configuration an press F5 (or green start button).

The executable will be started, the debugger set automatically a breakpoint at the beginning of the main() function.

### Build a Snap

- Select the CMake symbol in the sidebar, click '...' at the top of the explorer window and select 'Clean All Projects'.
- Select main menu item Terminal --> Run Build Task --> Select the CMake Build Kit 'GCC x86...' for building a snap for a ctrlX CORE virtual or 'GCC aarch64...' for a ctrlX CORE.

At the end of the build progress the snap file should be avaiable in project folder.

Right click the file and select 'Download' to install it on a ctrlX CORE.

## Troubleshooting

All automatically created files are located in folders `build` and `generated`.  
If there are unclear messages during the build process, it might help to delete the folders `build` and `generated` and restart the build process.

## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

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

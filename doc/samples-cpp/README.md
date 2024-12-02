# Samples C/C++

![C++](https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/ISO_C%2B%2B_Logo.svg/120px-ISO_C%2B%2B_Logo.svg.png)

## Prerequisites

* ctrlX WORKS
* ctrlX AUTOMATION App Build Environment
* ctrlX AUTOMATION SDK installed in the App Build Environment
* ctrlX CORE^virtual^ or ctrlX CORE

* Visual Studio Code installed on your host computer

Visual Studio Code extensions (Install in SSH):

* C/C++ for Visual Studio Code (Microsoft)
* C/C++ Extension Pack (Microsoft)
* CMake Tools (Microsoft)

## Find out the supported samples

### Basic Samples

* [Hello World](./hello.world/README.md) recommendation -> start with this example
* [Hello PLC](./hello.plc/README.md)
* [ctrlX Data Layer Provider Simple](./datalayer.provider.simple/README.md)
* [ctrlX Data Layer Client Simple](./datalayer.client.simple/README.md)
* [ctrlX Data Layer Client Subscription](./datalayer.client.sub/README.md)
* [Diagnostics Logbook](./diagnostics.logbook/README.md)
* [ctrlX Data Layer Diagnosis](./datalayer.diagnosis/README.md)

### Advanced Samples

* [ctrlX Data Layer EtherCAT I/O's](./datalayer.ecat.io/README.md)
* [ctrlX Data Layer Shared Memory](./datalayer.sharedmemory/README.md)
* [ctrlX Data Layer Provider All Data Types](./datalayer.provider.alldata/README.md)
* [ctrlX Data Layer Client All Data Types](./datalayer.client.alldata/README.md)

## Debug and Run

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install

* Launch Visual Studio Code as your IDE
* If the IDE is started remote connect via Remote Explorer extension to your builder environment (SSH target)
* Ensure that Visual Studio Code extensions "C/C++ Extension Pack" and "CMake Tools" are installed in your SSH target.
* Open the folder of the desired cpp sample under  `samples-cpp/`

### Build a Snap

See [Samples Overview](../samples.md)

__Important:__ Before building an arm64 snap run script `public/scripts/environment/scripts/install-cpp-aarch64-libs.sh` once!

### Create an Executable for Debugging

Select the CMake symbol in the sidebar, click '...' at the top of the explorer window and select 'Clean Reconfigure All Projects'.

Then make these settings in the status bar (blue area) :

* CMake (Current build variant): Select 'Debug'
* Active kit: Select 'GCC ... x86...'
* Select 'x64...' as build target

Click Build and observe the progress in the output window - the build process should finish without errors (exit code 0).

Now the executable for debugging is available in the subdirectory: generated/__ubuntu22-gcc-x64/Debug/__

To start debugging select 'Run and Debug' in the side bar, select 'x64...' as launch configuration an press F5 (or green start button).

The executable will be started and the debugger will set automatically a breakpoint at the beginning of the main() function.

## Troubleshooting

All automatically created files are located in subfolders `build` and `generated`.  

If there are unclear messages during the build process, it might help to delete the folders `build` and `generated` and restart the build process.

## Support

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

## Official Documentation

<https://docs.automation.boschrexroth.com/>

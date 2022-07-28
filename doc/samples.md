
## Sample Projects

Applications (snaps) for ctrlX CORE targets can be written in several programming language or even as shell scripts.

For creating your own applications, the ctrlX AUTOMATION SDK offers sample projects that can be used directly or as templates. The sample projects are saved in sub folders according to their programming language or their type. 


Here an overview of the sample project types and their subfolders.


| Sample Project Type | Description                | Link to Projects                                       |
| :------------------ | :------------------------- | :----------------------------------------------------- |
| __C/C++__           | Samples written in C/C++   | [samples-cpp/README.md](./samples-cpp/README.md)       |
| __Go__              | Samples written in Go      | [samples-go/README.md](./samples-go/README.md)         |
| __Python__          | Samples written in Python  | [samples-python/README.md](./samples-python/README.md) |
| __.NET__            | Samples written in .NET    | [samples-net/README.md](./samples-net/README.md)       |
| __Node.js__         | Samples written in Node.js | [samples-node/README.md](./samples-node/README.md)     |
| __Shell__           | Sample shell script        | [samples-sh/README.md](./samples-sh/README.md)         |
| __Snap__            | Sample package manifest    | [samples-snap/README.md](./samples-snap/README.md)     |
| __IEC 61131-3__     | Samples in IEC 61131-3 ST  | [samples-iec61131/README.md](./samples-iec61131/README.md)|


## Building Snaps

!!! important
    All project folders are containing these scripts to build snaps:

* __build-snap-amd64.sh__: Build an amd64 snap for a ctrlX CORE <sup>virtual</sup>.
* __build-snap-arm64.sh__: Build an arm64 (aarch64) snap for a ctrlX CORE.

You can call these scripts from the command line:

    ./build-snap-amd64.sh
    ./build-snap-arm64.sh

 or from Visual Studio Code:

 * Select main menu item Terminal --> Run Build Task
 * Select `build snap amd64` for building a snap for a ctrlX CORE virtual
 * Select `build snap arm64` for building a snap for a ctrlX CORE.

At the end of the build process the snap file should be available in the root folder of your project.

## Installing a Snap

Right click the snap file in the Visual Studio Code EXPLORER and select 'Download'. Visual Studio Code stores it in your home directory on your host computer. For Windows 10 this is %USERPROFILE%, in Linux this is: ~/

Start a Web browser, login into your ctrlX CORE:

* Select Settings - Apps
* Switch to Service Mode
* Click Install from file
* Select the downloaded file 
* Switch to Operation Mode
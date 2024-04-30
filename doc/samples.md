
## Sample Projects

Applications (snaps) for ctrlX CORE targets can be written in several programming language or even as shell scripts.

For creating your own applications, the ctrlX AUTOMATION SDK offers sample projects that can be used as source of code snippets as templates. The sample projects are saved in sub folders according to their programming language or their type.

Here an overview of the sample project types and their subfolders.

!!! important
    Run the install script listet in column three once before you are working with one or more sample projects.
    These scripts are located in __/home/boschrexroth/scripts__. See also [install-scripts](install-scripts.md).

| Sample Project Type | Link to Projects                                        | Install Script
| :------------------ | :------------------------------------------------------ | :------------------------ |
| __C/C++__           | [samples-cpp/README.md](samples-cpp/README.md)          | ~/scripts/install-cpp-aarch64-libs.sh
| __Go__              | [samples-go/README.md](samples-go/README.md)            | ~/scripts/install-go.sh
| __Python__          | [samples-python/README.md](samples-python/README.md)    |
| __.NET__            | [samples-net/README.md](samples-net/README.md)          | ~/scripts/install-dotnet-sdk.sh
| __Node.js__         | [samples-node/README.md](samples-node/README.md)        | ~/scripts/install-nodejs-npm.sh         |
| __Snap__            | [samples-snap/README.md](samples-snap/README.md)        |
| __IEC 61131-3__     | [samples-iec61131/README.md](samples-iec61131/README.md)|


## Building Snaps

!!! important
    All project folders are containing these scripts to build snaps:

* __build-snap-amd64.sh__: Build an amd64 snap for a ctrlX CORE<sup>virtual</sup>.
* __build-snap-arm64.sh__: Build an arm64 (aarch64) snap for a ctrlX CORE.

You can call these scripts from the command line:

    ./build-snap-amd64.sh
    ./build-snap-arm64.sh

 or from Visual Studio Code:

 * Select main menu item Terminal --> Run Build Task
 * Select `build snap amd64` for building a snap for a ctrlX COREvirtual
 * Select `build snap arm64` for building a snap for a ctrlX CORE.

At the end of the build process the snap file should be available in the root folder of your project.

See below for further Run Build Task items.

## Installing a Snap Manually

Right click the snap file in the Visual Studio Code EXPLORER and select 'Download'. Visual Studio Code stores it in your home directory on your host computer. For Windows 10 this is %USERPROFILE%, in Linux this is: ~/

Start a Web browser, login into your ctrlX CORE:

* Select Settings - Apps
* Switch to Service Mode
* Click Install from file
* Select the downloaded file
* Switch to Operation Mode

## Building and Installing a Snap Automated by Script

Using the bash script __build-upload-log-snap.sh__ in the ctrlX AUTOMATION SDK folder scripts/
You can create a snap file, upload it, install it on a ctrlX CORE and view the log output.

The script can be started either from the command line or with the additional Run Build Task items of the Visual Studio Code IDE.

### Starting Script from the Command Line

Change into a sample project folder (e.g. samples-cpp/datalayer.client) and enter

     ../../scripts/build-upload-log-snap.sh -help

All parameters and their default values are listed.

E.g. to build and install a snap for a ctrlX COREvirtual with Network Adapter enter

    ../../scripts/build-upload-log-snap.sh -NA

But we recommend calling this script from Visual Studio Code. See next chapter.

### Starting Script as Visual Studio Build Task

Additional to the two Build Tasks mentioned above there are further tasks:

* __Build upload snap - ctrlX COREvirtual Network Adapter__
* __Build upload snap - ctrlX COREvirtual Port Forwarding__
* __Build upload log snap - ctrlX CORE 192.168.1.1__
* __Build upload snap__

The first three tasks are calling the script with the parameter set needed for the according destination. There is sno need to select further parameters.

If you are choosing the last item each parameter is prompted.

Feel free to add more tasks or adapt the existing ones.

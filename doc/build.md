# Build Projects and Install Snaps

## Prerequisites

### Data Layer and libzmq

Change to directory deb/ and install the Data Layer package:

    sudo dpkg -i *.deb

If you are using a SDK QEMU VM the package libzmq is installed yet, otherwise run

    sudo apt-get -y install libzmq3-dev:amd64
    sudo apt-get -y install libzmq3-dev:arm64

### Snapcraft

Snapcraft is the software tool to build snaps. Ensure that the newest version of snapcraft is installed:

    sudo apt remove snapcraft
    sudo snap install snapcraft --classic

### Visual Studio Code

We recommend to use Visual Studio Code to work with the sample projects and of course with your own projects.

* For editing and compiling your projects you have to install programing language specific extensions in your SSH build environment -see [Setup Visual Studio Code to work with the ctrlX AUTOMATION SDK](.(vscode.md))

* To work with a project select menu item File - Open Folder and select your project folder.

## C++ Project

Building of a debug executable and a snap from a C/C++ project is described in details here ["C/C++ Samples"](./samples-cpp/README.md)

## Go Project

### Create a Snap

Start task `Snapcraft` by selecting the following command of the menu bar in Visual Studio Code Editor:  
__Terminal > Run Build Task__  or press __Ctrl-P__ and enter __task CMake Build Snap__. Select the target for which you want to create the snap.
Use `amd64` for __ctrlX CORE<sup>virtual</sup>__ and `arm64` for __ctrlX CORE__

Now you should have one snap file generated:  
e.g `./hello-webserver_1.0.0_amd64.snap`

## Python Project


# Install a Snap

* Open the web interface of your control. 
* Select "Install an app" from the Home screen.
* Click the "Service mode" field to switch to __Service Mode__
* Click the "Settings" icon in the gray header field, activated "Alow installation from unknown source" and click Save
* Click "Install from file" icon in the gray header field, in the upcomming dialog select your downloaded snap file 
* When the upload and installation process is finished click "Operation mode"

For information about snap debugging see [Debugging](./debug.md).
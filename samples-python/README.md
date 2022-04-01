# Python Samples

![Python](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c3/Python-logo-notext.svg/120px-Python-logo-notext.svg.png)

## Find out the supported samples

* [AppData File Storage](./appdata/README.md)
* [ctrlX Data Layer Calculation operations](./datalayer.calc/README.md)
* [ctrlX Data Layer Client](./datalayer.client/README.md)
* [ctrlX Data Layer Client Browse](./datalayer.client.browse/README.md)
* [ctrlX Data Layer Client Light](./datalayer.client.light/README.md)
* [ctrlX Data Layer Client Subscription](./datalayer.client.sub/README.md)
* [ctrlX Data Layer Provider](./datalayer.provider/README.md)
* [ctrlX Data Layer Provider All Data Types](./datalayer.provider.all-data/README.md)
* [Example Remote Debugging with Python](./datalayer.remote.debug/README.md)
* [ctrlX Diagnostics Logbook](./logbook/README.md)
* [ctrlX Data Layer + Webserver](./webserver/README.md)


## Prerequisites for the Developing Environment

General prerequisites are described in the chapter [Setup Overview](../doc/setup_overview.md).

snapcraft has to be installed, if not call 

    ~/install-snapcraft.sh

The ctrlx-datalayer debian package has to be installed, if not install it - check versions.


    wget https://github.com/boschrexroth/ctrlx-automation-sdk/releases/download/1.12.1/ctrlx-datalayer-1.7.5.deb

    sudo dpkg -i ctrlx-datalayer-1.7.5.deb 

### Visual Studio Code Extensions for Python

* Start Visual Studio Code on your host computer.
* Connect to your SDK build environment.
* Open the folder of a Python sample under samples-python/
* Install the extension 'Python from Microsoft': Install in SSH:...

## Working with a Python Sample Project

For both debugging and building a snap let's use Visual Studio Code:

* From the main menu click File --> Open Folder and select the folder of your desired Python project.
* Start a bash terminal within the main menu item - Terminal - New Terminal.

### Build a Snap

A snap is build by snapcraft with this command - here for amd64 architecture:

    snapcraft --destructive-mode --target-arch=amd64 --enable-experimental-target-arch

To simplify calling snapcraft, you can start one of these scripts from the command line:

* __build-snap-amd64.sh__: Builds a snap for a ctrlX CORE<sup>virtual</sup>
* __build-snap-aarch64.sh__: Builds a snap for a ctrlX CORE M3/4
* build-snaps-remote.sh: Example to build snaps both for ctrlX CORE<sup>virtual</sup> and ctrlX CORE M3/4 remotely on [The Launchpad build farm](https://launchpad.net/builders).

The scripts can also be started by the Visual Studio Code UI. Therefor select main menu item Terminal - Run Build Task... 

When the build process is finished right click the snap file in the Visual Studio Code explorer window and select 'Download' to store it to your host. From there you can install it on your ctrlX CORE.

### Developing and Debugging the Python Project

#### Install Python Packages

We recommend to create and activate a virtual Python environment for your project. Furthermore all required packages have to be installed.

For ease of use you can call this script from the command line:

    ./install-venv.sh

The script runs following commands:

    virtualenv -p python3 venv
    source venv/bin/activate
    pip3 install -r requirements.txt

A subfolder venv will be created and activated as virtual environment which contains the Python runtime and all required packages.

#### Using Visual Studio Code 

For a first test

* Open main.py 
* Select the Python interpreter: Shift+Ctrl+P --> Select 'Python ... ('venv':venv) ... Recommended' *)
* Set a breakpoint to the first code line e.g. import ...
* Press F5 (Start Debugging)

*) Click update icon if 'Python ... ('venv':venv) ... Recommended' doesn't appear.

## Troubleshooting

* Enable SSH access to your ctrlX CORE.

* Watch trace of your snap on the ctrlX CORE e.g.: `$ sudo snap logs -f sdk-py-provider-alldata`

* Debugging Python code on the ctrlX CORE is decribed here [Remote Debugging](./debug.md).

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/) 

### Issues

If you've found an error in these samples, please [file an issue](https://github.com/boschrexroth)
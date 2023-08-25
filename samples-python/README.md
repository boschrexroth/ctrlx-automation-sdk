![Python](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c3/Python-logo-notext.svg/120px-Python-logo-notext.svg.png)

## Find out the supported samples


### Basic Samples

* [ctrlX Data Layer Client Light](datalayer.client.light/README.md)
* [ctrlX Data Layer Client Bulk](datalayer.client.bulk/README.md)
* [ctrlX Data Layer Client](datalayer.client/README.md)
* [ctrlX Data Layer Provider](datalayer.provider/README.md)
* [ctrlX Diagnostics Logbook](logbook/README.md)

### Advanced Samples

* [AppData File Storage](appdata/README.md)
* [ctrlX Data Layer Calculation operations](datalayer.calc/README.md)
* [ctrlX Data Layer Client Browse](datalayer.client.browse/README.md)
* [ctrlX Data Layer Client Subscription](datalayer.client.sub/README.md)
* [ctrlX Data Layer Provider All Data Types](datalayer.provider.all-data/README.md)
* [Example Remote Debugging with Python](datalayer.remote.debug/README.md)
* [ctrlX Data Layer + Webserver](webserver/README.md)

## Build a Snap

See [Samples Overview](../samples.md)

## Working with Visual Studio Code

### Visual Studio Code Extensions for Python

* Start Visual Studio Code on your host computer.
* Connect to your SDK build environment.
* Open the folder of a Python sample under samples-python/
* Install the extension 'Python from Microsoft': Install in SSH:...

### Working with a Python Sample Project

* From the main menu click File --> Open Folder and select the folder of your desired Python project.
* Start a bash terminal within the main menu item - Terminal - New Terminal.
* Install or upgrade the ctrlX Data Layer build and runtime environment.

```bash
sudo dpkg -i ../..deb/ctrlx-datalayer*.deb
```

#### Install a Virtual Environment

We recommend to create and activate a virtual Python environment for your project. Furthermore all required packages have to be installed.

Call this script from the command line:

    ./install-venv.sh

The script runs following commands:

    virtualenv -p python3 venv
    source venv/bin/activate
    pip3 install -r requirements.txt

A subfolder venv/ will be created and activated as virtual environment which contains the Python runtime and all required packages. 

*Hints:* The terminal console path begins with **(venv)**. With **Ctrl+Shift+P** open the *"Command Palette..."* and select *"Python: Select Interpreter"*, then *'venv/bin/python'* from the virtual environment.



#### Debugging with Visual Studio Code 

* Open main.py 
* Select the Python interpreter: Shift+Ctrl+P --> Select 'Python ... ('venv':venv) ... Recommended' *)
* Set a breakpoint to the first code line e.g. import ...
* Press F5 (Start Debugging)

*) Click update icon if 'Python ... ('venv':venv) ... Recommended' doesn't appear.

## Troubleshooting

* Enable SSH access to your ctrlX CORE.

* Watch trace of your snap on the ctrlX CORE e.g.: `$ sudo snap logs -f sdk-py-provider-alldata`

* Debugging Python code on the ctrlX CORE is described here [Remote Debugging Python](../remote-debug-python.md).

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/) 

### Issues

If you've found an error in these samples, please [file an issue](https://github.com/boschrexroth)
# Python Samples

![Python](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c3/Python-logo-notext.svg/120px-Python-logo-notext.svg.png)

## Prerequisites for the Developing Environment

* Install Visual Studio Code on your host computer.
* Download or clone the ctrlX AUTOMATION SDK to your host computer: git clone https://github.com/boschrexroth/ctrlx-automation-sdk.git
* Create and launch a QEMU virtual machine - see README-files within the folder public/scripts/environment

!!! important
    Python has no cross build capability. You have to run a QEMU VM according the destination CPU architecture of your ctrlX.

* ctrlX CORE virtual --> Create and launch an AMD64 QEMU virtual machine
* ctrlX CORE M3/M4 --> Create and launch an ARM64 (AARCH64) QEMU virtual machine

## Installing the ctrlX AUTOMATION SDK on your QEMU Virtual Machine

* From Visual Studio Code connect to the running QEMU virtual machine using SSH port 10022 (or another if you changed the default settings).
* Download or clone the ctrlX AUTOMATION SDK to your QEMU VM: 

    mkdir ~/git
    cd ~/git
    git clone https://github.com/boschrexroth/ctrlx-automation-sdk.git


## Working with a Sample Project

* From the Visual Studio Code main menu use File --> Open Folder and select the folder of your desired Python project.
* Start a bash terminal within the Visual Studio Code

### Build a Snap

In the bash terminal enter:

    snapcraft clean --destructive-mode
    snapcraft --destructive-mode

The tool snapcraft will use the files snap/snapcraft.yaml and setup.py to download all required Python packages and build a snap which can be installed on your ctrlX.

When the build process is finished right click the snap file in the VS Code explorer window and select 'Download' to store it to your host. From there you can install it on your ctrlX.

### Developing and Debugging the Python Project

#### Install Python Packages

We recommend to create and activate a virtual Python environment for your project. Start a terminal and enter:

    virtualenv -p python3 venv
    source venv/bin/activate

Install required packages only:

    python3 setup.py install

Install required and additional packages:

    pip3 install -r requirements.txt

#### Using Visual Studio Code 

For a first test 
* open main.py 
* set a breakpoint to the first code line e.g. import ...
* and press F5 (Start Debugging)

## Troubleshooting

Enable ssh access to your ctrlX.

If your snap doesn't work well start a shell on the ctrlX and check the trace regarding your snap e.g.: `$ sudo snap logs -f sdk-py-provider-alldata`

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/) 

### Issues

If you've found an error in these samples, please [file an issue](https://github.com/boschrexroth)

## Find out the Supported Samples 

TODO: Entweder hier alle oder keines der Samples auflisten (in der Übersicht sind diese bereits gelistet)

* ["Data Layer Provider"](./datalayer.provider/README.md) A lightweight Data Layer Provider which you can easily extend -> start with this example.

* ["Data Layer Client"](./datalayer.client/README.md) An extended Data Layer Client sample. It interacts with Data Layer nodes provided by the AllDataProvider snap. So first build and install this snap located in the directory public/samples-cpp/datalayer.provider.all-data). Then start working with this sample. 
  


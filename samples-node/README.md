# README node.js Apps @ ctrlX

![.NODEJS](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d9/Node.js_logo.svg/320px-Node.js_logo.svg.png)

## Prerequisites for the Developing Environment

* Install Visual Studio Code on your host computer.
* Download or clone the ctrlX AUTOMATION SDK to your host computer: git clone https://github.com/boschrexroth/ctrlx-automation-sdk.git
* Create and launch a QEMU virtual machine - see README-files within the folder public/scripts/environment

!!! important
    npm has no cross build capability. You have to run a QEMU VM according the destination CPU architecture of your ctrlX.

* ctrlX CORE virtual --> Create and launch an AMD64 QEMU virtual machine
* ctrlX CORE M3/M4 --> Create and launch an ARM64 (AARCH64) QEMU virtual machine

## Installing the ctrlX AUTOMATION SDK on your QEMU Virtual Machine

* From Visual Studio Code connect to the running QEMU virtual machine using SSH port 10022 (or another if you changed the default settings).
* Download or clone the ctrlX AUTOMATION SDK to your QEMU VM: 

    mkdir ~/git
    cd ~/git
    git clone https://github.com/boschrexroth/ctrlx-automation-sdk.git

## Working with a Sample Project

* From the Visual Studio Code main menu use File --> Open Folder and select the folder of your desired node.js project.
* Start a bash terminal within the Visual Studio Code

### Build a Snap

In the bash terminal enter:

    ./build_snap.sh

The tool snapcraft will use the files snap/snapcraft.yaml download all required node.js packages via npm plugin and build a snap which can be installed on your ctrlX.

When the build process is finished right click the snap file in the VS Code explorer window and select 'Download' to store it to your host. From there you can install it on your ctrlX.

### Developing and Debugging the node.js Project

#### Install node.js

We recommend to install nodejs via [snapcraft store](https://snapcraft.io/node):

    sudo snap install node --classic

#### Using Visual Studio Code 

For a first test 
* open src/main.js
* set a breakpoint to the first code line e.g. import ...
* and press F5 (Start Debugging)

## Troubleshooting

Enable ssh access to your ctrlX.

If your snap doesn't work well start a shell on the ctrlX and check the trace regarding your snap e.g.: `$ sudo snap logs -f ctrlx-node-datalayer-client`

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/) 

### Issues

If you've found an error in these samples, please [file an issue](https://github.com/boschrexroth)

## Find out the Supported Samples 

TODO: Entweder hier alle oder keines der Samples auflisten (in der Übersicht sind diese bereits gelistet)

* ["Data Layer Client"](./datalayer.client/README.md) A client sample for reading and subscribing values from ctrlX Data Layer.
* ["Data Layer Provider"](./datalayer.provider/README.md) A provider sample for providing nodes to ctrlX Data Layer.


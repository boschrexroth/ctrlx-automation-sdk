# Remote Debugging Snaps Python

## Overview

This document describes how applications can be debugged if they are running in the confined Ubuntu Core snap environment of a ctrlX CORE (Remote Debugging).

In this document first is briefly described how ctrlX CORE instances are connected over TCP/IP and SSH.

Then each following chapter describes for one programming language the  language-dependent features of snap debugging.

## Connecting ctrlX CORE devices

### IP Address of a ctrlX CORE

From the build environment ctrlX CORE devices are connected via TCP/IP. Assuming we are using a (QEMU) virtual machine as build environment the different kind of ctrlX CORE devices are reachable over these IP addresses:

1. ctrlX CORE^virtual^ with port forwarding: 10.0.2.2
2. ctrlX CORE^virtual^ with network adapter: 192.168.1.1
3. ctrlX CORE: real IP address

To point 1.:
From the point of view of the virtual machine this is the IP address of the host computer. The ctrlX CORE^virtual^ has no own IP address. The connections to the control are realized via port forwarding.

### Set up SSH Public Key Authentication

Debugging is based on SSH, so SSH should be set up between the debugger client on the host computer and the debugger server on the ctrlX CORE. To make logging in easier public key authentification should be installed.

For this kind of authorization a private / public key pair is generated. The private key is stored into a file kept secret on your host computer. The content of the public key is appended to the file ~/.ssh/authorized_keys in your home directory of the ctrlX CORE (remote computer).

!!! important
    You need an account with root rights and a home directory on the ctrlX CORE.

Creating the key pair and the installation both on the host computer and on the ctrlX CORE (remote computer) is described here

[Set up SSH public key authentication to connect to a remote system](https://kb.iu.edu/d/aews)

or here (in German)

[OpenSSH Public Key Authentifizierung unter Ubuntu](https://www.thomas-krenn.com/de/wiki/OpenSSH_Public_Key_Authentifizierung_unter_Ubuntu).

Additional hints:

```code
- Use standard file names: "id_rsa" for the private and "id_rsa.pub" for the public key file.
- On the host computer set the (UNIX) permissions attributes for "id_rsa" to 600 (-rw-------)
- On the host computer use the file "~/.ssh/config" to configure several ssh connections to different ctrlX CORE devices.
```

## Remote Debugging Python

This chapter describes remote debugging of snap written in Python.

In the sample project samples-python/datalayer.remote.debug all the features described in this chapter have been implemented.

### Using Visual Studio Code - Additional Tasks in launch.json

On the host computer Visual Studio Code is used as Integrated Development Environment (IDE).

For general informations regarding Python debugging in Visual Studio Code see [https://code.visualstudio.com/docs/python/debugging](https://code.visualstudio.com/docs/python/debugging), chapter "Debugging by attaching over a network connection".

The Python Visual Studio Code extension has a Python debug client. For a remote debugging session we have add a configuration set with these parameters/values:

* The request type is __"attach"__: "request": "attach",
* The IP address of the ctrlX CORE has to be provided e.g.: "host": "192.168.2.61",
* The port number where the debug server is listening has to be provided e.g.: "port": 12345,
* and a link to the exe

Here you can find the complete file: samples-python/datalayer.remote.debug/.vscode/launch.json

There are three launch configurations for remote debugging:

* "Remote ctrlX COREvirtual - Port forwarding"
* "Remote ctrlX COREvirtual - Network adapter"
* "Remote ctrlX CORE"

### Preparing your Python Project for Remote Debugging

Installing the required Python packages into your build environment ist described here: samples-python/python.md

In our sample project the Python package  __debugpy__ is used for remote debugging of Python code.

#### The Python Package debugpy

The package debugpy provides a debug server and functions to configure the debug connection and to set breakpoints.

The configuration of the debug connection and the start of the debug server have to be called in the Python application program so that the application stops (preferred in the startup phase) and waits for a debug client whow wants attach to the process.

Setting breakpoints by program code is optional.

To make the usage of debugpy easier the essential function calls are extracted and provided in the script samples-python/datalayer.remote.debug/debugging.py.

* debugging.init() scans the applications parameter list for --debug-port=... If this parameter is not found remote debugging will be not enabled. Usage see main.py.
* debugging.wait_for_client(port: int) is an internal function called by init(). It configures the remote debug connections and starts the debug server. The application is stopped until a debug client connects.
* With debugging.breakpoint() breakpoints can be set by the Python code itself - see main.py.

#### Packing debugpy into your snap

snapcraft will pack debugpy into the snap because it is listed in the parameter "install_requires" of the function setup() in setup.py.

### Creating and Installing a snap

Start a terminal and enter

```code
    snapcraft clean --destructive-mode
    snapcraft --destructive-mode
```

!!! important
    To create a snap for a ctrlX CORE you need an aarch64 (arm64) building environment e.g. a QEMU VM with aarch64 emulation.

Install the snap on your ctrlX CORE.

After installation of the snap the app will be started at once - but without remote debugging enabled.

### Start your Python App in Remote Debugging Mode

Connect via SSH to your ctrlX and start your app in debugging mode.

Stop the snap:

```code
    sudo snap stop sdk-py-remote-debug
```

Start the app providing a debug port as command line parameter:

```code
    sudo snap run sdk-py-remote-debug.app --debug-port=12345
```

!!! important
    Ensure that the port number is the same as configured in launch.json.

Now the app holds on startup and waits for a debug client to connect.

### Do Remote Debugging with Visual Studio Code

In Visual Studio Code

* Click the 'Run and Debug' icon in the side bar,
* select the according launch configuration for remote debugging
* and enter F5

Visual Studio Code will connect to the debug server on the ctrlX CORE and attach to the process.

Breakpoints can be set within the Python editor. Furthermore breakpoints set programatically are active.

### Using Python Package debugpy in your own Projects

To use remote debugging in your own projects just do following steps:

* Copy the script samples-python/datalayer.remote.debug/debugging.py into your project folder
* Install debugpy into your development environment e.g. by adding 'debugpy' into the file requirerequirements.txt
* Add debugpy to the list of parameter install_requires=[...]

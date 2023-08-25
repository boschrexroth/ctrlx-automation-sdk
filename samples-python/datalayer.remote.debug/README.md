# Python sample for remote debugging

This sample shows how remote debugging can be done with Python.

## Prerequisites

If you use a ctrlX CORE<sup>virtual<sup/> enable port forwarding of your remote debug port e.g. for port 15678

    8022:22,8443:443,8740:11740,4840:4840,2069:2069,2070:2070__,15678:15678__

## Implementation

Remote debugging is enabled with by the package [__debugpy__](https://pypi.org/project/debugpy/)

The script debug.py provides the function __debug.remote_debugging_wait_for_client()__ were debugpy is used.

In main.py __debug.remote_debugging_wait_for_client()__ is called. 

Installed on a ctrlX CORE the service stops executing and waits until a remote debug client will attach.

!!! important 
    Feel free to use the script debug.py in your own projects.

## Build snap and install and start it

For this job we recommend to use our shell script, here for a ctrlX ctrlX CORE<sup>virtual<sup/> with port forwarding

    ../../scripts/build-upload-log-snap.sh -PF

and here with network adapter

    ../../scripts/build-upload-log-snap.sh -NA

For a bare metal ctrlX CORE you are able to provide all settings via command line, call this for help:

    ../../scripts/build-upload-log-snap.sh -help 

When build and installation succeeded the snap is started as a service waiting for a remote debug client to connect. This print out should be shown e.g.:

    __Accepting remote debug client attaches to port 15678__

!!! hint
    In Visual Studio Code you can use menu item Terminal - Run Build Task... - Build upload snap ...

## Start debugging wih Visual Studio Code


The file .vscode/launch.json contains launch configurations for remote debugging. 

We assume to debug the snap on a ctrlX ctrlX CORE<sup>virtual<sup/> with port forwarding click the launch icon on the side bar and select configuration 'Remote ctrlX COREvirtual - Port forwarding'.

The debugging client of Visual Studio Code will connect to the waiting server and debugging will start.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

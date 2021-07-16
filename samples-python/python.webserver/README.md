# README python.webserver

This python app __python.webserver__ deploys a simple webserver including Data Layer connection and user permissions with webinterface accessible from ctrlX webpage.
## Introduction

The app shows how to build a simple webserver with reverse proxy and Data Layer access, setup user permissions through package-manifest and token authentification.

## Prerequisites for Developing Python Apps

* Linux Ubuntu 18.4 (or Windows PC running with Windows Subsystem for Linux (WSL) or a Virtual Box VM)
* Python3 is installed (VS Code extension in case of using WSL) 
* ctrlX AUTOMATION SDK Version 1.8 is installed (extracted and copied to the Ubuntu users home directory)

## Getting Started

As Integrated Development Environment (IDE) we recommend Visual Studio Code.
Therefor the folder .vscode contains configuration files.

### Install Virtual Environment

We recommend to create and activate a virtual environment. Start a terminal in the project directory (workspaceFolder) and enter:

    virtualenv -p python3 venv
    source venv/bin/activate

### Install Wheels

Install the required packages:

    pip3 install -r requirements.txt
    pip3 install ../../whl/ctrlx_datalayer*
    pip3 install ../../whl/ctrlx_fbs-*

### Start IDE

Start your IDE, open main.py and start the program.

## Build an AMD64 Snap

Check your snapcraft version with the command:

    snapcraft --version

For cleaning and building snaps see according chapter.

### Version 2.43

To clean the previous snap build results use:

    snapcraft clean

Build the snap with:

    snapcraft

### Version 4.x or higher

To clean the previous snap build results use:

    snapcraft clean --destructive-mode

Build the snap with:

    snapcraft --destructive-mode

__Important:__  To build an ARM64 (AARCH64) snap use a native ARM64 computer or a virtual machine emulating the ARM64 CPU e.g. with QEMU.


### Debug App
With debugging inside your local development environment you can run the program and step through the code while its connected to the Data Layer of your ctrlX CORE via tcp.
1. Select the Run icon in the Activity Bar on the side of VS Code. You can also use the keyboard shortcut Ctrl+Shift+D.
2. Click on the green play button on the top of VS Code to start debbuging process.

Hint: Because the webserver checks the authentification token, its not possible to access the webpage while debugging inside your local development. Disable token checking in webserver.py to be able to access the webpage through url http://127.0.0.1:1234/python-webserver.


## Install the Snap on the ctrlX CORE

* Open the ctrlX CORE Home page, select Settings - Apps, click on 'Service mode' and confirm.
* Click on the Settings icon and select 'Allow installation from unknown source'
* Select tab 'Local storage', click the + icon, upload and install the snap.
* Switch to Operation mode

## How to Use
Once the installation of the snap was finished, you can access to the provided webpage through the sidebar or overview menu of ctrlX CORE webpage.

![ctrlX Integration](./docs/images/ctrlx_integration.png)


The Python-Webpage allows you to read and write at the Data Layer (depends on permissions of the current user) and shows permissions of the current user. 

![Python Webserver](./docs/images/python_webserver.png)

You can find the configuration of user permissions (scopes) at settings section <ins>Users & Permissions > Users</ins>

![ctrlX User Permissions](./docs/images/ctrlx_userpermissions.png)

The integration of the app inside ctrlX Core webpage (sidebar and overview integration, reverse proxy, bearertoken, scopes) is done by the package-manifest (<ins>./configs/package-assets/rexroth-python-webserver.package-manifest.json</ins>). Take a look at [package-manifest](./../package-manifest.md) to find out the basics.


## Troubleshooting

* If your snap doesn't work well start a shell on the ctrlX and check the trace regarding your snap: `$ sudo snap logs -f rexroth-python-provider`

## Support

If you've any questions visit the [ctrlX AUTOMATION Communitiy](https://developer.community.boschrexroth.com/)

___

## License

MIT License

Copyright (c) 2020-2021 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

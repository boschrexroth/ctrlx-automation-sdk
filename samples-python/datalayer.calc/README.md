# ctrlX Data Layer Calculator Sample in Python 

This app is a ctrlX Data Layer Provider and Client written in Python.

## Introduction

The app reads two Data Layer values and calculates the output value 'out'. All values are float64 value types.

The Data Layer addresses of the input values are configured by the string input values 'in1' and 'in2'.

How the out value is calculated can be configured as string value of the node 'mode'.

 Supported operations are:

- +
- -
- *
- /

## Design 

There are two Python scripts:

- main.py - the main function
- calculations.basic_arithmetic_operations.py - Class called BasicArithmeticOperations which reads the input values and calculates the output value.

### main.py

This function manages the Data Layer Provider and Client connection.

Furthermore it creates one instance of the BasicArithmeticOperations class and starts it.

If the client connection is lost the main function exits with an error code.

If the app is installed as snap the system will restart the application after a short period of time.

### calculations.basic_arithmetic_operations.py

This class BasicArithmeticOperations has several functions.

It is a Data Layer Provider for four nodes: 
1. in1: string, specifies the DL address of the first operand
2. in2: string, specifies the DL address of the second operand
3. mode: string, specifies the operation
4. out: float64, the result

With the address values of in1 and in2 a subscription is started. Everytime when new values are published the value of out is recalculated.

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

### Install the Snap on the ctrlX

* Open the ctrlX CORE Home page, select Settings - Apps, click on 'Service mode' and confirm.
* Click on the Settings icon and select 'Allow installation from unknown source'
* Select tab 'Local storage', click the + icon, upload and install the snap.
* Switch to Operation mode


## Support

If you've any questions visit the [ctrlX AUTOMATION Communitiy](https://developer.community.boschrexroth.com/)

## License

MIT License

Copyright (c) 2021 Bosch Rexroth AG

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

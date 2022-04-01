# README hello.plc

## Introduction

The sample project __hello.plc__ shows the write access to some ctrlX PLC variables as a Data Layer client on ctrlX CORE or ctrlX CORE <sup>virtual</sup>.

## Prerequisites

* Running ctrlX CORE with app Rexroth PLC installed.
* From ctrlX WORKS start `ctrlX PLC Engineering`:
* Open, compile, download and start the PLC project hello.plc.project of the subfolder plc_application

With a web browser login into your ctrlX CORE and check the Datalayer folder plc/app/hello_plc_application/sym/PLC_PRG.
Here all PLC variables should appear.

## Function Description

When the app has been started, all variables from the PLC program "PLC_PRG" that are present in the PLC symbol configuration will be read out.

Then the following is executed once:

* the value "HelloDeveloper" is written to each variable of type STRING.
* the value "1337" is written to each variable of type INT16 (ctrlX-Plc: INT).
* the value "0.815f" is written in each variable of type FLOAT32 (ctrlX-Plc: REAL).

All variables with another data type are ignored.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open the web interface of your ctrlX CORE
* Login with `ctrlX PLC Engineering` and watch the values of the variables. Variables in the PLC program "PLC_PRG" that are available in the PLC symbol configuration should have the following values:
    * STRING variables should have the value "HelloDeveloper".
    * INTEGER variables should have the value "1337"
    * REAL variables should have the value "0.815f"

## Screenshots

![Declare some plc variables](docs/images/hello.plc/hello.plc.declare.variables.offline.png)
_Declare some plc variables_
![Declare some plc variables](docs/images/hello.plc/hello.plc.symbolconfiguration.png)
_symbolconfiguration PLC_PRG selected_
![Status of the plc-variables before the snap runs](docs/images/hello.plc/hello.plc.status.variables.online.png)
_Status of the plc-variables before the snap runs_
![Status of the plc-variables after the snap runs](docs/images/hello.plc/hello.plc.status.variables.online2.png)
_Status of the plc-variables after the snap runs_
![Output in diagnosis logbook](docs/images/hello.plc/messages.in.logbook.png)
_Output in diagnosis logbook_

___

## License

MIT License

Copyright (c) 2020-2022 Bosch Rexroth AG

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
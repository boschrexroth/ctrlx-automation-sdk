# README hello.plc

## Introduction

This sample project shows the ctrlX Data Layer access to ctrlX PLC variables of a ctrlX CORE or ctrlX CORE<sup>virtual</sup>.

## Prerequisites

* ctrlX WORKS with `ctrlX PLC Engineering` installed.
* ctrlX CORE with app PLC installed.
* __IMPORTANT: Do not use this project in a control that is in production or in which the PLC program or its variables must not be changed.__

## Function Description

The app browses all PLC variables mapped to the ctrlX Data Layer.

If the attached app 'hello_plc_application' has been installed, its variables will be overwritten:

* "HelloDeveloper" is written to each variable of type STRING.
* "1337" is written to each variable of type INT16 (ctrlX-Plc: INT).
* "0.815f" is written in each variable of type FLOAT32 (ctrlX-Plc: REAL).

All variables with another data type are ignored.

## Debug and Run

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

### Read Access to Variables of your PLC Application

* Install your default PLC application in the ctrlX CORE.
* Install this sample project as snap or start it in the App Build Environment

All DL variables will be listet but not overwritten.

### Read/Write Access to the Variables of our Test PLC Application

* Download the file hello_plc_application.export from sub folder plc_application/
* Create a new PLC project with ctrlX PLC Engineering.
* Select the node PLC Logic in the project tree with the cursor.
* Click main menu item Project -> Import... and import the whole content.
* Login into your PLC runtime (project will be compiled and downloaded).
* Start your PLC application (F5) - be sure your ctrlX is in Operting
* Install this sample project as snap or start it in the App Build Environment

All DL variables will be listet, INT, REAL (Float) and STRING values are overwritten.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

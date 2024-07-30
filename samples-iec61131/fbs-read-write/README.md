# Using Flatbuffers in IEC 61131-3 ST Code

The ctrlX data layer contains many nodes that provide their data in Flatbuffers.

The sample shows how a Flatbuffers node can be read, changed and written.

## Prequisites for your Windows host

* ctrlX WORKS >= 2.3.0 installed
* ctrlX PLC Engineering >= 2.3.0 installed
* ctrlX CORE^virtual^ with running app PLC
* ctrlX AUTOMATION SDK >= 2.2.0 installed

## Steps to build and download the PLC project

We describe the usage of a ctrlX CORE^virtual^.

* Start ctrlX PLC Engineering.
* Create a new project, use __ctrlX CORE x64 Project__ as template.
* Within the device view select the node Application and delete it.
* Select node PLC Logic.
* Select main menu item Project -> PLCOpenXML import
* From ctrlX AUTOMATION SDK select file __samples-iec61131\fbs-read-write\fbs-datalayer-subscriptions-settings.xml__
* Import complete content.
* Select PLC node Libray Manager
* Delete placeholder StringUtils and add the library with the same name.
* Select PLC node Application, right click and select context menu item 'Import from Flatbuffers File...'
* From ctrlX AUTOMATION SDK select file __fbs\comm\datalayer\sub_settings.fbs__.

The fbs file is compiled into IEC61131 Structed Text and imported into your PLC project.

* Compile the project - there should by no error
* Connect your PLC device node with the CORE^virtual^
* Login (download) and start your PLC project.

## Test the project

* Open a web browser, log in into your ctrlX CORE^virtual^
* Select Settings - Data layer
* Select node datalayer/subscriptions/settings

The value of maximumBufferSize should be changed by your PLC project.
___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

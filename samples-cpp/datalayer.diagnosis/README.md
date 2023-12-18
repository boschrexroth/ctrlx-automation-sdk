# ctrlX Data Layer Diagnosis

This sample app uses some ctrlX Data Layer addresses to handle with diagnostics.

## Introduction

It demonstrates how to
* register and unregister diagnostics
* subscribe to specific diagnostic events
* set and reset diagnostic logs
* confirm diagnostic errors
* get the highest priority pending log
* get the actual pending list of errors and warnings
* get diagnostic texts from diagnostic numbers

## Prerequisites

* Installed app rexroth-automationcore

## Debug and Run

1. Build the executable via cmake plugin.
2. Launch the app with the debugger attached by pressing __F5__.

## Build and Install the Snap

Build and install the snap like described [here](../README.md).

## Test the Snap

* Open the web interface of your ctrlX CORE
* Check output in the web interface


![Output in diagnosis logbook](docs/images/datalayer.diagnosis/datalayer_tree_diagnosis.png)

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

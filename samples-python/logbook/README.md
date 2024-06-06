# ctrlX Diagnostics Logbook

## Introduction

This python app showes how to log with different log levels into the logbook of the ctrlX CORE.

## Function Description

The app writes log messages using the Python package [cysystemd](https://github.com/mosquito/cysystemd) version 1.5.4
and Python version 3.10.

## Implementation Description

__main.py__ just calls the function in logbook/app.py.

The methon run() in __logbook/app.py__ sends the messages.

## Hint to Cross Build

This example is based on python library 'cysystemd'. 'cysystemd' used within 'shared libraries'.
Version 1.5 provides wheel packet for the different architectures (arm64, amd64).
With version 1.6 these shared libraries are created for installation and there are problems with the cross build,
because these are created on the basis of the build system and not on the basis of the target system.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

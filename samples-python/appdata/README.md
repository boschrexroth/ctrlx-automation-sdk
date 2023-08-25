# AppData File Storage

## Introduction

This Python sample app shows how to persist application data in ctrlX CORE. 

## Function Description

This app uses a web server to listen for events regarding loading and saving configuration data.

Running in the snap environment of a ctrlX CORE therefor a web socket connection is used. In the build environment a TCP/IP connection is used.

## Implementation Description

__main.py__ starts the web server in TCP or Unix socket mode.

The package __web__  contains the web server implementation using the build in Python module http.server. For encoding and decoding JSON Web Tokens the Python library pyjwt is used.

The folder __configs__ contains the file package-manifest.json. It is used to register the snap within the ctrlX CORE as participant of the configuration load/store mechanism. 

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT
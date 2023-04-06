# AppData File Storage

## Introduction

This Python sample app shows how to persist application data in ctrlX environment. 

## Function Description

This app uses a web server to listen for events regarding loading and saving configuration data.

Running in the snap environment of a ctrlX CORE therefor a web socket connection is used. In the build environment a TCP/IP connection is used.

## Implementation Description

__main.py__ starts the web server in TCP or Unix socket mode.

The package __web__  contains the web server implementation using the build in Python module http.server. For encoding and decoding JSON Web Tokens the Python library pyjwt is used.

The folder __configs__ contains the file package-manifest.json. It is used to register the snap within the ctrlX CORE as participant of the configuration load/store mechanism. 

___

## License

MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

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
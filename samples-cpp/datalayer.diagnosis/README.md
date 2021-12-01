# README datalayer.diagnosis

This sample app uses some Data Layer addresses to handle with diagnostics.

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

* Build environment - we recommend a QEMU virtual machine
* ctrlX AUTOMATION SDK archive
* ctrlX CORE<sup>virtual</sup> or ctrlX CORE

### Prerequisites for Building the cpp Samples on the Build Environment

* Start your build environment
* Copy the ctrlX AUTOMATION SDK archive from your host computer to your build environment:

    scp -P 10022 com.boschrexroth.sdk.control-*.zip boschrexroth@localhost:~

* Open a SSH shell on your build environment:

    ssh -p 10022 boschrexroth@localhost

* Unzip the archive

    unzip -XK com.boschrexroth.sdk.control-*.zip

* Change to SDK directory public
* Set x flag: 

    chmod +x bin\oss.flatbuffers\u*\release\flatc

* If installed remove older ctrlx-datalayer package

    sudo dpkg -r ctrlx-datalayer 

* Install newest ctrlx-datalayer package

    sudo dpkg -i deb/ctrlx-datalayer*.deb

## Getting Started

1. Launch IDE (VSCode for example)
2. Open the sample directory `samples-cpp/datalayer.diagnosis`
3. Build and install the snap as described `Setup` section

## Screenshots

![Output in diagnosis logbook](docs/images/datalayer.diagnosis/datalayer_tree_diagnosis.png)

## Troubleshooting

All automatically created files are located in folders `build` and `generated`.  
If there are unclear messages during the build process, it might help to delete the folders `build` and `generated` and restart the build process.

## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

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

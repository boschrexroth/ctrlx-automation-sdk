# Example ctrlX Data Layer Provider

## Introduction

The app demonstrates how a Data Layer connecion is established and a string and flatbuffers node are provided.

## Function Description

The app connects to the ctrlX Data Layer and provides a string and flatbuffers node .

If the Data Layer connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

## Prerequisites

see the description on [ctrlX Data Layer client and provider library for golang](https://github.com/boschrexroth/ctrlx-datalayer-golang)

## Visual Studio Code

To use Go within Visual Studio Code, we recommend using the [Go for Visual Studio Code](https://github.com/golang/vscode-go) extension. The corresponding documentation can also be found under this link. 

Open this project folder directly, then the go build environment is set correctly. 

## Compile Flatbuffers Schema

This app uses a flatbuffer defined in the schema file sampleSchema.fbs. From this schema file we have to create a binary file which is registered on the ctrlX and a golang structure to create and access the defined flatbuffer data. These tasks can be done with the command line tool flatc.

This tool is provided by this SDK under /bin/oss.flatbuffers/..insert OS version here../release/

Do following steps - OS version here is 'ubuntu20-gcc-x64':

* `chmod +x ../../bin/oss.flatbuffers/ubuntu20-gcc-x64/release/flatc`

* Create a binary flatbuffers file:

    `../../bin/oss.flatbuffers/ubuntu20-gcc-x64/release/flatc -o bfbs/ -b --schema fbs/sampleSchema.fbs`

* Generate the Go code:

    `../../bin/oss.flatbuffers/ubuntu20-gcc-x64/release/flatc --go fbs/sampleSchema.fbs`

The generated code is stored in sample/schema/InertialValue.go

## Building

Dependency updates with `go mod tidy`

Build the executable with `go build ./cmd/...`

Build the snap with `snapcraft --destructive-mode`

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

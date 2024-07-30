# Example ctrlX Data Layer Provider

## Introduction

The app demonstrates how a ctrlX Data Layer connecion is established and a string and flatbuffers node are provided.

## Function Description

The app connects to the ctrlX Data Layer and provides a string and flatbuffers node .

If the ctrlX Data Layer connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

## Prerequisites

see the description on [ctrlX Data Layer client and provider library for Go](https://github.com/boschrexroth/ctrlx-datalayer-golang/v2)

## Visual Studio Code

To use Go within Visual Studio Code, we recommend using the [Go for Visual Studio Code](https://github.com/golang/vscode-go) extension. The corresponding documentation can also be found under this link.

Open this project folder directly, then the go build environment is set correctly.

## Compile Flatbuffers Schema

This app uses a flatbuffer defined in the schema file sampleSchema.fbs. From this schema file we have to create a binary file which is registered on the ctrlX and a Go structure to create and access the defined flatbuffer data. These tasks can be done with the command line tool flatc.

This tool is provided by this SDK under /bin/oss.flatbuffers/..insert OS version here../release/

* Create a binary flatbuffers file:

    `<path to>/flatc -o bfbs/ -b --schema fbs/sampleSchema.fbs`

* Generate the Go code:

    `<path to>/flatc --go fbs/sampleSchema.fbs`

The generated code is stored in sample/schema/InertialValue.go

## Building

Dependency updates with `go mod tidy`

Build the executable with `go build ./cmd/...`

Build the snap with `snapcraft --destructive-mode`

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

# Example ctrlX Data Layer Client Extensions

## Introduction

The sample demonstrates how to uses the subcription and bulk from the ctrlX Data Layer.

## Function Description

The app connects to the ctrlX Data Layer using either port 8443 or 443.
A subscription is used to get values by data change event in a deterministic publish interval.
The single read is performed every 10 seconds in an endless loop.

If the ctrlX Data Layer connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

## Prerequisites

See the description on [ctrlX Data Layer client and provider library for golang](https://github.com/boschrexroth/ctrlx-datalayer-golang)

## Visual Studio Code

To use Golang within Visual Studio Code, we recommend using the [Go for Visual Studio Code](https://github.com/golang/vscode-go) extension. The corresponding documentation can also be found under this link.

Open this project folder directly, then the go build environment is set correctly.

## Building

Dependency updates with `go mod tidy`

Build the executable with `go build ./cmd/...`

Build the snap with `snapcraft --destructive-mode`

## Runtime

This application needs "sdk-cpp-alldata" as provider.

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

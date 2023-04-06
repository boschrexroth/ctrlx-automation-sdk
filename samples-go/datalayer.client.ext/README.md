# Example ctrlX Data Layer Client Extensions

## Introduction

The sample demonstrates how to uses the subcription and bulk from the ctrlX Data Layer.

## Function Description

The app connects to the ctrlX Data Layer using either port 8443 or 443.
A subscription is used to get values by data change event in a deterministic publish interval.
The single read is performed every 10 seconds in an endless loop.

If the Data Layer connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

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

MIT License

Copyright (c) 2021-2023 Bosch Rexroth AG

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

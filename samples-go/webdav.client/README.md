# Example WebDAV Client

## Introduction

The sample demonstrates how to access to configuration files of a ctrlX device remotely using the WebDAV protocol.

## Function Description

The application creates a webdav client and connects to a virtual control (localhost:8443) with the default credentials.
It executes some file operations on the ctrlX-filesystem.

## Visual Studio Code

To use Golang within Visual Studio Code, we recommend using the [Go for Visual Studio Code](https://github.com/golang/vscode-go) extension. The corresponding documentation can also be found under this link.

Open this project folder directly, then the go build environment is set correctly.

## Building

Dependency updates with `go mod tidy`
Build the executable with `go build ./cmd/...`

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

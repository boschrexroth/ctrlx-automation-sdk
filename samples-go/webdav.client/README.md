# Example WebDAV Client

## Introduction

The sample demonstrates how to access to configuration files of a ctrlX CORE remotely using the WebDAV protocol.

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

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

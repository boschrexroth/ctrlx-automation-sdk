# Example ctrlX Data Layer Client

## Introduction

The sample demonstrates how to use bulk operations to handle values of the ctrlX Data Layer.

## Function Description

The app connects to the ctrlX Data Layer using either port 8443 or 443.
A subscription is used to get values by data change event in a deterministic publish interval.
The single read is performed every 10 seconds in an endless loop.

If the ctrlX Data Layer connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

## Prerequisites

See the description on [ctrlX Data Layer client and provider library for Java](../README.md)

## Visual Studio Code

To use Java within Visual Studio Code, we recommend using the [Java in Visual Studio Code](https://code.visualstudio.com/docs/languages/java) extension or see [sample-java](../README.md).

## Building

```bash
    ./gradlew clean
    ./gradlew build
```

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

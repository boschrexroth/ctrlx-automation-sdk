# Example ctrlX Data Layer Provider Subscription

## Introduction

The app demonstrates shows how a provider supports "__ctrlX Data Layer__" subscription interface.

## Function Description

The app connects to the __ctrlX Data Layer__ using either port 8443 or 443.
Provider supported the subscription interface.

## Prerequisites

See the description on [ctrlX Data Layer client and provider library for Java](../README.md)

## Visual Studio Code

To use Java within Visual Studio Code, we recommend using the [Java in Visual Studio Code](https://code.visualstudio.com/docs/languages/java) extension or see [sample-java](../README.md).

## Building

```bash
    ./gradlew clean
    ./gradlew build
    ./gradlew run --args="provider"
    ./gradlew run --args="client"
```

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

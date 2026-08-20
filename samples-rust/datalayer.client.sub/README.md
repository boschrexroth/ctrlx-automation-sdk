# Example ctrlX Data Layer Client

## Introduction

The sample demonstrates how to uses the subcription from the __ctrlX Data Layer__.

## Function Description

The app connects to the __ctrlX Data Layer__ using either port 8443 or 443.
A subscription is used to get values by data change event in a deterministic publish interval.

If the __ctrlX Data Layer__ connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

## Prerequisites

See the description on [ctrlX Data Layer client and provider library for Rust](../README.md)

## Visual Studio Code

See [samples-rust](../README.md).

## Building

```bash
    cargo build
```

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

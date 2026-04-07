# Example ctrlX Data Layer Provider

## Introduction

The sample demonstrates how to read values from __ctrlX Data Layer__ and write out the values to console.

## Function Description

The app connects to the __ctrlX Data Layer__ using either port 8443 or 443.
A subscription is used to get values by data change event in a deterministic publish interval.
The single read is performed every 10 seconds in an endless loop.

If the __ctrlX Data Layer__ connection breaks the app will exit. After 10s it will be automatically restarted by snapd.

## Prerequisites

See the description on [ctrlX Data Layer client and provider library for Java](../README.md)

## Visual Studio Code

To use Java within Visual Studio Code, we recommend using the [Java in Visual Studio Code](https://code.visualstudio.com/docs/languages/java) extension or see [sample-java](../README.md).

## Building

```bash
    ./gradlew clean
    ./gradlew build
```

## Compile Flatbuffers Schema

This app uses a flatbuffer defined in the schema file sampleSchema.fbs. From this schema file we have to create a binary file which is registered on the ctrlX and a Java structure to create and access the defined flatbuffer data. These tasks can be done with the command line tool flatc.

This tool is provided by this SDK under /bin/oss.flatbuffers/..insert OS version here../release/

* Create a binary flatbuffers file:

    `<path to>/flatc -o bfbs/ -b --schema fbs/sampleSchema.fbs`

* Generate the Java code:

    `<path to>/flatc --gen-object-api --gen-compare --no-warnings --suppress-validate-version --java -o app/src/main/java fbs/sampleSchema.fbs`

The generated code is stored in pkg/sample/schema/InertialValue.java

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

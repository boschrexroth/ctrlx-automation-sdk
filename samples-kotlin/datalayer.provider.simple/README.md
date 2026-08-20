# Example ctrlX Data Layer Provider (Kotlin)

This tutorial shows how to build and package a small Kotlin app that acts as a ctrlX Data Layer provider. You will generate the FlatBuffers schema, build the application, and pack it as a snap.

## What You Will Build

The sample registers four provider nodes under the Data Layer address root `sdk/kotlin/provider/simple/`:

- `string`
- `int32`
- `timestamp`
- `inertial-value` using FlatBuffers

When the Data Layer connection drops, the app exits and snapd restarts it after 10 seconds.

## Before You Start

This sample is the Kotlin version of the [datalayer.provider.simple Java sample](../../samples-java/datalayer.provider.simple/README.md). For general ctrlX Data Layer setup, see [Samples Java overview](../../samples-java/README.md).

You also need the Kotlin extension in Visual Studio Code if you want editor support:

- [Kotlin extension](https://marketplace.visualstudio.com/items?itemName=fwcd.kotlin)

## 1. Generate The FlatBuffers Code

The sample uses the schema in `fbs/sampleSchema.fbs`. From that schema, generate the binary schema file that is registered on ctrlX and the Kotlin FlatBuffers classes used by the app.

The SDK provides `flatc`. On this workspace it is available at `public/bin/oss.flatbuffers/ubuntu24-gcc-x64/flatc` for x64 builds.

Generate the files with:

```bash
public/bin/oss.flatbuffers/ubuntu24-gcc-x64/flatc \
    -o bfbs/ -b --schema fbs/sampleSchema.fbs

public/bin/oss.flatbuffers/ubuntu24-gcc-x64/flatc \
    --gen-object-api --gen-compare --no-warnings --suppress-validate-version \
    --kotlin -o app/src/main/kotlin fbs/sampleSchema.fbs
```

The generated Kotlin code is stored in `app/src/main/kotlin/pkg/sample/schema/InertialValue.kt`.

## 2. Build The App

Build the Kotlin application with Gradle:

```bash
./gradlew clean build
```

This compiles the Kotlin sources, runs the tests, and assembles the application jar.

## 3. Build The Snap

To package the sample as a snap for amd64, run:

```bash
./build-snap-amd64.sh
```

If you want an arm64 build, use:

```bash
./build-snap-arm64.sh
```

After the snap starts, open the app from the ctrlX OS sidebar as `SDK Kotlin Provider`.

The app is integrated through reverse proxy mapping to `/sdk-kotlin-provider-simple/` and the webserver is bound to a Unix socket in snap mode.

For local non-snap runs, the webserver listens on `http://127.0.0.1:8080/` and exposes `http://127.0.0.1:8080/api/provider`.

## Project Structure

The app is intentionally small and split into a few focused pieces:

- `app/src/main/kotlin/datalayer/provider/simple/App.kt` starts the Data Layer connection, registers the nodes, and keeps the provider running.
- `app/src/main/kotlin/datalayer/provider/simple/ProviderNodeCallbackString.kt`, `app/src/main/kotlin/datalayer/provider/simple/ProviderNodeCallbackInteger.kt`, and `app/src/main/kotlin/datalayer/provider/simple/ProviderNodeCallbackTimestamp.kt` implement the simple scalar nodes.
- `app/src/main/kotlin/datalayer/provider/simple/ProviderNodeCallbackFbs.kt` shows how to publish a FlatBuffers value from Kotlin.
- `app/src/main/kotlin/datalayer/provider/simple/ProviderWebServer.kt` exposes the current provider values over HTTP.
- `app/src/main/kotlin/pkg/sample/schema/InertialValue.kt` is the generated FlatBuffers binding used by the app.
- `fbs/sampleSchema.fbs` is the schema input that generates the Kotlin binding and the binary schema.
- `bfbs/sampleSchema.bfbs` is the binary schema registered on ctrlX at runtime.
- `snap/snapcraft.yaml` packages the app as a snap and defines the runtime environment.
- `configs/package-assets/sdk-kotlin-provider-simple.package-manifest.json` defines reverse proxy mapping and sidebar menu entries.

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

# Samples Kotlin

![Kotlin](https://upload.wikimedia.org/wikipedia/commons/thumb/7/74/Kotlin_Icon.png/120px-Kotlin_Icon.png)

## Prerequisites

* ctrlX WORKS
* ctrlX AUTOMATION App Build Environment
* ctrlX AUTOMATION SDK installed in the App Build Environment
* ctrlX CORE<sup>virtual</sup> or ctrlX OS device

## Find out the supported samples

### Basic Samples

* [ctrlX Data Layer Provider](./datalayer.provider.simple/README.md)

## Java and Gradle Setup

Kotlin samples are built with Gradle.

From the sdk root folder, install Java JDK and Gradle in the SDK build environment:

```bash
./scripts/install-java-gradle.sh
```

### Install ctrlX Data Layer build and runtime environment

```bash
./scripts/install-ctrlx-datalayer.sh
```

## Working with Visual Studio Code

* Start Visual Studio Code on your host computer.
* Connect to your SDK build environment.
* Open the folder of the Kotlin sample under `samples-kotlin/`.
* Optionally install Java language tooling:

```bash
./scripts/install-vcode-extensions-java.sh
```

* Install the extension "Kotlin" in SSH.

## Build with Gradle

From the root directory of the selected Kotlin sample run:

```bash
./gradlew clean build
```

## Build a Snap

See [Samples Overview](../samples.md)

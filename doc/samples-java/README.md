# Samples Java

## Basic Samples

* [ctrlX Data Layer Client](./datalayer.client.simple/README.md)
* [ctrlX Data Layer Client Subscription](./datalayer.client.sub/README.md)
* [ctrlX Data Layer Client Bulk](./datalayer.client.bulk/README.md)
* [ctrlX Data Layer Provider](./datalayer.provider.simple/README.md)
* [ctrlX Data Layer Provider Subscription](./datalayer.provider.sub/README.md)

## Prerequisites

### Java & Gradle

Java JDK LTS is used for development. [Gradle Build Tool](https://gradle.org/) is used as a build tool for the Java projects. The Gradle settings are based on Kotlin DSL.

Java JDK and Gradle are installed using the following script (see script for versions).

```bash
    cd ctrlx-automation-sdk/scripts
    ./install-java-gradle.sh
```

Gradle Builds

```bash
    ./gradlew clean build
```

### Visual Studio Code

#### Java Extensions

Java Extensions for Visual Studio Code can be loaded using this script or see [Java in Visual Studio Code](https://code.visualstudio.com/docs/languages/java).

This script must be executed in a Visual Stuido Code terminal/console.

```bash
    ./install-vcode-extensions-java.sh
```

Settings:

"\<ctrl>\<shift>+p" "Java: Clean Java Language Server Workspace"

### Install ctrlX Data Layer build and runtime environment

```bash
    ctrlx-automation-sdk/scripts/install-ctrlx-datalayer.sh
```

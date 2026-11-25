<!-- markdownlint-disable MD033 -->
# Introduction

With the help of this guide, you will be able to build your first snap with one of the example projects of the ctrlX AUTOMATION SDK in just a few steps.

## Prerequisites

* Install ctrlX WORKS with the function **App Build Environment** on your Windows host computer.

* If your Windows host computer is behind a proxy server, install and run [Px Proxy](px.md).

## Building a Snap

Building a snap from an SDK sample project can be done in a few steps:

* **From ctrlX WORKS, create and start a new App Build Environment (QEMU VM) instance.**
    Wait until the VM shuts down automatically after the initialization phase. Then restart your VM.
    For more information, see [ctrlX WORKS Feature App Build Environment](setup_qemu_ctrlx_works.md).

* **When ctrlX WORKS enables the SSH link, click this link to start an SSH session.**
    At the password prompt, enter `boschrexroth`.

* **Install the latest ctrlX AUTOMATION SDK from GitHub**
    See the installation instructions here: [Installation of the ctrlX AUTOMATION SDK](https://github.com/boschrexroth/ctrlx-automation-sdk)

* **Change to the project folder ctrlx-automation-sdk/samples-cpp/hello.world**

* **Build one or both snaps**

    To build a snap for a ctrlX CORE<sup>virtual</sup> enter:

    ```bash
    ./build-snap-amd64.sh
    ```

    To build a snap for a ctrlX CORE enter:

    ```bash
    ./build-snap-arm64.sh
    ```

    As a result, a snap file `sdk-cpp-registernode_...._amd64.snap` or `sdk-cpp-registernode_...._arm64.snap` is created.

!!! important
    Please ignore warnings during snap builds such as
    'Unable to determine library dependencies for ...'.
    These warnings occur with cross builds (on amd64 for arm64) because the tool 'ldd' used by snapcraft cannot resolve the dependencies of the arm64 snap on amd64 host systems.

To copy the snap(s) into your Windows file system, start cmd.exe on **your Windows host** and enter this command:

```bash
    scp -P 10022 boschrexroth@localhost:~/ctrlx-automation-sdk/samples-cpp/hello.world/*.snap .
```

* **Log in to your ctrlX CORE and install the corresponding snap**

    Don't forget to enable the option `Allow installation from unknown source` under Apps --> Settings.

* **Select Settings --> ctrlX Data Layer and check the nodes under the path sdk-cpp-registernode/**

    Several nodes created by the snap should appear.

!!! important
    To set up a complete toolset for developing your own applications (snaps), see the following chapters:

## Futher informations

* [Visual Studio Code](vscode.md)
* [SSH Connection](ssh_connection.md)
* [All sample projects](samples.md)

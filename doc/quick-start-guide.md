With the help of this guide you will be able to build your first snap with one of the example projects of the ctrlX AUTOMATION SDK in just a few steps.

## Prerequisites

* Install ctrlX WORKS >= 1.20 with function 'App Build Environment' on your Windows host computer
 
* If your Windows host computer is behind a proxy server, install and run [PX.exe](px.md). 

## Building a Snap

Building a snap from a SDK sample project can be done in a few steps:
  
* __From ctrlX WORKS create and start a new App Build Environment (QEMU VM) instance.__

Wait until the VM is shutting down automatically after the initialization phase. Then restart your VM.
For more information see [ctrlX WORKS Feature App Build Environment](setup_qemu_ctrlx_works.md).

* __When ctrlX WORKS enables the SSH link click this link to start a SSH session.__

On password prompt enter `boschrexroth`

* __Install the latest ctrlX AUTOMATION SDK from github__

Call this script:

    ~/scripts/install-sdk.sh

The script downloads ctrlx-automation-sdk.zip from github and unzips it to the folder /home/boschrexroth/ctrlx-automation-sdk/

* __Change to project folder /home/boschrexroth/ctrlx-automation-sdk/samples-cpp/datalayer.register.node__

* __Build one or both snaps__

To build a snap for a ctrlX CORE<sup>virtual</sup> enter:

    ./build-snap-amd64.sh

To build a snap for a ctrlX CORE enter:

    ./build-snap-arm64.sh

As a result a snap file `sdk-cpp-registernode_...._amd64.snap` respectively `sdk-cpp-registernode_...._arm64.snap` will be created.

!!! important
    Please ignore the following warnings during snap builds like
    'Unable to determine library dependencies for ...'. 
    These warnings occur with cross builds (on amd64 for arm64) because the used tool 'ldd' used by snapcraft cannot resolve the dependencies of the arm64 snap on amd64 host systems.

To copy the snap(s) into your Windows file system start cmd.exe on __your Windows host__ and enter this command:

    scp -P 10022 boschrexroth@localhost:~/ctrlx-automation-sdk/samples-cpp/datalayer.register.node/*.snap .

* __Login into your ctrlX CORE and install the according snap__

Don't forget to set option 'Allow installation from unknown source' under Apps --> Settings

* __Select Settings --> ctrlX Data Layer and check the nodes under path sdk-cpp-registernode/__

There should appear several nodes created by the snap.

!!! important
    To setup a complete tool set to develop own applications (snaps) see following chapters:

* [Visual Studio Code](vscode.md)

* [SSH Connection](ssh_connection.md)

* [All sample projects](samples.md)


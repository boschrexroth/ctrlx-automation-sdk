# Go Samples

![Go](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTN4W4k4zseh-AKurUNiz98TiScprGtQNykdQ&usqp=CAU)

## Find out the supported samples

* [Hello Webserver](./hello.webserver/README.md)
* [Load/Save Application Data](./appdata/README.md)
* [ctrlX Data Layer Client](./datalayer.client/README.md)
* [ctrlX Data Layer Provider](./datalayer.provider/README.md)
* [Usage of TPM2](./tpm2.srk/README.md)

## Prerequisites for the Developing Environment

General prerequisites are described in the chapter [Setup Overview](../doc/setup_overview.md).

snapcraft has to be installed, if not call

    ~/install-snapcraft.sh

The ctrlx-datalayer debian package has to be installed, if not install it - check versions.

  `wget https://github.com/boschrexroth/ctrlx-automation-sdk/releases/download/1.12.1/ctrlx-datalayer-1.7.5.deb`
  `sudo dpkg -i ctrlx-datalayer-1.7.5.deb`

## Build a Snap

A snap is build by snapcraft with this command - here for amd64 architecture:

    snapcraft --destructive-mode --target-arch=amd64 --enable-experimental-target-arch

To simplify calling snapcraft, you can start one of these scripts from the command line:

* __build-snap-amd64.sh__: Builds a snap for a ctrlX CORE<sup>virtual</sup>
* __build-snap-aarch64.sh__: Builds a snap for a ctrlX CORE M3/4
* build-snaps-remote.sh: Example to build snaps both for ctrlX CORE<sup>virtual</sup> and ctrlX CORE M3/4 remotely on [The Launchpad build farm](https://launchpad.net/builders).

The scripts can also be started by the Visual Studio Code UI. Therefor select main menu item Terminal - Run Build Task...

When the build process is finished right click the snap file in the Visual Studio Code explorer window and select 'Download' to store it to your host. From there you can install it on your ctrlX CORE.
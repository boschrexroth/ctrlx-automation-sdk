<!-- markdownlint-disable MD033 -->
# README Go appdata

## Introduction

This sample shows how to load and save data according to the ctrlx storage concept.

## Debug and Run

Launch the app with the debugger attached by pressing **F5**.

## Build and Install

1. Launch Visual Studio Code.
2. Click on the **Remote Explorer** from the menu.
3. Connect via SSH to your Ubuntu development environment.
4. In the menu item File - Open Folder select the folder samples-go/**appdata**.

## Build a Snap

Select main menu item Termninal - Run Build Task select the architecture of your ctrlX CORE:
    amd64 for ctrlX CORE<sup>virtual</sup>
    arm64 for ctrlX CORE

A snap file will be build.

## Install the Snap

Right click the snap file in the File explorer window and select download. The file will be downloaded into a destination directory on your host computer.

* Login into your ctrlX, from the home page click **Install an app**.
* Switch to Service Mode
* Select **Install from file** and select the snap file on your host computer.
* When installation is finished switch to Operation Mode.

## Test the Snap

* From the ctrlX home page Cclick **Diagnostics -> Logbook**
* Click **Settings** on the right top corner
* Enable **Show system messages**
* Navigate to **Filter -> Units** and check your **snap.sdk-go-appdata.app.service**
* Now you should the see the app output diagnostics.
* Press **Refresh** to update.

* If you have root permissions, you can also watch the output on any ssh console with the command:

```bash
   sudo snap logs -f sdk-go-appdata.app
```

## Provide FOSS Info

If your programcode contains additional open source software, the FOSS Info (Free and Open Source Software) must be filled in order to publish your App to the ctrlX Store.

1. Navigate to the folder configs/package-assets in the root path of your snap project
2. Open the file app.'snap-name'.fossinfo.json
3. Add license information of the used open source software with the following json-structure:

```json
   {
        "name": "",
        "version": "",
        "homepage": "",
        "usage": "",
        "integrationMechanism": "",
        "copyright": {
            "text": ""
        },
        "licenses": [
            {
                "name": "",
                "spdx": "",
                "text": ""
            }
        ]
    }
```

## Install the App

Login into ctrlX and install the App (Apps).

## Gratulations - We're finished - Let's start coding

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/)

### SDK Forum

Please visit the [SDK Forum](https://developer.community.boschrexroth.com/t5/ctrlX-AUTOMATION/ct-p/dcdev_community-bunit-dcae/)

### Issues

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

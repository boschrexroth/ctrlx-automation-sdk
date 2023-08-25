# ctrlX Data Layer Client Browse

## Introduction

The app demonstrates how the ctrlX Data Layer browse functionality works.

## Function Description

The ctrlX Data Layer tree is browsed recursively and the data of the nodes are listed to the console output.

## Implementation Description


Module __main.py__ creates a ctrlX Data Layer client connection and calls the function browse_tree(). This function tries to read and list the value of the (root) node. Then it scans recursively through all sub nodes. When all nodes are listed the app ends.

The app is restarted automatically ny snapd because of its restart option in snap/snapcraft.yaml.

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

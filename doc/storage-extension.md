# Introduction

Storage Extension allows you to add a secondary storage device to your ctrlX using external media, such as a μSD card or USB drive. To ensure optimal system performance and device longevity, we recommend configuring your applications to run and store their data on this external media. This configuration must be done manually for each app to enable storage extension on the external media.

The device variants ctrlX CORE X3plus, X5 and X7 can be ordered with an 80 GB SSD memory extension module. This extension module is listed under its fixed system name e.g. "nvme1n1p1" for the X7, and "nvme0n1p1" for the X3plus and X5.

>!!! warning
> External media mounted as storage extension is fully encrypted with dmcrypt and bound to this device. Removing the external medium will require a complete reformat before it can be used again. This reformatting process will permanently delete all data on the medium.

If a data transfer is required, please see documentation of [Data exchange](data-exchange.md).

## App Configuration

To enable your application to utilize the Storage Extension, you must perform several configuration steps before building your app.

**App Isolation:**
Each app is allocated its own exclusive, isolated area on the storage extension partition: `$SNAP_COMMON/storage-extension/<app-name>`.
No other application can access this path.

## Step 1: Update snapcraft.yaml

You must declare the mount-control plug in your snapcraft.yaml to allow your app to bind to the system-provided mount point.

```yaml title="snapcraft.yaml"
name: storage-extension-sample
title: Storage extension sample app
base: core20
version: 0.0.1
summary: Storage extension sample
description: |
  <!--
  author: Company Name
  url: Company URL
  -->
  Short description of the app.
grade: stable
confinement: strict

parts:
    <...>

plugs:
    storage-extension-mnt:
        interface: mount-control
        mount:
            - what: /writable/system-data/var/run/mnt/storage-extension/${SNAPCRAFT_PROJECT_NAME}
              where: $SNAP_COMMON/storage-extension/${SNAPCRAFT_PROJECT_NAME}
              options:
                  - rw
                  - bind

apps:
    sample:
        command: bin/sample
        plugs:
            - storage-extension-mnt
            - mount-observe
```

- ${SNAPCRAFT_PROJECT_NAME}: A native Snapcraft variable automatically replaced with your project name (e.g., storage-extension-sample). No manual modification is required in this block.

- mount-observe: This system plug is mandatory. If either mount-control (under your custom name) or mount-observe is missing, your application will fail to run correctly.

## Step 2: Add a Remove Hook

A remove hook is required so the system can cleanly unmount your application's storage area if the app is uninstalled. Without this, the system will prevent the application from being removed.

Create an executable shell script named remove and place it in your project's snap/hooks/ directory:

```bash title="snap/hooks/remove"
     snapctl umount $SNAP_COMMON/storage-extension/$SNAP_NAME
```

Make sure the file is marked as executable in your build environment:

```bash
    chmod +x remove
```

## Step 3: Update the App Start Script

Your application must mount the Storage Extension folder inside its virtual environment before executing the main application binary.

```bash title="script.sh"

#!/usr/bin/env bash

# check if storage extension is ready to use (prepared by Device Admin)
mount | grep /var/snap/rexroth-deviceadmin/common/storage-extension
if [ $? -ne 0 ]; then
    # No storage extension detected - abort script
    echo No storage extension found
    exit 1
fi

# storage extension detected, mount for your app
snapctl mount -o rw,bind /writable/system-data/var/run/mnt/storage-extension/$SNAP_NAME $SNAP_COMMON/storage-extension/$SNAP_NAME

$SNAP/your-app.sh
```

## Step 4: Test Unsigned Apps (Optional for Development)

By default, the storage extension interface is on a security denylist. Consequently, only cryptographically signed production apps are permitted to access it.

If you want to test your own unsigned apps during the active development phase, you must temporarily clear this denylist on your ctrlX CORE. This requires root user privileges.

Establish an SSH connection to your ctrlX CORE and execute the following commands:

```bash
    sudo -s
    mkdir -p /etc/writable/integrity
    echo [] >/etc/writable/integrity/denylist.json
    snap restart rexroth-deviceadmin.web
```

## Activating the Storage Extension

Once your app is configured for the storage extension, follow these steps on the ctrlX CORE to activate the storage medium.

1. Insert the external media (μSD card or USB device) into the control.
2. In the ctrlX CORE Web UI, navigate to the "Settings" -> "Storage" and mount the media using the option "To extend system data".
3. Click Continue. The storage extension is prepared and integrated. Subsequently, the control automatically reboots.
4. After the reboot, the extended storage is active and available to your configured apps under the following path:
$SNAP_COMMON/storage-extension/**app-name**

!!! attention
To safely disconnect the media, use the Remove Safely button. This process will conclude with a system restart. Removing the external medium will require a complete reformat before it can be used again. This reformatting process will permanently delete all data on the medium.

**Remarks:**

- Ubuntu Snapshots: Storage Extension cannot be used in combination with the Ubuntu Snapshot Mechanism.
- System State: If necessary, you must switch the control to the Service State to mount or unmount Storage Extension partitions.
- Hardware Limit: Only one partition can be actively used as a Storage Extension at any given time.

# Introduction

Storage extension is a feature to extend the internal storage by an external media like μSD card or USB device. Storage extension was introduced with Device Admin 1.18.

This storage is encrypted and bond to one ctrlX CORE, so it cannot be used to transfer data from one ctrlX CORE to another one. If a data transfer is required, please see documentation of [Data exchange](data-exchange.md).

!!! attention
    >
    - All data on the storage extension partition will be erased! Be sure you back up your data before.
    - The partition will be formatted with ext4 and encrypted with dmcrypt.
    - Each app has an own, exclusive area on the storage extension partition, no other app can access this location.

# Requirements
To use the extended storage in your app, some changes are needed in your app. The minimum requirements are shown in the samples.

The storage extension mount point must be added as a mount-control plug in the [snapcraft.yaml](https://snapcraft.io/docs/snapcraft-yaml-reference). Additionally your app needs the mount-observe plug and a remove hook.

And as the last step some modifications of the start script of your app are required.

The storage extension parent for your app is mounted by the Device Admin. The location is `/writable/system-data/var/run/mnt/storage-extension/${SNAPCRAFT_PROJECT_NAME}`.

# Development

!!! attention

   **Root user required**

    The storage extension plug is on the denylist.
    Therefore only signed apps can use storage extension.
    To avoid trouble during the development, a root user for the ctrlX is required
    and the denylist has to be empty.


   **Empty the denylist**

    sudo -s
    mkdir -p /etc/writable/integrity
    echo [] >/etc/writable/integrity/denylist.json
    snap restart rexroth-deviceadminw.web


After clearing the denylist it's possible to test storage extension without signed apps.

# Configuration

Some modifications in your app are required to use storage extension.

# Snapcraft.yaml

The storage extension plug is required in **[snapcraft.yaml](https://snapcraft.io/docs/snapcraft-yaml-reference)**. The type of the plug is **mount-control**, the necessary parameters are shown in the sample:

!!! example
    ```yaml title="snapcraft.yaml"
    name: storage-extension-sample
    title: Storage extension sample app
    base: core20
    version: 0.0.1
    summary: Storage extension sample
    description: |
        Storage extension sample
    grade: stable
    confinement: strict

    parts:
        <...>
        backup:
            plugin: nil
            source: backup
            overrride-build: |
            snapcraftctl build
            mkdir -p $SNAPCRAFT_PART_INSTALL/meta
            cp $SNAPCRAFT_PART_SRC/snapshots.yaml $SNAPCRAFT_PART_INSTALL/meta

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

Remark: the variable `${SNAPCRAFT_PROJECT_NAME}` is a known variable by snapcraft and will be replaced with the project name (here: **storage-extension-sample**). So no modifications in this block is required! The variable `$SNAP_COMMON` is known in the snap context.

Your app needs the plugs **mount-observe** and the defined storage extension plug (here: **storage-extension-mnt**). If one of the plugs are missing, your app won't run like expected.

# Remove hook
There has to be a remove hook in your app. Otherwise, it's not possible to remove your app if a storage extension is available.

To add the remove hook, the file `remove` has to be placed in the `snap/hooks` folder.

Don't forget to mark the file as executable:

    chmod +x remove

!!! remove
    ```title=""
     snapctl umount $SNAP_COMMON/storage-extension/$SNAP_NAME
    ```



Remark: The variables `$SNAP_COMMON` and `$SNAP_NAME` are known in the snap context.

# App start script
Before your app can start and use the storage extension, the storage extension mount must be done. Therefore, your start script has to mount the storage extension.

!!! example
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

# Exclude from backup support
All your data from the storage extension are added to the backup by default. You typically use the storage extension when you have increased memory requirements in your app. Therefore it is necessary to exclude the storage extension from the backup.

This will be done by adding a "part" in your snapcraft.yaml and add a snapshots.yaml to the project.

!!! example
    ```yaml title="snapshots.yaml"
    exclude:
        # Exclude storage-extension partition from backup
        - $SNAP_COMMON/storage-extension/*
    ```


# Sample app

This sample app ( [storage-extension-sample](./samples-snap/storage-extension/README.md) ) is the minimum set to use storage extension.
Feel free to add the content to your app.

# Use storage extension

To use storage extension, mount the media with the option "To extend system data".

!!! attention
    The partition will be formatted and encrypted. All data on the partition will be lost!

After clicking "continue" the storage extension will be added to the system and the control will be rebooted.

After the next reboot the storage extension will be available for the system.


Each app has its own area, the path is `$SNAP_COMMON/storage-extension/<app-name>`.

!!! Remarks
    - By pressing the remove safely button, you can remove the storage extension (this will be finished with a restart of the control).
    - !!! warning
        Be careful: You cannot reuse the partition without formatting and encrypting it again. So all data will be lost!

    - **Only one** partition can be used as storage-extension at one time
    - Data on storage-extension is **not** part of backup/restore
    - If necessary, you have to switch to service state to mount or unmount storage-extension partitions

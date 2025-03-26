# Introduction

You can use external storage (μSD card / USB device) for data exchange purpose (removable media). Can be used from Device Admin 1.16.

If a storage extension is required, please see documentation of [Storage extension](storage-extension.md).

!!! attention
    - **The data on the external storage (μSD card / USB device) is not encrypted. Others can unplug the device and can read the data on another devices.**
    - **The storage with ext4 and fat32 format are supported (formatting can be done in ctrlX UI: Settings->Storage).**
    - **There is no exclusive access to the medium for a singe application. Your data can be overwritten by other applications.**
    - **The data written to the storage cannot be encrypted.**

## Requirements

A storage is inserted, formatted and mounted:

If the storage is not formatted as fat32 or ext4, please format it first.

To format or mount a storage, use the according buttons in the details window of the Storage. The details window appears, when you select the according tile of the storage:

After the storage is mounted, the path is displayed on the details page of the storage:

Alternatively, the mount point is determinable in the ctrlX Data Layer (in Settings menu) under **system/resources/storage**

## Configuration steps

To access the storage from your app (e.g. storage-test), use the **removable-media** interface as plug in the **[snapcraft.yaml](https://snapcraft.io/docs/snapcraft-yaml-reference)** of your application:

!!! example
    ```yaml title="snapcraft.yaml"
    name: storage-test # you probably want to 'snapcraft register name'
    base: core20 # the base snap is the execution environment for this snap
    version: '0.1' # just for humans, typically 'i.2+git' or '1.3.2'
    summary: Single-line elevator pitch for your amazing snap # 79 char long summary
    description: |
        This is my-snap's description.
        You have a paragraph or two to tell the most important story about your snap.
        Keep it under 100 words though,
        we live in tweet space and your description wants to look good in the snap store.

    grade: devel # must be 'stable' to release into candidate/stable channeis
    confinement: strict # use 'strict' once you have the right plugs and slots
    architectures:
        - all
    parts:
        storage:
	        plugin: dump
		    source: storage
    apps:
	    storage:
		    command: bin/storage
		    daemon: simple
		    plugs:
			    - removable-media
    ```

## Usage

- Install your app in ctrlX UI via Settings → Apps → Install from file.

- After installation, you have permissions to read and write data in the mounted directory. The path is displayed in Storage details page `/media/<storage>`.

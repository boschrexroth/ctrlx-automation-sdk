# How to Configure the maximal Number of Open File Handles

## Introduction

The maximum number of open file/socket handles is by default limited to 1024. This is not enough for some apps (e.g. connectivity apps or databases).

The actual settings can be shown with:

    ulimit -a

Maximum file handles:

    cat /proc/sys/fs/file-max

Allocated, allocated but unused, maximum number of file handles:

    cat /proc/sys/fs/file-nr

## Solution

For a lot of apps the number of 1024 maximal opened file handles is sufficient.

If a larger number of opened file handles is required this can be done with the command `ulimit`, for example:

    ulimit -n 16384

Your app should run this command in the startup phase of your app.

Therefor we recommend to:

- Create a script in a sub folder e.g. ulimit-snap/ of your project.
- And to add into your snapcraft.yaml file a daemon entry to call this script at startup.

??? Script

    ```yaml title="increase-ulimit.sh"
    #!/bin/sh
    ulimit -n 16384
    ```

??? Daemon

    ```yaml title="snapcraft.yaml"
    parts:
    ulimit-script:
        plugin: dump
        source: ./ulimit-snap

    apps:
    ulimit:
        daemon: simple
        command: increase-ulimit.sh
    ```

## Complete Example Snap

In this SDK an complete example project creating a demo snap is stored under __samples-snap/ulimit__.

The sample project folder also contains a README.md file with further informations.

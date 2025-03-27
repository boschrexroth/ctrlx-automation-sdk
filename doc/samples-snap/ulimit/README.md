# Example Snap to increase the maximal number of opened files

## Introduction

## Solution

To increase the maximal number of opened files the command

    ulimit -n xxx

can be used.

This command can be called within a snap defining a daemon which starts a script with this command.

## Files

### build-snap.sh

Calling this script will build a snap for amd64 and arm64 architecture.

### ulimit-script/increase-ulimit.sh

This script calls the ulimit command.

### snap/snapcraft.yaml

This section causes the script to be packed into the snap archive:

```yaml
parts:
  ulimit-script:
    plugin: dump
    source: ./ulimit-script
```

Here the daemon is defined to call the script when the snap is started:

```yaml
apps:
  ulimit:
    daemon: simple
    command: increase-ulimit.sh
```

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

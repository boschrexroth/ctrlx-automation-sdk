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

```
parts:
  ulimit-script:
    plugin: dump
    source: ./ulimit-script
```

Here the daemon is defined to call the script when the snap is started:

```
apps:
  ulimit:
    daemon: simple
    command: increase-ulimit.sh

```


## License

MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

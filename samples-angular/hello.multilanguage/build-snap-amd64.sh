#!/usr/bin/env bash
snapcraft clean --destructive-mode
snapcraft --destructive-mode --target-arch=amd64 --enable-experimental-target-arch
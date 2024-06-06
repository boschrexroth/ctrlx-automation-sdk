#!/usr/bin/env bash
set -e
TARGET_ARCH=amd64

# Clean all snap part assets
snapcraft clean --destructive-mode

# Build snap without separate container
snapcraft --destructive-mode --enable-experimental-target-arch --target-arch ${TARGET_ARCH}
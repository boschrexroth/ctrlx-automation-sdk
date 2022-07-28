#!/usr/bin/env bash
TARGET_ARCH=arm64

# Clean all snap part assets
snapcraft clean --destructive-mode

# Build snap without separate container
snapcraft --destructive-mode --enable-experimental-target-arch --target-arch ${TARGET_ARCH}
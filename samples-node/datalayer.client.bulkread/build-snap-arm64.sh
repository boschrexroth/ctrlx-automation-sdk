#!/usr/bin/env bash
snapcraft clean --destructive-mode
snapcraft --destructive-mode --target-arch=arm64 --enable-experimental-target-arch
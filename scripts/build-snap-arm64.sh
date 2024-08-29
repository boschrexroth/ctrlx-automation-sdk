#!/usr/bin/env bash
set -e
sudo snapcraft clean
sudo snapcraft --build-for=arm64 --verbosity=verbose
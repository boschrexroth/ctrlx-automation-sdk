#!/usr/bin/env bash
set -e
snapcraft clean
snapcraft pack --build-for=arm64 --verbosity=verbose
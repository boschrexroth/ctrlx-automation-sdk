#!/usr/bin/env bash
set -e
snapcraft clean
snapcraft pack --build-for=amd64 --verbosity=verbose
#!/usr/bin/env bash
set -e
sudo snapcraft clean
sudo snapcraft --build-for=amd64 --verbosity=verbose
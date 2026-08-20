#!/usr/bin/env bash
set -e

create_crate_dir() {
    CURRENT_DIR=$(pwd)
    ONE_LEVEL_UP=$(dirname "$CURRENT_DIR")
    TWO_LEVELS_UP=$(dirname "$ONE_LEVEL_UP")
    CRATE_DIR=${TWO_LEVELS_UP}/rust/crate
    ln -s "${CRATE_DIR}" "${CURRENT_DIR}"
}

if [ ! -d "crate" ]; then
    create_crate_dir
fi

# Build snap
../../scripts/build-snap-amd64.sh

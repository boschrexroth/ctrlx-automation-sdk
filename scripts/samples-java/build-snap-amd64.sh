#!/usr/bin/env bash
set -e

create_java_dir() {
    CURRENT_DIR=$(pwd)
    ONE_LEVEL_UP=$(dirname "$CURRENT_DIR")
    TWO_LEVELS_UP=$(dirname "$ONE_LEVEL_UP")
    JAVA_DIR=${TWO_LEVELS_UP}/java
    ln -s "${JAVA_DIR}" "${CURRENT_DIR}"

    cp ../../oss.gradle/*.zip gradle/wrapper/
}

if [ ! -d "java" ]; then
    create_java_dir
fi

# Build snap
../../scripts/build-snap-amd64.sh

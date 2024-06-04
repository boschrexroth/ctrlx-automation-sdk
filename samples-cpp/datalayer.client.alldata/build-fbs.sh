#!/bin/bash
set -e

echo "-- Generating flatbuffers files --"

PROJECT_DIR=$(realpath "$(dirname "$0")")
SDK_ROOT_DIR=$(realpath "$PROJECT_DIR/../..")
FLATC=$SDK_ROOT_DIR/bin/oss.flatbuffers/ubuntu22-gcc-x64/flatc

echo "Set executable permission for flatc"
chmod +x "$FLATC"

echo "Compile flatbuffers cpp classes"
$FLATC --schema --gen-object-api --cpp -o "$PROJECT_DIR/" "$PROJECT_DIR/sampleSchema.fbs"
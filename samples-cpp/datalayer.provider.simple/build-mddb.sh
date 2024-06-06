#!/bin/bash
set -e

echo "-- Generating metadata base file --"

PROJECT_DIR=$(realpath "$(dirname "$0")")
SDK_ROOT=$(realpath "$PROJECT_DIR/../..")
MDDB_COMPILER=$SDK_ROOT/bin/comm.datalayer/ubuntu22-gcc-x64/mddb_compiler

echo "Set executable permission for mddb_compiler"
chmod +x "$MDDB_COMPILER"

echo "Compile csv to mddb"
$MDDB_COMPILER -in "$PROJECT_DIR/metadata.csv" -out "$PROJECT_DIR/compiled/metadata.mddb"

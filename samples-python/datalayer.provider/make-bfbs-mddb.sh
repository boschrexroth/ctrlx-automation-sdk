#!/usr/bin/env bash

# Current directory must by project root directory


# Create compiled/sampleSchema.bfbs

UNAME=$(uname -a)
UDIR=ubuntu20-gcc-x64
if grep -q "aarch64" <<<${UNAME}
then
  UDIR=ubuntu20-gcc-aarch64
fi

mkdir -p bfbs
../../bin/oss.flatbuffers/${UDIR}/release/flatc -o bfbs/ -b --schema sampleSchema.fbs

../../bin/oss.flatbuffers/${UDIR}/release/flatc --python --gen-object-api --gen-mutable sampleSchema.fbs

# Create compiled/metadata.mddb
mkdir -p mddb
mddb_compiler -in metadata.csv -out ./mddb/metadata.mddb
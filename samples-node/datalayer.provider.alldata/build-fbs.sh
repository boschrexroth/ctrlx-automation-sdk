#!/bin/bash
flatc=$(dirname $0)/../../../public/bin/oss.flatbuffers/ubuntu22-gcc-x64/flatc
rm -frv ./src/bfbs 
mkdir -pv ./src/bfbs
$flatc --gen-object-api --gen-all --gen-compare --no-warnings --ts-no-import-ext --schema --ts -o ./src ./fbs/sampleSchema.fbs
$flatc --schema --binary --bfbs-comments --bfbs-builtins --no-warnings -o ./src/bfbs ./fbs/sampleSchema.fbs 

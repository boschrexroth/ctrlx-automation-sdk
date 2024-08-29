#!/bin/bash
flatc=$(dirname $0)/../../../public/bin/oss.flatbuffers/ubuntu22-gcc-x64/flatc
rm -frv ./bfbs 
mkdir -pv ./bfbs 
$flatc --schema --gen-object-api --gen-compare --no-warnings --csharp ./fbs/*
$flatc --schema --binary --bfbs-comments --bfbs-builtins --no-warnings -o ./bfbs ./fbs/*

#!/bin/bash

flatc_linux=$(dirname $0)/../../bin/oss.flatbuffers/ubuntu22-gcc-x64/release/flatc
flatc_windows=$(dirname $0)/../../bin/oss.flatbuffers/win-msvc-x64/release/flatc

case "$OSTYPE" in
  linux*)   flatc=$flatc_linux ;;
  msys*)    flatc=$flatc_windows ;;
  cygwin*)  flatc=$flatc_windows ;;
esac

if [ -z "$flatc" ]; then
  echo "flatbuffers compiler $flatc could not be found!"
  exit 1
fi

rm -frv ./bfbs 
mkdir -pv ./bfbs 
$flatc --schema --gen-object-api --gen-compare --suppress-validate-version --no-warnings --csharp ./fbs/*
$flatc --schema --binary --bfbs-comments --bfbs-builtins --no-warnings -o ./bfbs ./fbs/*
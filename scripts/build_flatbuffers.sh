#!/bin/bash
flatc=$(dirname $0)/../bin/oss.flatbuffers/linux-gcc-x64/release/flatc
# flatc=$0/../bin/oss.flatbuffers/win-msvc-x64/release/flatc.exe
lang=$1
fbs=$2
out=$(dirname $2)
echo flatc: $flatc
echo fbs:   $fbs
echo out:   $out
echo lang:  $lang
$flatc -b --schema --$lang -o $out $fbs
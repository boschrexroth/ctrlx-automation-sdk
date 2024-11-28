#!/usr/bin/env bash
set -e
rm -rf generated/
rm -rf build/
mkdir build/
pushd build
cmake -DBUILD_KIT='GCC x86_64-linux-gnu' -DCMAKE_BUILD_TYPE=Debug ..
make install
popd
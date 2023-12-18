#!/usr/bin/env bash
set -e
rm -rf bfbs/
rm -rf generated/
rm -rf build/
mkdir build/
pushd build
cmake \
    -DBUILD_KIT='GCC aarch64-linux-gnu' \
    -DCMAKE_BUILD_TYPE=RelWithDebugInfo \
    -DBUILD_SNAP=ON \
    ..
make install
popd
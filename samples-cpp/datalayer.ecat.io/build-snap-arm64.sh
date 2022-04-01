#!/usr/bin/env bash
rm -rf bfbs/
rm -rf generated/
rm -rf build/
mkdir build/
pushd build
cmake -DCMAKE_BUILD_KIT='GCC aarch64-linux-gnu' -DBUILD_SNAP=ON ..
make install
make clean
popd
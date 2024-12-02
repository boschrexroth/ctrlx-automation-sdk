#!/usr/bin/env bash
set -e
rm -rf bfbs/
rm -rf generated/
rm -rf build/
mkdir build/

pushd build
cmake -DBUILD_KIT='GCC aarch64-linux-gnu' -DBUILD_SNAP=ON ..
make install
make clean
popd

sudo snapcraft clean
sudo snapcraft --build-for=arm64 --verbosity=verbose
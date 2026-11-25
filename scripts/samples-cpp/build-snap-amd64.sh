#!/usr/bin/env bash
set -e
#If SNAPCRAFT_BUILD_ENVIRONMENT is empty or unset, the fallback host is set. 
#If the variable was already defined (e.g., to lxd), the existing value is preserved.
export SNAPCRAFT_BUILD_ENVIRONMENT="${SNAPCRAFT_BUILD_ENVIRONMENT:-host}"

rm -rf bfbs/
rm -rf generated/
rm -rf build/
mkdir build/
pushd build
cmake -DBUILD_KIT='GCC x86_64-linux-gnu' -DBUILD_SNAP=ON ..
make install
make clean
popd
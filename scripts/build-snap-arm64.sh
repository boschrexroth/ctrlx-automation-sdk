#!/usr/bin/env bash
set -e
#If SNAPCRAFT_BUILD_ENVIRONMENT is empty or unset, the fallback host is set. 
#If the variable was already defined (e.g., to lxd), the existing value is preserved.
export SNAPCRAFT_BUILD_ENVIRONMENT="${SNAPCRAFT_BUILD_ENVIRONMENT:-host}"

snapcraft clean
snapcraft pack --build-for=arm64 --verbosity=verbose
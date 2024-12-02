#!/usr/bin/env bash

source ./build-9-user-data-img.sh amd64 proxy
source ./build-9-user-data-img.sh amd64 noproxy
mv user-data-amd64-* builder/

source ./build-9-user-data-img.sh aarch64 proxy
source ./build-9-user-data-img.sh aarch64 noproxy

#!/usr/bin/env bash

source ./build.sh amd64 proxy
source ./build.sh amd64 noproxy
source ./build.sh aarch64 proxy
source ./build.sh aarch64 noproxy

#!/usr/bin/env bash

source ./build.sh amd64 proxy
source ./build.sh amd64 noproxy
source ./build.sh arm64 proxy
source ./build.sh arm64 noproxy

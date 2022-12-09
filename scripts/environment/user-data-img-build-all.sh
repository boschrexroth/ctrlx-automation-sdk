#!/usr/bin/env bash

source ./user-data-img-build.sh amd64 proxy
source ./user-data-img-build.sh amd64 noproxy
source ./user-data-img-build.sh aarch64 proxy
source ./user-data-img-build.sh aarch64 noproxy

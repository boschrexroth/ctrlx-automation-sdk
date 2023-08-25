#!/usr/bin/env bash

# AMD64 only: If not installed the required arm64 libraries are installed.

ARCH=$(uname -m)
if grep -q "aarch64" <<<${ARCH}
then
  exit 0
fi

sudo apt-get -y update 

sudo apt-get -y install libsystemd-dev:arm64
sudo apt-get -y install libssl-dev:arm64
sudo apt-get -y install libzmq3-dev:arm64



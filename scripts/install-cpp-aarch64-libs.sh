#!/usr/bin/env bash

# AMD64 only: If not installed the required arm64 libraries are installed.

ARCH=$(uname -m)
if grep -q "aarch64" <<<${ARCH}
then
  return
fi

sudo apt-get -y update 

sudo apt-get -y install libsystemd-dev:arm64
sudo apt-get -y install libssl-dev:arm64
sudo apt-get -y install libzmq3-dev:arm64
sudo apt-get -y install libzip-dev:arm64
sudo apt-get -y install uuid-dev:arm64
sudo apt-get -y install libssl-dev:arm64
sudo apt-get -y install libcurl4-openssl-dev:arm64
sudo apt-get -y install libjansson-dev:arm64
sudo apt-get -y install libffi-dev:arm64
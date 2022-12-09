#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing required libraries
echo "============================================"
read -t 2

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y install libsystemd-dev:arm64
sudo apt-get -y install libsystemd-dev:amd64
sudo apt-get -y install libssl-dev:amd64
sudo apt-get -y install libssl-dev:arm64
sudo apt-get -y install libzmq3-dev:amd64
sudo apt-get -y install libzmq3-dev:arm64

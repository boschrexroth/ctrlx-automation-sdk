#!/usr/bin/env bash

#
# This script installs debian packages required to build apps
# with the ctrlX AUTOMATION SDK.
#
# https://wiki.ubuntu.com/MultiarchSpec

ARCH=$(uname -m)
OS_NAME=$(lsb_release -si)

if [[ "$ARCH" == "x86_64" ]] && [[ "$OS_NAME" == "Ubuntu" ]] 
then
  sudo dpkg --add-architecture arm64
  DIST="$(lsb_release -sc)"
  sudo echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports/ ${DIST} main restricted universe multiverse" | sudo tee /etc/apt/sources.list.d/multiarch-libs.list
  sudo echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports/ ${DIST}-backports main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
  sudo echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports/ ${DIST}-security main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
  sudo echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports/ ${DIST}-updates main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
  # Qualify architecture
  sudo sed -i 's/deb http:/deb [arch=amd64] http:/g' /etc/apt/sources.list
else
  echo "[Warning] Crosscompiling not enabled ! CPU Architecture != x86_64 detected or the systemen isn't running Ubuntu"
fi

# Environment variable to enable/disable the use of certain CPU capabilities.
# 0x1: Disable all run-time detected optimizations
# see https://gnutls.org/manual/html_node/Debugging-and-auditing.html
# Fixes issue: "Method https has died unexpectedly! Sub-process https received signal 4"
# see https://askubuntu.com/questions/1420966/method-https-has-died-unexpectedly-sub-process-https-received-signal-4-after
export GNUTLS_CPUID_OVERRIDE=0x1

# Prevent prompt that ask to restart services
export DEBIAN_FRONTEND=noninteractive

sudo -E apt update
sudo -E apt upgrade

# install base packages ...
sudo -E apt install -y \
  zip \
  unzip \
  p7zip-full \
  git \
  apt-transport-https \
  whois \
  net-tools \
  pkg-config \
  jq \
  sshpass \
  dpkg-dev

# install python tools ...
sudo -E apt install -y \
  python3-pip \
  virtualenv

# install amd64 build tools ...
sudo -E apt install -y \
  build-essential \
  gdb \
  cmake

# install arm64 cross build tools ...
sudo -E apt install -y \
  crossbuild-essential-arm64 \
  gdb-multiarch

# install required amd64 packages ...
sudo -E apt install -y \
  libxml2-dev \
  uuid-dev \
  libbz2-1.0 \
  libzmq3-dev \
  libsystemd-dev \
  libssl-dev \

# install required arm64 packages ...
sudo -E apt install -y \
  libxml2-dev:arm64 \
  uuid-dev:arm64 \
  libbz2-1.0:arm64 \
  libzmq3-dev:arm64 \
  libsystemd-dev:arm64 \
  libssl-dev:arm64 \


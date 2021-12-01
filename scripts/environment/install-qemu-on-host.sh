#!/usr/bin/env bash

# Function(s) #################################################################

install_pkg()
{
  if ! dpkg -s $1 >/dev/null 2>&1; then
    sudo apt-get -y install $1
  fi
}

remove_pkg()
{
  if dpkg -s $1 >/dev/null 2>&1; then
    sudo apt-get -y remove $1
  fi
}

# Commands ####################################################################

remove_pkg qemu-system-x86
remove_pkg qemu-system-arm
remove_pkg qemu-efi-aarch64
remove_pkg qemu-efi
remove_pkg qemu-utils
sudo apt-get -y autoremove 


install_pkg ninja-build 
install_pkg libglib2.0-dev 
install_pkg libpixman-1-dev 

git clone https://gitlab.com/qemu-project/qemu.git
cd qemu
git checkout tags/v5.2.0 -b v520 
git submodule init
git submodule update --recursive
./configure
make

sudo make install

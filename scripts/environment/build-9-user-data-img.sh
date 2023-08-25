#!/usr/bin/env bash

# This script ...
# - Installs the cloud-image-utils package if it's not installed
# - Removes optionally proxy usage lines from a cloud-config file
# - Calls cloud-localds to compile a cloud-config file into a user data img file (ubuntu-xx.yy-server-cloudimg-aaa-user-data-ppp.img)
#   were aaa is the CPU architecture 'amd64' or 'aarch64'and ppp 'proxy' or 'noproxy'

# Usage: build-user-data-img.sh aaa ppp info help
#   aaa: CPU Architecture amd64 (default) | aarch64 
#   ppp: Use proxy server: proxy (default) | noproxy
#   info: Print infos during run
#   help: Print help informations and exit


# Set script variables ########################################################
# CPU architecture ------------------------------------------------------------
ARCH=amd64
# Use proxy server ------------------------------------------------------------
PROXY=proxy
# Info print ------------------------------------------------------------------
INFO=""

# if grep -q "xxx" <<<${PARAM} --> IF ${PARAM}.contains("xxx")
for PARAM in "$@"
do
  if grep -q "ar" <<<${PARAM}; then
    ARCH=aarch64
  fi

  if grep -q "no" <<<${PARAM}; then
    PROXY=noproxy
  fi

  if grep -q "in" <<<${PARAM}; then
    INFO=info
  fi

  if grep -q "help" <<<${PARAM}; then
    echo Downloads/builds all files needed to lauch a QEMU VM
    echo "Parameters:"
    echo "   CPU Architecture:  amd64 (default) | aarch64"
    echo "   Use proxy server:  proxy (default) | noproxy"
    echo "   Print info:        info"
    echo "   Help (this info):  help"

    exit 1
  fi

done

# Operating system ------------------------------------------------------------
OS=ubuntu-22.04-server

# Ubuntu cloud image name -----------------------------------------------------
# The Ubunt version name
U=${OS}-cloudimg
UA=${U}-${ARCH}

# Name of the Ubuntu cloud image file to be downloaded
if grep -q "ar" <<<${ARCH}
then
  IMG=${U}-arm64.img
else
  IMG=${U}-amd64.img
fi

# Architecture independent snapshot from cloud image 
QCOW2=${UA}-${PROXY}.qcow2

# cloud-config ----------------------------------------------------------------
CC=cloud-config-amd64-generated
if grep -q "ar" <<<${ARCH}
then
  CC=cloud-config-aarch64-generated
fi

CCG=${CC}-${PROXY}

# User data image created from cloud-config -----------------------------------
UDIMG=user-data-${ARCH}-${PROXY}.img

# Function(s) #################################################################

info()
{
  echo "Using these settings: -----------------------------------"
  echo OS=$OS
  echo ARCH=$ARCH
  echo PROXY=$PROXY
  echo IMG=$IMG
  echo QCOW2=$QCOW2
  echo UDIMG=$UDIMG
  echo CCG=${CCG}

  if grep -q "ar" <<<${ARCH}
  then
    echo QEMU_EFI.fd
  fi
  echo "----------------------------------------------------------"

  read -p "Waiting 5s ..." -t 5
}

install_pkg()
{
  if ! dpkg -s $1 >/dev/null 2>&1; then
    sudo apt-get -y install $1
  fi
}

# Info ########################################################################
if [ ! -z $INFO ]; then
  info
fi


# Create user data image file #################################################
install_pkg cloud-image-utils

if grep -q "no" <<<${PROXY}
then
  # Remove these lines
  grep -v "#proxy-ONLY" $CC >$CCG 
else
  cp $CC $CCG
  python3 replace-text.py $CCG "#proxy-ONLY" " "

fi

# Create user-data-image
cloud-localds -v ${UDIMG} $CCG

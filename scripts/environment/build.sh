#!/usr/bin/env bash

# Parameters
#   CPU Architecture: amd64 (default) | arm64 | aarch64 
#   Use proxy server: proxy (default) | noproxy
#   Info: info
#   Help: help


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
    ARCH=arm64
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
    echo "   CPU Architecture:  amd64 (default) | arm64 | aarch64"
    echo "   Use proxy server:  proxy (default) | noproxy"
    echo "   Print info:        info"
    echo "   Help (this info):  help"

    exit 1
  fi

done

# Operating system ------------------------------------------------------------
OS=ubuntu-20.04-server

# Ubuntu cloud image name -----------------------------------------------------
# The Ubunt version name
U=${OS}-cloudimg
# Combined with architecture e.g. ubuntu-20.04-server-cloudimg-arm64
UA=${U}-${ARCH}

# Name of the Ubuntu cloud image file to be downloaded
# e.g. ubuntu-20.04-server-cloudimg-arm64.img
IMG=${UA}.img

# Architecture independent snapshot from cloud image 
# e.g. ubuntu-20.04-server-cloudimg-snapshot.qcow2 
QCOW2=${UA}-snapshot-${PROXY}.qcow2

# cloud-config ----------------------------------------------------------------
CC=cloud-config-amd64
if grep -q "ar" <<<${ARCH}
then
  CC=cloud-config-aarch64
fi

CCG=${CC}-${PROXY}-generated

# User data image created from cloud-config -----------------------------------
UDIMG=${UA}-user-data-${PROXY}.img

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


# Create user data image files ################################################
install_pkg cloud-image-utils

if grep -q "no" <<<${PROXY}
then
  grep -v "#Remove-if-noproxy" $CC >$CCG 
else
  cp $CC $CCG
fi

# Create user-data-image
cloud-localds -v ${UDIMG} $CCG


# Download QEMU_EFI.fd ####################################################
if grep -q "ar" <<<${ARCH}
then
  if [ ! -f QEMU_EFI.fd ]; then
    wget --no-check-certificate https://releases.linaro.org/components/kernel/uefi-linaro/16.02/release/qemu64/QEMU_EFI.fd
  fi
fi


# Download Ubuntu cloud image file ##########################################
if [ ! -f $IMG ]; then
  wget https://cloud-images.ubuntu.com/releases/focal/release/$IMG
fi


# Create snapshot file ######################################################
if [ ! -f $QCOW2 ]; then
  qemu-img create -f qcow2 -b $IMG -F qcow2 $QCOW2 1T
fi
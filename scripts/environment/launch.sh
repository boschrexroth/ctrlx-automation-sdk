#!/usr/bin/env bash

# $1 Guest System CPU Architecture : amd64 (default), aarch64
# $2 Proxy usage: proxy (default), no-proxy

# Set variables ########################################################
ARCH=amd64
PROXY=proxy

# Scan parameter list
for PARAM in "$@"
do
  if grep -q "ar" <<<${PARAM}; then
    ARCH=aarch64
  fi

  if grep -q "no" <<<${PARAM}; then
    PROXY=noproxy
  fi

done

# Operating system ------------------------------------------------------------
VERSION=22.04

OS=ubuntu-${VERSION}-server

# Ubuntu cloud image name -----------------------------------------------------
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

# User data image created from cloud-config -----------------------------------
UDIMG=user-data-${ARCH}-${PROXY}.img

# aarch64 only: Download QEMU_EFI.fd ##########################################
if grep -q "ar" <<<${ARCH}
then
  if [ ! -f QEMU_EFI.fd ]; then
    wget --no-check-certificate -q --show-progress https://releases.linaro.org/components/kernel/uefi-linaro/16.02/release/qemu64/QEMU_EFI.fd
  fi
fi

# Download Ubuntu cloud image file ##########################################
if [ ! -f $IMG ]; then
  rm -rf img 2>/dev/null
  mkdir img
  wget -q --show-progress  https://cloud-images.ubuntu.com/releases/${VERSION}/release/$IMG -O img/$IMG
  mv -f img/$IMG .
fi

# Create snapshot file ######################################################
if [ ! -f $QCOW2 ]; then
  qemu-img create -f qcow2 -b $IMG -F qcow2 $QCOW2 32G
fi

# EMULATED aarch64 Guest System:  -cpu cortex-a57
if grep -q "ar" <<<${ARCH}
then
  qemu-system-aarch64 \
  -M virt \
  -cpu cortex-a57 \
  -accel kvm \
  -accel xen \
  -accel tcg \
  -smp 4 \
  -m 4G \
  -bios QEMU_EFI.fd \
  -netdev user,id=eth0,hostfwd=tcp::10022-:22 \
  -device virtio-net-pci,netdev=eth0,mac=DE-AD-BE-00-00-01 \
  -drive file="$QCOW2",format=qcow2 \
  -drive file="$UDIMG",format=raw \
  -display none \
  -nographic  \
  ;

  exit $?
fi

# amd64 Guest System
if grep -q "am" <<<${ARCH}
then
  qemu-system-x86_64 \
  -machine q35,accel=kvm:xen:tcg \
  -smp 4 \
  -m 4G \
  -netdev user,id=eth0,hostfwd=tcp::10022-:22 \
  -device virtio-net-pci,netdev=eth0,mac=DE-AD-BE-00-00-01 \
  -drive file="$QCOW2",format=qcow2 \
  -drive file="$UDIMG",format=raw \
  -display none \
  -nographic  \
  ;

  exit $?
fi

echo "Wrong CPU Architecture specified:" $ARCH
exit 1

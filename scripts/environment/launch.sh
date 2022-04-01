#!/usr/bin/env bash

# $1 CPU Architecture Guest System: amd64 (default), aarch64
# $2 proxy (default), no-proxy

source ./build.sh $1 $2

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

  exit
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

  exit
fi

echo "Wrong CPU Architecture specified:" $ARCH
exit 1

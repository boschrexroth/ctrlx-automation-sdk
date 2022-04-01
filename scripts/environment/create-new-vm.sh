#!/usr/bin/env bash

# $1 amd64 | aarch64
# $2 proxy | noproxy
# $3 Destination directory

ARCH=$1
if [ -z "$ARCH" ]
then
      ARCH=amd64
fi

PROXY=$2
if [ -z "$PROXY" ]
then
      PROXY=proxy
fi

DIR=$3
if [ -z "$DIR" ]
then
      DIR=qemu-vm-${ARCH}-${PROXY}
fi

if [ -d "$DIR" ]
then
  echo "ERROR Destination directory ${DIR} exists!"
  exit 1
fi

# Create destination directory
mkdir -p "$DIR" 

cp README.md "$DIR" 
cp build.sh "$DIR" 
cp install-kvm-on-host.sh "$DIR" 
cp install-qemu-on-host.sh "$DIR" 
cp launch.sh "$DIR" 

cp launch-${ARCH}-${PROXY}.sh "$DIR"
cp ubuntu-20.04-server-cloudimg-${ARCH}-user-data-${PROXY}.img "$DIR"


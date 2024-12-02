#!/usr/bin/env bash

# Moves the actual overlay file to a new backing file
# and creates a new overlay file.

VERSION=22.04

# amd64|arm64
ARCH=amd64

#proxy|noproxy
PROXY=noproxy

QCOW2=ubuntu-${VERSION}-server-cloudimg-${ARCH}-${PROXY}.qcow2

for i in $(seq 1 100);
do
    SNAPSHOT=ubuntu-${VERSION}-server-cloudimg-${ARCH}-${PROXY}-$i.qcow2
	if [ ! -f  ${SNAPSHOT} ]
	then
		# File does not exist
		break
	fi
done

echo mv ${QCOW2} ${SNAPSHOT}

# The actual overlay file will become the backing file
mv ${QCOW2} ${SNAPSHOT}

# Create a new overlay file
qemu-img create -F qcow2 -b ${SNAPSHOT} -f qcow2 ${QCOW2}

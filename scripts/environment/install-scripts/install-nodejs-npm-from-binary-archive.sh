#!/usr/bin/env bash

# Installs node.js and npm on the virtual machine

# Uninstall an install snap
sudo snap remove node

# TODO: Check/change the version
VERSION=v16.13.2

ARCH=`uname -p`
ARCH_OK=no

if grep -q "x86_64" <<<${ARCH}; then
    ARCH=x64
    ARCH_OK=yes
fi

if grep -q "aarch64" <<<${ARCH}; then
    ARCH=arm64
    ARCH_OK=yes
fi

if grep -q "no" <<<${ARCH_OK}; then
    echo ERROR: Unknown processor type $ARCH
    exit 1
fi

DISTRO=linux-${ARCH}
NODE=node-${VERSION}-${DISTRO}
ARCHIVE=${NODE}.tar.xz
EXPORT_CMD=export' 'PATH=/usr/local/lib/nodejs/${NODE}/bin
PROFILE=~/.profile
NODEJS_DIR=/usr/local/lib/nodejs
URL=https://nodejs.org/dist/${VERSION}/${ARCHIVE}

if [ ! -f ${ARCHIVE} ]; then
    echo Downloading $URL
    read -p "OK? Waiting 10s ..." -t 10

    wget $URL
    EXIT_CODE=$?
    if [ ! $EXIT_CODE -eq 0 ]
    then
        echo ERROR: Downloading $URL failed with exit code $EXIT_CODE
        exit $EXIT_CODE
    fi
fi

echo Installing content of $ARCHIVE to $NODEJS_DIR
read -p "OK? Waiting 10s ..." -t 10

sudo rm -rf $NODEJS_DIR
sudo mkdir -p $NODEJS_DIR
sudo tar -xJvf ${ARCHIVE} -C $NODEJS_DIR

if [ ! -f $PROFILE ]; then
    echo Creating ${PROFILE}
    echo '# Created by install-nodejs-npm.sh' >${PROFILE}
fi

if ! grep -q $NODE $PROFILE
then
    # Add two line feeds
    echo Appending $EXPORT_CMD:'$PATH' to $PROFILE
    read -p "OK? Waiting 10s ..." -t 10
    sed -i -z 's/$/\n\n/g' $PROFILE
    echo $EXPORT_CMD:'$PATH' >>${PROFILE}
    cat $PROFILE
    . ~/.profile
    echo $PATH changed: $PATH
fi

echo
echo Done: ${NODEJS_DIR}/${NODE}/ installed
node --version
echo     
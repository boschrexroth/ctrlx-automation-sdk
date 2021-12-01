#!/usr/bin/env bash

# Installs go on the virtual machine

# TODO: Check/change the version
GO_VERSION=go1.17.2.linux

ARCH=`uname -p`
ARCH_OK=no

if grep -q "x86_64" <<<${ARCH}; then
  ARCH=amd64
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

ARCHIVE=${GO_VERSION}-${ARCH}.tar.gz
URL=https://golang.org/dl/${ARCHIVE}

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

INSTALL_DIR=/usr/local
GO_INSTALL_DIR=/usr/local/go
EXPORT_CMD=export' 'PATH=/usr/local/go/bin
PROFILE=~/.profile

echo Installing content of $ARCHIVE to $INSTALL_DIR
read -p "OK? Waiting 10s ..." -t 10


sudo rm -rf $GO_INSTALL_DIR
sudo tar -xzf $ARCHIVE -C $INSTALL_DIR

if [ ! -f $PROFILE ]; then
    echo Creating ${PROFILE}
    echo '# Created by install-nodejs-npm.sh' >${PROFILE}
fi

if ! grep -q $GO_INSTALL_DIR $PROFILE
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
echo Done: $ARCHIVE installed to $GO_INSTALL_DIR/ 
go version
echo 
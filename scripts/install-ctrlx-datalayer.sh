#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status.
set -e

DIR=$1
if [ ! -z "$DIR" ]; then
    cd $DIR
fi

sudo apt-get install -y dpkg-dev

# Install debian package locally so that 'apt-get install' will find it (for building sample project snaps)
dpkg-scanpackages -m . >Packages

# Add package to sources list
full_path=$(pwd)
echo "deb [trusted=yes] file:${full_path} ./" | sudo tee /etc/apt/sources.list.d/ctrlx-automation.list

# Fix the APT Error Download that is performed unsandboxed as root as
echo "APT::Sandbox::User \"root\";" | sudo tee /etc/apt/apt.conf.d/10sandbox 

sudo apt-get update

# Install newest ctrlx-datalayer package
sudo apt-get install ctrlx-datalayer -y

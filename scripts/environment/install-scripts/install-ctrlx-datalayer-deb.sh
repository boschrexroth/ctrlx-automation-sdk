#!/usr/bin/env bash

DEP_PACKAGE=*.deb

echo " "
echo "============================================"
echo Installing ${DEP_PACKAGE}
echo "============================================"
read -t 2

if compgen -G ${DEP_PACKAGE} > /dev/null; then
	# Exists
	# Install package containing required component dpkg-scanpackages 
	sudo apt-get install dpkg-dev

	# Install debian package locally so that 'apt-get install' will find it (for building sample project snaps) 
	dpkg-scanpackages -m . > Packages

	# Create full path
	FULL_PATH=$(pwd)
	# Add package to sources list
	sudo sh -c "echo 'deb [trusted=yes] file:${FULL_PATH} ./' > /etc/apt/sources.list.d/ctrlx-automation.list"
	# Use newest sources list
	sudo apt update

	# Install newest ctrlx-datalayer package
	sudo apt-get install -y ctrlx-datalayer
else
	echo "ERROR ${DEP_PACKAGE} not found!"
fi

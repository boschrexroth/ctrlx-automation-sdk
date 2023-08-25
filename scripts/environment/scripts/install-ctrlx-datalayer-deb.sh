#!/usr/bin/env bash

DEP_PACKAGE=*.deb
DEP_DIR=/etc/apt/ctrlx-datalayer

echo " "
echo "=================================================================================="
echo Installing ${DEP_PACKAGE} into ${DEP_DIR}
echo "=================================================================================="
echo " "

if compgen -G ${DEP_PACKAGE} > /dev/null; then
	# Exists
	# Install package containing required component dpkg-scanpackages 
	sudo apt install -y dpkg-dev

	# Install debian package as source locally
	sudo dpkg-scanpackages -m . > Packages

	sudo mkdir -p ${DEP_DIR}
	sudo cp ${DEP_PACKAGE} ${DEP_DIR}
	sudo mv Packages ${DEP_DIR}

	# Add package to sources list
	sudo sh -c "echo 'deb [trusted=yes] file:${DEP_DIR} ./' > /etc/apt/sources.list.d/ctrlx-automation.list"
	
	# Use newest sources list
	sudo apt update

	# Install newest ctrlx-datalayer package
	sudo apt-get install -y ctrlx-datalayer
else
	echo "ERROR ${DEP_PACKAGE} not found!"
fi

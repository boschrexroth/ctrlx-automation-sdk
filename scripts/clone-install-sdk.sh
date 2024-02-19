#!/usr/bin/env bash

#
# Clones ctrlX AUTOMATION SDK and
# installes the required files from ctrlx-automation-sdk-*.zip.
#

WORKING_DIR=$(pwd)
SDK_DIR=${WORKING_DIR}/ctrlx-automation-sdk
SEPARATION_LINE_1="===================================================================================================="
SEPARATION_LINE_2="----------------------------------------------------------------------------------------------------"

if [[ "--help" = $1 ]]; then
	echo ""
	echo "This script creates a local copy of the ctrlX AUTOMATION SDK github repository"
	echo "and installs the required files from the ctrlX AUTOMATION SDK zip archive."
	echo ""
	echo "During this process please enter the desired version (release) of the zip archive to install."
	echo ""
	exit 1
fi

echo ""
echo $SEPARATION_LINE_1
echo "Enter the desired version of the zip archive to install (e.g. 2.4.0)."
echo "This will be used as 'Tag' for the local github repository."
echo $SEPARATION_LINE_2
TAG=2.4.0
read -rp "Version ($TAG)? " TAG
if [ -z "$TAG" ]; then
	TAG=2.4.0
fi

echo ""
echo $SEPARATION_LINE_1
echo "Local SDK github repo: ${SDK_DIR}"
echo "Version (release):     ${TAG}"
echo $SEPARATION_LINE_2
read -rt 20 -p "OK? "

# Older versions have directories without x-permission so sudo ...
sudo rm -rf "${SDK_DIR}"/ 2>/dev/null

echo ""
echo $SEPARATION_LINE_1
echo "Cloning https://github.com/boschrexroth/ctrlx-automation-sdk.git ..."
echo $SEPARATION_LINE_2
git clone --quiet https://github.com/boschrexroth/ctrlx-automation-sdk.git
cd "$SDK_DIR" || exit

echo ""
echo $SEPARATION_LINE_1
echo "Checking out version (tag) ${TAG} ..."
echo $SEPARATION_LINE_2
# git checkout tags/${TAG}

cd "$WORKING_DIR" || exit

ZIP_ARCHIVE=ctrlx-automation-sdk-${TAG}.zip
# Delete existing zip archive
rm "$ZIP_ARCHIVE" 2>/dev/null

echo ""
echo $SEPARATION_LINE_1
DOWNLOAD_URL=https://github.com/boschrexroth/ctrlx-automation-sdk/releases/download/${TAG}/${ZIP_ARCHIVE}
echo "Downloading ${ZIP_ARCHIVE} ..."
echo $SEPARATION_LINE_2
wget -q "$DOWNLOAD_URL"

if [ ! -f "${ZIP_ARCHIVE}" ]; then
	echo ""
	echo "ERROR Could not download ${DOWNLOAD_URL}"
	exit 1
fi

# Create temp dir for unzip
ZIP_DIR=ctrlx-automation-sdk-${TAG}
# sudo required because in older zip archives some directories have no x permission (it's a bug)
sudo rm -rf "$ZIP_DIR" 2>/dev/null
mkdir "$ZIP_DIR"

echo ""
echo $SEPARATION_LINE_1
echo "Unzipping ctrlx-automation-sdk zip ..."
echo $SEPARATION_LINE_2
unzip -xKq "${ZIP_ARCHIVE}" -d "$ZIP_DIR"

echo ""
echo $SEPARATION_LINE_1
echo "Copying files from zip to local git repo, excepting existing files ..."
echo $SEPARATION_LINE_2
rsync -a --ignore-existing "$ZIP_DIR"/ctrlx-automation-sdk/* "$SDK_DIR"

# sudo required because in older zip archives some directories have no x permission (it's a bug)
sudo rm -rf "$ZIP_DIR" 2>/dev/null
rm "$ZIP_ARCHIVE" 2>/dev/null

cd "${SDK_DIR}" || exit

echo ""
echo $SEPARATION_LINE_1
echo "Setting permissions ..."
echo $SEPARATION_LINE_2

# Set drwxrwxr-x for directories and -rw-rw-r-- for files
find . \( -type d -exec chmod 775 {} \; \) -o \( -type f -exec chmod 664 {} \; \)
# oss.flatbuffers* so that oss.flatbuffers.1.12 also fits
chmod a+x bin/comm.datalayer/ubuntu*/release/mddb_compiler
chmod a+x bin/comm.datalayer/ubuntu*/release/dl_compliance
chmod a+x bin/oss.flatbuffers*/ubuntu*/release/flatc
chmod a+x bin/framework/ubuntu*/rexroth-automation-frame
# Add x permission to all .sh files
find . -name '*.sh' -exec chmod +x {} \;

cd "${SDK_DIR}"/deb || exit

echo ""
echo $SEPARATION_LINE_1
echo "Installing required component dpkg-scanpackages ..."
echo $SEPARATION_LINE_2
sudo apt-get install -y dpkg-dev

# Install debian package locally so that 'apt-get install' will find it (for building sample project snaps)
dpkg-scanpackages -m . >Packages

# Add package to sources list
FULL_PATH=$(pwd)
echo "deb [trusted=yes] file:${FULL_PATH} ./" | sudo tee /etc/apt/sources.list.d/ctrlx-automation.list

# Use newest sources list
sudo apt-get update

# Install newest ctrlx-datalayer package
sudo apt-get install -y ctrlx-datalayer

cd "${SDK_DIR}"/scripts || exit

echo ""
echo $SEPARATION_LINE_1
echo "Installing aarch64 libs required for C++ cross compiling ..."
echo $SEPARATION_LINE_2

INSTALL_AARCH64LIBS_SCRIPT=$(find . -name 'install-cpp-aarch64-libs.sh')
if [ -n "$INSTALL_AARCH64LIBS_SCRIPT" ]; then
	source "$INSTALL_AARCH64LIBS_SCRIPT"
else
	sudo apt-get -y install libsystemd-dev:arm64
	sudo apt-get -y install libssl-dev:arm64
	sudo apt-get -y install libzmq3-dev:arm64
	sudo apt-get -y install libzip-dev:arm64
	sudo apt-get -y install uuid-dev:arm64
	sudo apt-get -y install libssl-dev:arm64
	sudo apt-get -y install libcurl4-openssl-dev:arm64
	sudo apt-get -y install libjansson-dev:arm64
	sudo apt-get -y install libffi-dev:arm64
fi

INSTALL_SNAPCRAFT_SCRIPT=$(find . -name 'install-snapcraft.sh')
if [ -n "$INSTALL_SNAPCRAFT_SCRIPT" ]; then
	source "$INSTALL_SNAPCRAFT_SCRIPT"
else
	echo "WARNING snapcraft could not be installed!"
	echo "INFO Search install-snapcraft.sh and start manually."
fi

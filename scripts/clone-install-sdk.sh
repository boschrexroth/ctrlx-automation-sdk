#!/usr/bin/env bash

#
# Clones ctrlX AUTOMATION SDK and
# installes the required files from ctrlx-automation-sdk-*.zip.
#

if [[ "--help" = $1 ]]; then
	echo ""
	echo "This script creates a local copy of the ctrlX AUTOMATION SDK github repository"
	echo "and installs the required files from the ctrlX AUTOMATION SDK zip archive."
	echo ""
	echo "During this process please enter the desired release tag to be installed."
	echo ""
	exit 1
fi

WORKING_DIR=$(pwd)
SDK_DIR=${WORKING_DIR}/ctrlx-automation-sdk
SEPARATION_LINE_1="===================================================================================================="
SEPARATION_LINE_2="----------------------------------------------------------------------------------------------------"

if [ -d ${SDK_DIR} ]; then
	echo ""
	read -rp "Delete existing directory ${SDK_DIR} Y/n? " ANS
	if [[ "$ANS" == *"n"* ]]; then
		echo "Change your working directory or delete the directory yourself ctrlx-automation-sdk/"
		exit 1
	fi

	# Older versions have directories without x-permission so sudo ...
	sudo rm -rf "${SDK_DIR}"/ 2>/dev/null
fi

echo ""
echo $SEPARATION_LINE_1
echo "Cloning https://github.com/boschrexroth/ctrlx-automation-sdk.git ..."
echo $SEPARATION_LINE_2
git clone --quiet https://github.com/boschrexroth/ctrlx-automation-sdk.git
cd "$SDK_DIR" || exit

echo ""
echo $SEPARATION_LINE_1
echo "Enter the desired tag to be checked out from this list:"
git tag
echo $SEPARATION_LINE_2
TAG_LATEST=$(git describe --tags --abbrev=0)
read -rp "Version ($TAG_LATEST)? " TAG
if [ -z "$TAG" ]; then
	TAG=$TAG_LATEST
fi

echo ""
echo $SEPARATION_LINE_1
echo "Local SDK github repo: ${SDK_DIR}"
echo "Version (release):     ${TAG}"
echo $SEPARATION_LINE_2
read -rt 20 -p "OK? "

echo ""
echo $SEPARATION_LINE_1
echo "Checking out tag ${TAG} ..."
echo $SEPARATION_LINE_2
git checkout tags/${TAG}

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
echo "Unzipping ${ZIP_ARCHIVE} ..."
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

# Add x permission (path independ solution)
find $SDK_DIR/bin -name mddb_compiler -exec chmod +x {} \;
find $SDK_DIR/bin -name dl_compliance -exec chmod +x {} \;
find $SDK_DIR/bin -name flatc -exec chmod +x {} \;
# All scripts
find . -name '*.sh' -exec chmod +x {} \;
# Former versions are containing  rexroth-automation-frame
find $SDK_DIR/bin -name rexroth-automation-frame -exec chmod +x {} \;



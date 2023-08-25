#!/usr/bin/env bash

TIME_PROMPT=10

DEST_DIR=ctrlx-automation-sdk
ZIP_ARCHIVE=ctrlx-automation-sdk-*.zip
ZIP_DIR=UNZIPPED_IZqkx4cTqamJikhsXfq5
DEP_PACKAGE=*.deb

MY_DIR=$(pwd)

if [ -d "$DEST_DIR" ];
then
	echo " "
	echo "-----------------------------------------------------------------------"
	echo "Deleting existing directory ${DEST_DIR}"
	read -t $TIME_PROMPT -p "Waiting ${TIME_PROMPT}s - Press ENTER to continue or Ctrl-C to exit"
	sudo rm -rf $DEST_DIR
fi

if ls $ZIP_ARCHIVE 1> /dev/null 2>&1; then
	echo " "
	echo "-----------------------------------------------------------------------"
	echo "Deleting existing zip file(s) ${ZIP_ARCHIVE}"
	read -t $TIME_PROMPT -p "Waiting ${TIME_PROMPT}s - Press ENTER to continue or Ctrl-C to exit"
	rm $ZIP_ARCHIVE
fi

echo " "
echo "-------------------------------------------"
echo " "
git clone https://github.com/boschrexroth/ctrlx-automation-sdk.git

#LATEST=$(curl -s -u USER:TOKEN https://api.github.com/repos/boschrexroth/ctrlx-automation-sdk/releases/latest)
LATEST=$(curl -s https://api.github.com/repos/boschrexroth/ctrlx-automation-sdk/releases/latest)
	
if grep -q "limit exceeded" <<<${LATEST}; then
	echo " "
    echo ERROR
	echo " "
	echo ${LATEST}
	echo " "
	echo "Hint:"
	echo This error happens often because a proxy server is used.
	echo In this case github registered a hugh number of requests from the same machine.
	echo " "
	echo Workaround:
    echo "- Open/Register your user account on github"
	echo "- Create a user token"
	echo "- Add user:token to the curl command above: curl -s -u USER:TOKEN ..."
    exit 1
fi

# .assets[0].browser_download_url   --> ctrlx-...zip
# .assets[1].browser_download_url   --> ctrlx-...deb
sudo apt-get install jq -y
DOWNLOAD_URL=$(echo $LATEST | jq -r .assets[0].browser_download_url)

echo " "
echo "------------------------------------------------------"
echo "Downloading latest ctrlx-automation-sdk zip archive..."
echo " "
wget $DOWNLOAD_URL	
echo " "

if compgen -G ${ZIP_ARCHIVE} > /dev/null; then
	echo Done
else
	echo "ERROR Could not download ${ZIP_ARCHIVE} does not exist"
    exit 1
fi

echo " "
echo "------------------------------------------------------"
echo "Unzipping ctrlx-automation-sdk zip archive..."
echo " "

# Get subdirectories of github repo
DIRS_UNDER_GITHUB=$(ls -d ${DEST_DIR}/*/)

# Create temp dir for unzip
sudo rm -rf ${ZIP_DIR}/
mkdir $ZIP_DIR
cd $ZIP_DIR

unzip -xK $(ls ../${ZIP_ARCHIVE})

# Remove unzipped dirs of zip archive already stored in github repo
sudo rm -rf $DIRS_UNDER_GITHUB 

cd $DEST_DIR

# Get list of remaining dirs to be moved to github dir
DIRS_IN_ZIP=$(ls -d */)

for DIR in $DIRS_IN_ZIP
do 
	mv "$DIR" ${MY_DIR}/${DEST_DIR}
done

sudo rm -rf ${ZIP_DIR}/
rm ${MY_DIR}/$ZIP_ARCHIVE

echo " "
echo "------------------------------------------------------"
echo "Setting x permissions..."
echo " "

cd ${MY_DIR}/${DEST_DIR}

chmod a+x bin/oss.flatbuffers*/ubuntu22-gcc-*/release/flatc 
chmod a+x bin/framework/ubuntu22-gcc-*/rexroth-automation-frame
chmod a+w -R include
sudo chown -R boschrexroth:boschrexroth fbs/comm/ethercat/master/
sudo chown -R boschrexroth:boschrexroth  bfbs/comm/ethercat/master/
sudo chmod 777 -R fbs/comm/ethercat/master/
sudo chmod 777 -R bfbs/comm/ethercat/master/
find . -name '*.sh' -exec chmod +x {} \;

cd deb/


echo " "
echo "------------------------------------------------------"
echo "Installing debian package ${DEP_PACKAGE}..."
echo " "

if compgen -G ${DEP_PACKAGE} > /dev/null; then

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

REM Installing snapcraft
~/scripts/install-snapcraft.sh" 


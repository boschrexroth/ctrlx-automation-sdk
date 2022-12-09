#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing ctrlX AUTOMATION SDK from github
echo "============================================"
read -t 2

ZIP_ARCHIVE=ctrlx-automation-sdk-*.zip
if compgen -G ${ZIP_ARCHIVE} > /dev/null; then
	read -p "${ZIP_ARCHIVE} exists, delete, exit or continue d/e/c?" USERS_INPUT
	
	if grep -q "d" <<<${USERS_INPUT}; then
		rm ${ZIP_ARCHIVE}
	fi
	
	
	if grep -q "e" <<<${USERS_INPUT}; then
		exit 1
	fi
fi

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
    echo "- Download the zip archive from here https://github.com/boschrexroth/ctrlx-automation-sdk/releases"
	echo "- Copy it into this directory."
	echo "- Restart this script."
	echo " "	
	read -p "Continue or exit c/e?" USERS_INPUT
	if grep -q "e" <<<${USERS_INPUT}; then
		exit 1
	fi
	echo " "
else
	# .assets[0].browser_download_url   --> ctrlx-...zip
	# .assets[1].browser_download_url   --> ctrlx-...deb
	sudo apt-get install jq -y
	DOWNLOAD_URL=$(echo $LATEST | jq -r .assets[0].browser_download_url)
	wget $DOWNLOAD_URL	
fi

if compgen -G ${ZIP_ARCHIVE} > /dev/null; then
	unzip -XK $(ls ctrlx-automation-sdk-*.zip)
	chmod a+x ctrlx-automation-sdk/bin/oss.flatbuffers*/ubuntu20-gcc-*/release/flatc 
	chmod a+x ctrlx-automation-sdk/bin/framework/ubuntu20-gcc-*/rexroth-automation-frame
	chmod a+w -R ctrlx-automation-sdk/include
	sudo chown -R boschrexroth:boschrexroth ctrlx-automation-sdk/fbs/comm/ethercat/master/
	sudo chown -R boschrexroth:boschrexroth  ctrlx-automation-sdk/bfbs/comm/ethercat/master/
	sudo chmod 777 -R ctrlx-automation-sdk/fbs/comm/ethercat/master/
	sudo chmod 777 -R ctrlx-automation-sdk/bfbs/comm/ethercat/master/
	find ctrlx-automation-sdk/ -name '*.sh' -exec chmod +x {} \;
	
	cd ctrlx-automation-sdk/deb/
	source ~/install-ctrlx-datalayer-deb.sh
	
else
	echo "ERROR ${ZIP_ARCHIVE} does not exist"
fi

echo " "
echo "============================================"
echo Installing snapcraft snap
echo "============================================"

sudo snap install snapcraft --classic
source ~/install-deb.sh




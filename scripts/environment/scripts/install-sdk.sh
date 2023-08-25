#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing ctrlX AUTOMATION SDK
echo "============================================"
echo " "

ZIP_ARCHIVE=ctrlx-automation-sdk-*.zip
if compgen -G ${ZIP_ARCHIVE} > /dev/null; then
	read -p "${ZIP_ARCHIVE} exists! Delete, exit or continue: d e c? " USERS_INPUT
	
	if grep -q "e" <<<${USERS_INPUT}; then
		exit 1
	fi

	if grep -q "d" <<<${USERS_INPUT}; then
		rm ${ZIP_ARCHIVE}
	fi
fi

if compgen -G ${ZIP_ARCHIVE} > /dev/null; then
	# OK it exists
	echo " "
else
	source ~/scripts/download-sdk.sh
fi

if compgen -G ${ZIP_ARCHIVE} > /dev/null; then

	unzip -XK $(ls ctrlx-automation-sdk-*.zip)
	chmod a+x ctrlx-automation-sdk/bin/oss.flatbuffers*/ubuntu22-gcc-*/release/flatc 
	chmod a+x ctrlx-automation-sdk/bin/framework/ubuntu22-gcc-*/rexroth-automation-frame
	chmod a+w -R ctrlx-automation-sdk/include

	find ctrlx-automation-sdk/ -name '*.sh' -exec chmod +x {} \;
	
	cd ctrlx-automation-sdk/deb/
	source ~/scripts/install-ctrlx-datalayer-deb.sh
	
else
	echo "ERROR ${ZIP_ARCHIVE} does not exist"
fi

source ~/scripts/install-snapcraft.sh


#!/usr/bin/env bash

echo " "
echo Downloading ctrlX AUTOMATION SDK from github
echo " "

LATEST=$(curl -s https://api.github.com/repos/boschrexroth/ctrlx-automation-sdk/releases/latest)

if grep -q "limit exceeded" <<<${LATEST}; then
	# Providing a dummy user/token can avoid the limit exceeded problem
	LATEST=$(curl -s -u xx:yy https://api.github.com/repos/boschrexroth/ctrlx-automation-sdk/releases/latest)
fi

if grep -q "limit exceeded" <<<${LATEST}; then
	echo " "
    echo ERROR
	echo " "
	echo ${LATEST}
	echo " "
	echo "Hint:"
	echo "This error can happen on an unauthenticated request and if a proxy server is used."
	echo "In this case, github has registered too many requests from the same machine."
	echo " "
	echo "To avoid this please provide your github username and an access token."
	echo "See - https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token"
	echo " "
	echo "Enter your github user and token - or ctrl-c to exit"
	read -p "github user: " GITHUB_USER
	read -s -p "github token: " GITHUB_TOKEN
	echo " "	
	LATEST=$(curl -s -u ${GITHUB_USER}:${GITHUB_TOKEN} https://api.github.com/repos/boschrexroth/ctrlx-automation-sdk/releases/latest)
	echo " "
fi

if grep -q "limit exceeded" <<<${LATEST}; then
	echo " "
    echo ERROR
	echo " "
	echo ${LATEST}
	echo " "
	echo "Recommended workaround:"
    echo "- Download the zip archive manually from here https://github.com/boschrexroth/ctrlx-automation-sdk/releases"
	echo "- Restart install-sdk.sh"
	echo " "	
	exit 1
fi

# .assets[0].browser_download_url   --> ctrlx-...zip
# .assets[1].browser_download_url   --> ctrlx-...deb
DOWNLOAD_URL=$(echo $LATEST | jq -r .assets[0].browser_download_url)
wget $DOWNLOAD_URL	

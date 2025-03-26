#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing node.js and npm
echo "============================================"
echo " "

sudo snap install node --classic --channel=22/stable
npm config set prefix ${HOME}/npm

if [[ ! -z "$HTTP_PROXY" ]]; then
  echo proxy=http://10.0.2.2:3128/ >> ~/.npmrc
fi

if [[ ! -z "$HTTPS_PROXY" ]]; then
  echo https-proxy=http://10.0.2.2:3128/ >> ~/.npmrc
fi

node --version
npm --version


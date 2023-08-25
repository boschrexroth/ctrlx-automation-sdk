#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing node.js and npm
echo "============================================"
echo " "

sudo snap install node --classic
npm config set prefix ${HOME}/npm
echo proxy=http://10.0.2.2:3128/ >> ~/.npmrc
echo https-proxy=http://10.0.2.2:3128/ >> ~/.npmrc
node --version
npm --version

source ~/scripts/install-snapcraft.sh

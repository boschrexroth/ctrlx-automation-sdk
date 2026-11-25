#!/usr/bin/env bash

echo " "
echo "============================================"
echo "Installing Node.js ..."
echo "============================================"
echo " "

sudo snap install node --classic --channel=24/stable
npm config set prefix "${HOME}"/npm

if [[ -n "$HTTP_PROXY" ]]; then
  echo proxy=http://10.0.2.2:3128/ >> ~/.npmrc
fi

if [[ -n "$HTTPS_PROXY" ]]; then
  echo https-proxy=http://10.0.2.2:3128/ >> ~/.npmrc
fi

# Add Environment variables to user profile
profile=$(cat ~/.profile)
if [[ "$profile" != *"NPM"* ]];then
  sudo echo 'export NPM=~/npm/bin' | sudo tee -a ~/.profile
  sudo echo 'export PATH=$NPM:$PATH' | sudo tee -a ~/.profile
  export NPM=~/npm/bin
  export PATH=$NPM:$PATH
fi

echo "Node.js: $(node --version)"
echo "npm:     $(npm --version)"
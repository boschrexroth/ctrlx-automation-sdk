#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing latest LTS dotnet-sdk 
echo "============================================"
echo " "

# Scripted installation based on
# https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual#scripted-install
# https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual#set-environment-variables-system-wide
# https://learn.microsoft.com/de-de/dotnet/core/tools/dotnet-install-script

# Install Latest .NET SDK (LTS)
wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
sudo chmod +x ./dotnet-install.sh
./dotnet-install.sh --version latest

# Add Environment variables to user profile
profile=$(cat ~/.profile)
if [[ "$profile" != *"DOTNET_ROOT"* ]];then
  sudo echo 'export DOTNET_ROOT=$HOME/.dotnet' | sudo tee -a ~/.profile
  sudo echo 'export PATH=$PATH:$DOTNET_ROOT:$DOTNET_ROOT/tools' | sudo tee -a ~/.profile
fi

# Dump version and sdk
dotnet --list-sdks

# break the info output
if [ "$1" == "quiet" ]; then
  exit 0
fi

echo
echo "dotnet-sdk installation completed."
echo
echo "============================================"
echo "NOW, PLEASE DO EITHER OF THE FOLLOWING:"
echo " - Reload your profile by executing command '. ~/.profile' in terminal."
echo " or"
echo " - Restart your App Builder Environment."
echo "============================================"
echo
read -r -p "Press enter to continue"
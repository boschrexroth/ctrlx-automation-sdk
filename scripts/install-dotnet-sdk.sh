#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing dotnet-sdk
echo "============================================"
echo " "

# Remove these packages
sudo apt remove  -y 'dotnet*' 'aspnet*' 'netstandard*'
# Remove sources list
sudo rm /etc/apt/sources.list.d/microsoft-prod.list 2>/dev/null


# Install Debian Package
sudo apt update
sudo apt install -y dotnet-sdk-6.0 

# Test installation
dotnet --version


source ~/scripts/install-snapcraft.sh
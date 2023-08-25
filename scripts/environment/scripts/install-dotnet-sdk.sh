#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing dotnet-sdk
echo "============================================"
echo " "

# Install as snap
#sudo snap install dotnet-sdk --classic 
#sudo snap alias dotnet-sdk.dotnet dotnet
#sh -c 'echo "export DOTNET_ROOT=/snap/dotnet-sdk/current" >> /home/boschrexroth/.bashrc'
#export DOTNET_ROOT=/snap/dotnet-sdk/current
#sudo ln -sv /snap/dotnet-sdk/current/dotnet /usr/local/bin/dotnet

# Install as Debian Package
wget https://packages.microsoft.com/config/ubuntu/22.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb
rm packages-microsoft-prod.deb
sudo apt-get update
sudo apt-get install -y dotnet-sdk-6.0
dotnet --version

source ~/scripts/install-snapcraft.sh

#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing dotnet-sdk
echo "============================================"
echo " "

# Install Microsoft Package source
sudo wget https://packages.microsoft.com/config/ubuntu/22.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb
sudo rm packages-microsoft-prod.deb

# Install Debian Package
sudo GNUTLS_CPUID_OVERRIDE=0x1 apt update
sudo GNUTLS_CPUID_OVERRIDE=0x1 apt install -y dotnet-sdk-8.0 

# Test installation
dotnet --version

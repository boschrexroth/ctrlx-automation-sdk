#!/usr/bin/env bash


# Exit immediately if a command exits with a non-zero status.
set -e

etc_environment=$(cat /etc/environment)
if [[ ${etc_environment} != *"DEBIAN_FRONTEND=noninteractive"* ]];then
    # https://unix.stackexchange.com/questions/701285/is-debian-frontend-noninteractive-redundant-with-apt-get-yqq
    # DEBIAN_FRONTEND is read by debconf

    echo "-------------------------------------------------------"
    echo "Disable debian fronted ..."
    echo "-------------------------------------------------------"

    export DEBIAN_FRONTEND=noninteractive
    sudo apt-get install -y debconf
    sudo echo "DEBIAN_FRONTEND=noninteractive" | sudo tee -a /etc/environment
fi

# Get architecture, define x-build architecture
echo "-------------------------------------------------------"
echo "Define cross-build architecture ..."
echo "-------------------------------------------------------"
arch=$(dpkg --print-architecture)
if [[ ${arch} == "amd64" ]];then
    arch_x=arm64
    repository_url_x="http://ports.ubuntu.com/ubuntu-ports"
else
    arch_x=amd64
    repository_url_x="http://archive.ubuntu.com/ubuntu"
fi

echo arch: $arch
echo arch_x: $arch_x
echo repository_url_x: $repository_url_x

# Add an extra architecture for x-build and print it
echo "-------------------------------------------------------"
echo "Add cross-build architecture $arch_x ..."
echo "-------------------------------------------------------"
sudo dpkg --add-architecture $arch_x
echo foreign-architectures: $(sudo dpkg --print-foreign-architectures)

echo "-------------------------------------------------------"
echo "Patch '/etc/apt/sources.list.d/multiarch-libs.list' ..."
echo "-------------------------------------------------------"
# Get distribution name
dist="$(lsb_release -sc)"
sudo echo "deb [arch=$arch_x] $repository_url_x ${dist} main restricted universe multiverse" | sudo tee /etc/apt/sources.list.d/multiarch-libs.list
sudo echo "deb [arch=$arch_x] $repository_url_x ${dist}-backports main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
sudo echo "deb [arch=$arch_x] $repository_url_x ${dist}-security main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
sudo echo "deb [arch=$arch_x] $repository_url_x ${dist}-updates main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list

echo "-------------------------------------------------------"
echo "Patch '/etc/apt/sources.list' ..."
echo "-------------------------------------------------------"
sudo sed -i "s/deb http:/deb [arch=$arch] http:/g" /etc/apt/sources.list
cat /etc/apt/sources.list

echo "-------------------------------------------------------"
echo "Update packages ..."
echo "-------------------------------------------------------"
sudo apt-get update

echo "-------------------------------------------------------"
echo "Install required packages for $arch ..."
echo "-------------------------------------------------------"
# TODO Ist bereits im AppBuildEnv
sudo apt-get install zip -y
sudo apt-get install unzip -y
sudo apt-get install p7zip-full -y
sudo apt-get install git -y
sudo apt-get install apt-transport-https -y
sudo apt-get install whois -y
sudo apt-get install net-tools -y
sudo apt-get install pkg-config -y
sudo apt-get install jq -y
sudo apt-get install sshpass -y
sudo apt-get install dpkg-dev -y

sudo apt-get install python3-pip -y
sudo apt-get install virtualenv -y

sudo apt-get install build-essential -y
sudo apt-get install gdb -y

sudo apt-get install gdb-multiarch -y
sudo apt-get install cmake -y

sudo apt-get install libxml2-dev -y
sudo apt-get install uuid-dev -y
sudo apt-get install libbz2-1.0 -y
sudo apt-get install libzmq3-dev -y
sudo apt-get install libsystemd-dev -y
sudo apt-get install libssl-dev -y

echo "-------------------------------------------------------"
echo "Install required packages for $arch_x ..."
echo "-------------------------------------------------------"
sudo apt-get install crossbuild-essential-$arch_x -y
sudo apt-get install libxml2-dev:${arch_x} -y
sudo apt-get install uuid-dev:${arch_x} -y
sudo apt-get install libbz2-1.0:${arch_x} -y
sudo apt-get install libzmq3-dev:${arch_x} -y
sudo apt-get install libsystemd-dev:${arch_x} -y
sudo apt-get install libssl-dev:${arch_x} -y

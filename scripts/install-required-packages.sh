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
    echo "DEBIAN_FRONTEND=noninteractive" | sudo tee -a /etc/environment
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

echo "arch: $arch"
echo "arch_x: $arch_x"
echo "repository_url_x: $repository_url_x"

# Add an extra architecture for x-build and print it
echo "-------------------------------------------------------"
echo "Add cross-build architecture $arch_x ..."
echo "-------------------------------------------------------"
foreign_architectures=$(dpkg --print-foreign-architectures)
echo "foreign-architectures: '${foreign_architectures}'"
if [[ "${foreign_architectures}" != *"${arch_x}"* ]] ; then
    sudo dpkg --add-architecture ${arch_x}

    echo "-------------------------------------------------------"
    echo "Patch '/etc/apt/sources.list.d/multiarch-libs.list' ..."
    echo "-------------------------------------------------------"
    # Get distribution name
    dist="$(lsb_release -sc)"
    echo "deb [arch=$arch_x] $repository_url_x ${dist} main restricted universe multiverse" | sudo tee /etc/apt/sources.list.d/multiarch-libs.list
    echo "deb [arch=$arch_x] $repository_url_x ${dist}-backports main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
    echo "deb [arch=$arch_x] $repository_url_x ${dist}-security main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list
    echo "deb [arch=$arch_x] $repository_url_x ${dist}-updates main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/multiarch-libs.list

    echo "-------------------------------------------------------"
    echo "Patch '/etc/apt/sources.list' ..."
    echo "-------------------------------------------------------"
    sudo sed -i "s/deb http:/deb [arch=$arch] http:/g" /etc/apt/sources.list
    cat /etc/apt/sources.list
fi

echo "-------------------------------------------------------"
echo "Update packages ..."
echo "-------------------------------------------------------"
sudo apt-get update

echo "-------------------------------------------------------"
echo "Install required packages for $arch ..."
echo "-------------------------------------------------------"

sudo apt-get install -y \
    zip \
    unzip \
    p7zip-full \
    git \
    apt-transport-https \
    whois \
    net-tools \
    pkg-config \
    jq \
    sshpass \
    dpkg-dev \
    python3-pip \
    virtualenv \
    build-essential \
    gdb \
    gdb-multiarch \
    cmake \
    libxml2-dev \
    uuid-dev \
    libbz2-1.0 \
    libsystemd-dev \
    libssl-dev

echo "-------------------------------------------------------"
echo "Install required packages for $arch_x ..."
echo "-------------------------------------------------------"
sudo apt-get install -y \
    crossbuild-essential-$arch_x \
    libxml2-dev:${arch_x} \
    uuid-dev:${arch_x} \
    libbz2-1.0:${arch_x} \
    libsystemd-dev:${arch_x} \
    libssl-dev:${arch_x}

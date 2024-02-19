#!/usr/bin/env bash

#
# This script installs debian packages required to build apps
# with the ctrlX AUTOMATION SDK.
#


PROXY=n
echo "------------------------------------------------------------------"
echo "If you are using a HTPP/HTTPS proxy and the URLs are not already"
echo "registered in /etc/wgetrc and /etc/environment"
echo "answer y and enter the URLs."
echo "If you restart this script this step must be skipped, enter N."
echo ""
read -p "Add HTTP/HTTPS proxy server URLs (y/N) " PROXY

if grep -q "y" <<<${PROXY}
then

  HTTP_PROXY_URL=http://10.0.2.2:3128
  read -p "Enter the HTTP proxy server URL (${HTTP_PROXY_URL}) " HTTP_PROXY_URL
  if [ "$HTTP_PROXY_URL" == "" ];then
    HTTP_PROXY_URL=http://10.0.2.2:3128
  fi

  HTTPS_PROXY_URL=http://10.0.2.2:3128
  read -p "Enter the HTTPS proxy server URL (${HTTPS_PROXY_URL}) " HTTPS_PROXY_URL
  if [ "$HTTPS_PROXY_URL" == "" ];then
    HTTPS_PROXY_URL=http://10.0.2.2:3128
  fi

  echo " "
  echo "------------------------------------------------------------------"
  echo "HTTP proxy server ${HTTP_PROXY_URL}"
  echo "HTTPS proxy server ${HTTPS_PROXY_URL}"
  echo " "
  read -t 10 -p "OK? "

  sudo chmod 777 /etc

  sudo chmod 777 /etc/wgetrc
  sudo echo "http_proxy = ${HTTP_PROXY_URL}" >> /etc/wgetrc
  sudo echo "https_proxy = ${HTTPS_PROXY_URL}" >> /etc/wgetrc
  sudo echo "use_proxy = on" >> /etc/wgetrc

  #   # Proxy settings for environment
  if [ -f /etc/environment ]
  then
    sudo chmod 777 /etc/environment
  fi        
  sudo echo "http_proxy=\"${HTTP_PROXY_URL}\"" >> /etc/environment
  sudo echo "https_proxy=\"${HTTPS_PROXY_URL}\"" >> /etc/environment
  sudo echo "HTTP_PROXY=\"${HTTP_PROXY_URL}\"" >> /etc/environment
  sudo echo "HTTPS_PROXY=\"${HTTPS_PROXY_URL}\"" >> /etc/environment
  sudo echo "no_proxy=localhost,127.0.0.1,10.0.2.2,.local" >> /etc/environment

fi	

sudo apt update -y
sudo apt upgrade -y

sudo apt install -y \
   zip \
   unzip \
   p7zip-full \
   git \
   cifs-utils \
   apt-transport-https \
   build-essential \
   crossbuild-essential-arm64 \
   gcc \
   gdb-multiarch  \
   gdb \
   whois \
   python3-pip \
   virtualenv \
   cmake \
   net-tools \
   pkg-config \
   jq \
   sshpass \
   libsystemd-dev \
   libzmq3-dev \
   libssl-dev \
   libzip-dev \
   uuid-dev \
   libssl-dev \
   libcurl4-openssl-dev \
   libjansson-dev \
   libffi-dev

# https://wiki.ubuntu.com/MultiarchSpec
sudo dpkg --add-architecture arm64
sudo chmod 777 /etc/apt/sources.list.d/
sudo chmod 777 /etc/apt/sources.list.d/*

sudo echo "deb [arch=arm64] http://ports.ubuntu.com/ jammy main restricted universe" > /etc/apt/sources.list.d/multiarch-libs.list
sudo echo "deb [arch=arm64] http://ports.ubuntu.com/ jammy-updates main restricted universe" >> /etc/apt/sources.list.d/multiarch-libs.list

  # Qualify architecture
sudo sed -i 's/deb http:/deb [arch=amd64] http:/g' /etc/apt/sources.list

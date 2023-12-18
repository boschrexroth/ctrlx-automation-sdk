#!/usr/bin/env bash     

INFO=$(snap list snapcraft)
if [[ ${INFO} != *"6."* ]]; then
    echo Removing: $INFO
    sudo snap remove snapcraft
fi

echo " "
echo "============================================"
echo Installing snapcraft 6.x/stable
echo "============================================"
echo " "

sudo snap install snapcraft --channel=6.x/stable --classic


#!/usr/bin/env bash    

etc_environment=$(cat /etc/environment)
if [[ ${etc_environment} != *"SNAPCRAFT_BUILD_ENVIRONMENT=host"* ]];then
    sudo echo "SNAPCRAFT_BUILD_ENVIRONMENT=host" | sudo tee -a /etc/environment
fi

if [[ ${etc_environment} != *"SNAPCRAFT_ENABLE_EXPERIMENTAL_TARGET_ARCH=1"* ]];then
    sudo echo "SNAPCRAFT_ENABLE_EXPERIMENTAL_TARGET_ARCH=1" | sudo tee -a /etc/environment
fi

function  install_snapcraft() {
    echo ""
    echo "-------------------------------------------------------"
    echo "install snapcraft 7.x"
    echo "-------------------------------------------------------"
    echo ""

    sudo snap install snapcraft --channel=7.x/stable --classic
}

# Avoid return or exit, so that thsi sscript can be called from elsewhere

snap_list=$(snap list)
if [[ ${snap_list} != *"snapcraft"* ]];then
    # snapcraft is not installed, so install it
    install_snapcraft
else
    # snapcraft is installed
    snapcraft_installed=$(snap list snapcraft)
    if [[ ${snapcraft_installed} != *"7."* ]];then
        # but not the desired version, so remove this versin and install the right one
        echo Uninstalling inappropriate snapcraft version...
        sudo snap remove snapcraft 2>/dev/null
        install_snapcraft
    fi
fi


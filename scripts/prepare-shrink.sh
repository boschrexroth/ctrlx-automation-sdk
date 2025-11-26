#!/bin/bash

#setup

echo Prepare QEMU shrink with shut down?
read -r -p "Press enter to continue"
echo Please execute shrink.bat afterwards to complete the shrink operation.

sudo fstrim -v /
sudo dd if=/dev/zero of=dev-zero
sudo rm dev-zero
sudo poweroff








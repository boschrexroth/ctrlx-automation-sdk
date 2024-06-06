#!/usr/bin/env bash

sudo apt update && upgrade

# QEMU needs the virtualization daemon libvirt 
sudo apt install -y libvirt-daemon
sudo systemctl enable libvirtd
sudo systemctl start libvirtd

sudo apt install -y qemu-kvm 
sudo apt install -y qemu-system-aarch64

#!/usr/bin/env bash

# Install KVM etc.
sudo apt-get -y install qemu-kvm libvirt-bin bridge-utils virtinst virt-manager

# Test KVM installation
sudo systemctl is-active libvirtd

# Add the actual user to user groups
sudo usermod -aG libvirt $USER
sudo usermod -aG kvm $USER

## Setup

### Install KVM on Your Linux Host

We recommend to install KVM (Kernel-based Virtual Machine) on your host sytem.

If your host OS is Ubuntu 20.04 just start

    ./install-kvm-on-host.sh

See here for further informations:

[How to Install Kvm on Ubuntu 20.04](https://linuxize.com/post/how-to-install-kvm-on-ubuntu-20-04/)

[linux-kvm](https://www.linux-kvm.org/page/Documents)


### Install the QEMU Machine Emulator and Virtualizer Toolset

The QEMU packages for Ubuntu are not up to date so we have to clone the whole toolset, build and install it. Therefor just start this script:

    ./install-qemu-on-host.sh

It removes former QEMU packages, clones the repository, builds and installs the toolset.

Be patient -  this will take a while.

### Install a QEMU Virtual Machine

Change to the directory __public/scripts/environment/__ of the ctrlX SDK and call one of the _create....sh_ scripts with a destination directory as argument. 

The script will create a runtime directory for your VM and copy all necessary files into this directory.
These scripts are available:

- create-new-vm-amd64-proxy.sh: Create an AMD64 VM with internet access using a proxy server on the host system 
- create-new-vm-amd64-noproxy.sh: Create an AMD64 VM with direct internet access
- create-new-vm-aarch64-proxy.sh: Create an AARCH64 VM with internet access using a proxy server on the host system
- create-new-vm-aarch64-noproxy.sh: Create an AARCH64 VM with direct internet access

Example: $ ./create-new-vm-amd64-proxy.sh ~/qemu-amd64-proxy

## Configuration

The file cloud-config contains several settings you might change:

- hostname
- proxy settings (default __10.0.2.2:3128__ for both http and https)
- user/passwd

For further informations see [here](https://cloudinit.readthedocs.io/en/latest/topics/examples.html).

There is a port forwarding from port 10022 of your host to port 22 of your guest operating system.
This is defined as 'hostfwd=tcp::10022-:22' in the launch-*-host.sh script files.

If there are problems with this forwarding search a free port on your host e.g. 20022 and make the changes in the script files.

## Start the QEMU Virtual Machine

On a Linux __amd64__ host system you can use the following shell scripts to launch your QEMU VM:

- launch-amd64-proxy.sh: AMD64 guest with proxy usage
- launch-amd64-noproxy.sh: AMD64 guest with direct internet access
- launch-aarch64-proxy.sh: AARCH64 guest with proxy usage
- launch-aarch64-noproxy.sh: AARCH64 guest with direct internet access

Continue to read [here](./1-README.md), chapter 'First Boot of the __QEMU VM__'


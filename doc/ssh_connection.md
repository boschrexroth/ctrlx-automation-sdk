# Using SSH to Connect to a Virtual Machine

## Overview

SSH (Secure Shell) is used to connect to a computer remotely, whether it is a Virtual Machine (VM) or a physical machine.

If you are using a VM with the default port forwarding settings, the SSH address is:
__localhost:10022__

This means that port 10022 on your host computer is forwarded to port 22 on the VM.

For a real machine, the address is:
__IP:22__
Example: __192.168.1.1:22__

The most common SSH tools are available for both Windows and Linux:

* ssh: connects to a remote machine securely
* scp: copies files securely

## Public Key Authentication

Instead of using a username and password, we recommend __public key authentication__. In this method, you create two keys:

* a private key, which stays on your computer and must be kept secret
* a public key, which is copied to the remote computer

When you connect, SSH uses your private key instead of a username and password.

A guide to creating a key pair and copying the public key to a remote computer is here: [What is SSH Public Key authentication?](https://www.ssh.com/academy/ssh/public-key-authentication)

## Port Specification for ssh and scp

Port 22 is the default port for SSH connections. If you are connecting to a VM that uses port forwarding, you must also specify the forwarded port number. Examples:

* scp -p 10022 ...
* scp -__P__ 10022 ...
* ssh-copy-id -p 10022 ...

> !!! important: for scp, the option is uppercase P.

## Git Bash for Windows Hosts

On Windows, ssh-copy-id is not available by default. You can install Git for Windows, which includes __Git Bash__.

From a __Git Bash__ terminal, you can use ssh-copy-id in the same way as shown above.

# Using SSH to Connect to a Virtual Machine

## Overview

Regardless of whether you want to connect to a VM or a real machine, "Secure Shell" (SSH) is used.

The SSH address of a virtual machine running with default port forwarding is __localhost:10022__
Here port 10022 of the host is forwarded to port 22 of the virtual machine (guest)).

The address of real machine is __IP:22__ e.g. 192.168.1.1:22

The most important SSH tools are available for Windows and Linux:

* ssh is secure shell
* scp is secure copy

## Public Key Authentication

Instead of user/password authentication, we recommend __public key authentication__. Here you have a privat key and a public key (text) file.
The public key file is copied to the computer you want to connect to, the private key is secret.

SSH tools are using you private key during a SSH connection instead of user/password.

Creating a key pair and copy it to the remote computer is described here: [What is SSH Public Key authentication?](https://www.ssh.com/academy/ssh/public-key-authentication)

## Port Specification for ssh and scp

Port 22 is the default port for a SSH connection. To communicate with a VM running with port forwarding you have to specify the port number e.g.

* scp -p 10022 ...
* scp -__P__ 10022 ...
* ssh-copy-id -p 10022 ...

!!! important
	scp expects it's option P in uppercase!

## Git Bash for Windows 10 Hosts

Because ssh-copy-id isn't available under Windows you can install Git for Windows which provides __Git Bash__.

From a __git bash__ session you can call ssh-copy-id as shown above.

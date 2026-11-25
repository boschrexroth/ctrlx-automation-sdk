<!-- markdownlint-disable MD033 -->
# Introduction

This document explains how to connect an **App Build Environment** to a ctrlX CORE.

The examples assume that the host computer runs Windows and uses [Px Proxy](./px.md).

## Connections between Host and App Build Environment

The **App Build Environment** is connected to the host computer through port forwarding. This means that services running in the SDK virtual machine can be accessed from the host computer through `localhost:FORWARDED_PORT`.

In the examples below:

* Port 10022 on the host computer is forwarded to port 22 (SSH) on the SDK amd64 VM.
* Port 20022 on the host computer is forwarded to port 22 (SSH) on the SDK aarch64 VM.

From inside the **App Build Environment**:

* `10.0.2.2` is the IP address of the host computer.
* `10.0.2.2` is also the gateway address.*
* <http://10.0.2.2:3128> is the address of the px.exe proxy running on the Windows host.

\* QEMU provides a virtual gateway. It routes network traffic from the App Build Environment to other devices and services.

## Connecting the Host Computer to a ctrlX CORE

There are three ways to connect to a ctrlX CORE:

1. A ctrlX CORE<sup>virtual</sup> using port forwarding configured in ctrlX WORKS.
2. A ctrlX CORE<sup>virtual</sup> using a virtual network adapter configured in ctrlX WORKS.
3. A physical ctrlX CORE connected through its network adapter.

### ctrlX CORE<sup>virtual</sup> with Port Forwarding

Like the **App Build Environment**, a ctrlX CORE<sup>virtual</sup> can be connected through port forwarding configured in ctrlX WORKS. When the ctrlX CORE<sup>virtual</sup> starts, an internal QEMU instance provides this port forwarding.

![ctrlX with port forwarding](images/ctrlx-sdk-port-forwarding.png)

In the image above, port 12345 is used for C++ remote debugging. This port must also be configured for forwarding in ctrlX WORKS.

> !!! important
> If port 8443 on the host computer is forwarded to the default HTTPS port 443 on the ctrlX CORE, add `sslport=8443` to the TCP connection string.

The connection string then ends with **?sslport=8443**:

    tcp://boschrexroth:boschrexroth@10.0.2.0?sslport=8443

Alternatively, change the port forwarding configuration to 443:443.

### ctrlX CORE<sup>virtual</sup> with a Network Adapter

In this setup, ctrlX WORKS and its internal QEMU instance provide the ctrlX CORE<sup>virtual</sup> with a virtual network adapter. The default IP address of this adapter is `192.168.1.1`.

To connect the Windows host computer to the ctrlX CORE<sup>virtual</sup>, ctrlX WORKS creates a **TAP Windows Adapter v9**.

![ctrlX CORE](images/ctrlx-sdk.png)

Use the following connection string to access the **ctrlX Data Layer**:

    tcp://boschrexroth:boschrexroth@192.168.1.1

### Physical ctrlX CORE

The physical ctrlX CORE is connected to the host computer through its network adapter.

For example, use the following connection string to access the **ctrlX Data Layer**:

    tcp://boschrexroth:boschrexroth@10.52.244.123

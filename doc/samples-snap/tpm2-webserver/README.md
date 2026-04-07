# README tpm2.webserver

The sample app __tpm2.webserver__ shows the basic usage of a TPM2 inside your application.

Also see [TPM2](../../tpm2.md).

## Introduction

It demonstrates how you can use the TPM2.0 of the ctrlX CORE or the emulated TPM2.0 on the ctrlX CORE<sup>virtual</sup>.

* Include the TPM2 Trusted Software Stack (TSS) in your snap, as well as OpenSSL provider.
* Integrate your application into key and certificate management of the ctrlX CORE.
* Convert the TPM2 Key to a keyform usable with OpenSSL
* Use the TPM2 TSS OpenSSL engine with an example webserver

## Pre-requisites

* Recommended: ctrlX WORKS App Build Environment (QEMU Virtual Machine) - see Setup
* ctrlX AUTOMATION SDK Version 1.
* ctrlX CORE<sup>virtual</sup> or ctrlX CORE or other ctrlX OS Device with TPM

## Getting Started

1. Launch Visual Studio Code
2. Open the sample directory `samples-snap/tpm2-webserver`
3. Build and install the snap as described in `Setup` section. Use attached build scripts.
4. Open webbrowser and open the page: "https://\<ctrlX CORE IP>:44330"

You can also see the generated key and certificate in the settings page "Certificate & Keys" in Certificate store "TPM2 Webserver".

## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT

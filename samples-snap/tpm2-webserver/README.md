# README tpm2.consumer

The sample app __tpm2.consumer__ shows the basic usage of a TPM2 inside your application.

Also see [TPM2](../../tpm2.md).

## Introduction

It demonstrates how you can use the TPM2.0 of the ctrlX CORE or the emulated TPM2.0 on the ctrlX CORE^virtual^.

* Build the TPM2 Trusted Software Stack (TSS) including OpenSSL engine.
* Integrate your application into key and certificate management of the ctrlX CORE.
* Convert the TPM2 Key to a keyform usable with OpenSSL
* Use the TPM2 TSS OpenSSL engine with an example webserver

## Pre-requisites

* Recommended: ctrlX WORKS App Build Environment (QEMU Virtual Machine) - see Setup
* ctrlX AUTOMATION SDK Version 1.
* ctrlX CORE^virtual^ or ctrlX CORE

## Getting Started

1. Launch Visual Studio Code
2. Open the sample directory `samples-snap/tpm2-webserver`
3. Build and install the snap as described in `Setup` section
4. Go to the webinterface -> Settings -> Certificate & Keys -> TPM2 Webserver
5. Create a key named "webserver" as RSA 2048 and "HSM protected"
6. Create a certificate signing request (CSR), download
7. Create a certificate (webserver.crt) with help of the CSR on your PC or CA

```bash
# For example, create yourself a CA
mkdir ca
openssl genrsa -out ca/ca.key 2048
openssl req -new -x509 -days 10000 -key ca/ca.key -out ca/ca.crt
openssl x509 -req -days 360 -in webserver_csr.pem -CA ca/ca.crt -CAkey ca/ca.key -CAcreateserial -out webserver.crt -sha256
```

8. Upload the certificate webserver.crt the TPM2 Webserver application
9. Open webserver on Port 44330 of the ctrlX CORE or ctrlX CORE^virtual^ (adapt port settings accordingly).

## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

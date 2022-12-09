# README tpm2.consumer

The sample app __tpm2.consumer__ shows the basic usage of a TPM2 inside your application.

Also see [TPM2](../../tpm2.md).

## Introduction

It demonstrates how you can use the TPM2.0 of the ctrlX CORE or the emulated TPM2.0 on the ctrlX CORE<sup>virtual</sup>.

* Build the TPM2 Trusted Software Stack (TSS) including OpenSSL engine.
* Integrate your application into key and certificate management of the ctrlX CORE.
* Convert the TPM2 Key to a keyform usable with OpenSSL
* Use the TPM2 TSS OpenSSL engine with an example webserver

## Pre-requisites

* Recommended: ctrlX WORKS App Build Environment (QEMU Virtual Machine) - see Setup
* ctrlX AUTOMATION SDK Version 1.
* ctrlX CORE<sup>virtual</sup> or ctrlX CORE

## Getting Started

1. Launch Visual Studio Code
2. Open the sample directory `samples-sh/tpm2.consumer`
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
9. Open webserver on Port 44330 of the ctrlX CORE or ctrlX CORE<sup>virtual</sup> (adapt port settings accordingly).

## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

## License

MIT License

Copyright (c) 2021-2022 Bosch Rexroth AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

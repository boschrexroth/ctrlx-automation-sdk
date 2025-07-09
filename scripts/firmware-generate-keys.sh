#!/bin/bash
# Currently only P-384 ecdsa keys and SHA-384 are supported by DeviceAdmin for validiating firmware signatures
openssl ecparam -name secp384r1 -genkey -noout -out private.pem
openssl ec -in private.pem -pubout -out public.pem
openssl req -x509 -key private.pem  -out cert.pem -sha384 -days 3650
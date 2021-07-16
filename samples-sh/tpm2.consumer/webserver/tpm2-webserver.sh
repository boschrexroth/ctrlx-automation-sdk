#!/bin/bash

. $SNAP_DATA/system-configuration/certificate-manager/tpm2/envvars

CERTSTORE=$SNAP_COMMON/package-certificates/rexroth-tpm2-webserver/rexroth-tpm2-webserver

mkdir -p $CERTSTORE/ca/
mkdir -p $CERTSTORE/own/private/
mkdir -p $CERTSTORE/own/certs/
mkdir -p $CERTSTORE/rejected/private/
mkdir -p $CERTSTORE/rejected/certs/
mkdir -p $CERTSTORE/trusted/private/
mkdir -p $CERTSTORE/trusted/certs/

TSSKEY=$CERTSTORE/own/private/webserver.tpm2tss
TPM2KEY_PRIVATE=$CERTSTORE/own/private/webserver.priv
TPM2KEY_PUBLIC=$CERTSTORE/own/private/webserver.pub
CERT=$CERTSTORE/own/certs/webserver.crt

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SNAP/usr/lib/*/engines-1.1
export OPENSSL_ENGINES=$SNAP/usr/lib/*/engines-1.1

if [ ! -f "$TSSKEY" ]; then
    tpm2tss-genkey -P $TPM2_SRK_PARENT -u $TPM2KEY_PUBLIC -r $TPM2KEY_PRIVATE $TSSKEY
fi

openssl s_server -engine tpm2tss -keyform engine -key $TSSKEY -cert $CERT -accept 44330 -www
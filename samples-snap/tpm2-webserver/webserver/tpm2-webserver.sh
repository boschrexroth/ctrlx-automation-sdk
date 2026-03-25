#!/bin/bash

. $SNAP_DATA/system-configuration/certificate-manager/tpm2/envvars
# this will be part of tpm2/envvars in future releases
export TPM2OPENSSL_TCTI="$TPM2TOOLS_TCTI"

CERTSTORE=$SNAP_COMMON/package-certificates/sdk-tpm2-webserver/tpm2-webserver

mkdir -p $CERTSTORE/ca/
mkdir -p $CERTSTORE/own/private/
mkdir -p $CERTSTORE/own/certs/
mkdir -p $CERTSTORE/rejected/private/
mkdir -p $CERTSTORE/rejected/certs/
mkdir -p $CERTSTORE/trusted/private/
mkdir -p $CERTSTORE/trusted/certs/

TPM2KEY_PRIVATE=$CERTSTORE/own/private/webserver.priv
TPM2KEY_PUBLIC=$CERTSTORE/own/private/webserver.pub
TPM2KEY_CONTEXT=$CERTSTORE/own/private/webserver.ctx
CERT=$CERTSTORE/own/certs/webserver.crt

for dir in $SNAP/usr/lib/*/ossl-modules; do 
    export OPENSSL_MODULES=$dir
done

if [ ! -f "$CERT" ]; then
    # for simplicity, we use persistent keys here. avoid this whereever you can! TPM2 nvram is limited!
    tpm2_evictcontrol -Q -c 0x81000010 || true
    tpm2_create -u $TPM2KEY_PUBLIC -r $TPM2KEY_PRIVATE -C $TPM2_SRK_PARENT
    tpm2_load -u $TPM2KEY_PUBLIC -r $TPM2KEY_PRIVATE -C $TPM2_SRK_PARENT -c $TPM2KEY_CONTEXT
    tpm2_evictcontrol -c $TPM2KEY_CONTEXT 0x81000010
    openssl req -provider tpm2 -provider default -propquery '?provider=tpm2' -new -x509 -days 365 -key handle:0x81000010 -out $CERT -subj "/C=DE/ST=Bavaria/L=Lohr am Main/O=Bosch Rexroth/OU=IT/CN=tpm2-webserver"
fi

openssl s_server -provider tpm2 -provider default -propquery '?provider=tpm2' -key handle:0x81000010 -cert $CERT -accept 44330 -www
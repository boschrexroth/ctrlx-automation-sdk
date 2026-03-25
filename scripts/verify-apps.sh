#!/bin/bash
# shellcheck disable=SC2046
# shellcheck disable=SC2002

OEM_CERT=${1}
APPS_DIR=${2}

APP_PRIVILEGES_CERT="-----BEGIN CERTIFICATE-----
MIIDHTCCAsOgAwIBAgIUY6gHYsyoH1EMdYX3DdQhGdI34FkwCgYIKoZIzj0EAwIw
gaQxCzAJBgNVBAYTAkRFMQswCQYDVQQIDAJCWTEVMBMGA1UEBwwMTG9ociBhbSBN
YWluMRkwFwYDVQQKDBBCb3NjaCBSZXhyb3RoIEFHMQ4wDAYDVQQLDAVEQy1BRTEh
MB8GA1UEAwwYY29tLmN0cmx4LWF1dG9tYXRpb24uYXBwMSMwIQYJKoZIhvcNAQkB
FhRpbmZvQGJvc2NocmV4cm90aC5kZTAgFw0yNTExMjMwOTQxMDVaGA85OTk5MTIz
MTIzNTk1OVowga8xCzAJBgNVBAYTAkRFMQswCQYDVQQIDAJCWTEVMBMGA1UEBwwM
TG9ociBhbSBNYWluMRkwFwYDVQQKDBBCb3NjaCBSZXhyb3RoIEFHMQ4wDAYDVQQL
DAVEQy1BRTEsMCoGA1UEAwwjY29tLmN0cmx4LWF1dG9tYXRpb24uYXBwLnByaXZp
bGVnZXMxIzAhBgkqhkiG9w0BCQEWFGluZm9AYm9zY2hyZXhyb3RoLmRlMFkwEwYH
KoZIzj0CAQYIKoZIzj0DAQcDQgAEqQw6812wDx805gY3hKCzGiqXOwkng8VRS3O/
e3YNjLdOjsD3vufeFefsxfm2m5oyultX2PTp0R9YLaGgHt3QXqOBwzCBwDAMBgNV
HRMBAf8EAjAAMB0GA1UdDgQWBBQvOqdTA2rEoPSGz4xTxSgj33XelzAfBgNVHSME
GDAWgBTaCP+uOXJDlMTiA1IUEheGCbDEPzAOBgNVHQ8BAf8EBAMCB4AwYAYDVR0f
BFkwVzBVoFOgUYZPaHR0cDovL2JpY3MubWNnLmVzY3J5cHQuY29tLzE3Ni9jcmw/
aWQ9ZGEwOGZmYWUzOTcyNDM5NGM0ZTIwMzUyMTQxMjE3ODYwOWIwYzQzZjAKBggq
hkjOPQQDAgNIADBFAiAq7TVyaBBQFZGzViKbdtzCjY6iaK09ArlAlgNtQPp64wIh
AKfo59UdcupA8FSkaMmEk6y1yd+zjaqOM/uWgR+gSftj
-----END CERTIFICATE-----"

WORKINGDIR=$APPS_DIR/verify_temp
GREEN='\033[0;32m'
NC='\033[0m'

for app_file in "$APPS_DIR"/*.app; do
  echo -e "\n=> Verifying $(basename "$app_file")"
  # get app name
  rm -rf "$WORKINGDIR" && mkdir -p "$WORKINGDIR"
  cp "$app_file" "$WORKINGDIR"/

  tar --wildcards --directory "$WORKINGDIR"/ -xf "$WORKINGDIR"/$(basename "$app_file") public/snaps/*/release/*.{snap,signature} -C "$WORKINGDIR"

  for d in "$WORKINGDIR"/public/snaps/*; do
    archdir=$(basename "$d") && echo "$archdir:" && echo -n "app-revision: "
    
    # Split *.signature file into blocks separated by empty lines
    mkdir -p "$WORKINGDIR"/"$archdir"
    csplit -s -z -f "$WORKINGDIR"/"$archdir"/block_ "$WORKINGDIR"/public/snaps/"$archdir"/release/*.signature '/^$/' '{*}'

    # Block 1: app-revision content
    mv "$WORKINGDIR"/"$archdir"/block_00 "$WORKINGDIR"/"$archdir"/app-revision.content

    # Block 2: app-revision signature
    mv "$WORKINGDIR"/"$archdir"/block_01 "$WORKINGDIR"/"$archdir"/app-revision.signature

    # Block 3: app-privileges content (optional)
    if [ -f "$WORKINGDIR"/"$archdir"/block_02 ]; then
      # Remove leading empty lines before moving
      sed '/./,$!d' "$WORKINGDIR"/"$archdir"/block_02 > "$WORKINGDIR"/"$archdir"/app-privileges.content
    fi

    # Block 4: app-privileges signature (optional)
    if [ -f "$WORKINGDIR"/"$archdir"/block_03 ]; then
      mv "$WORKINGDIR"/"$archdir"/block_03 "$WORKINGDIR"/"$archdir"/app-privileges.signature
    fi

    # validate origin
    openssl base64 -d < "$WORKINGDIR"/"$archdir"/app-revision.signature > "$WORKINGDIR"/"$archdir"/app-revision.signature.dat
    openssl x509 -pubkey -noout -in "$OEM_CERT" > "$WORKINGDIR"/signing-key.pub
    openssl dgst -sha384 -verify "$WORKINGDIR"/signing-key.pub -signature "$WORKINGDIR"/"$archdir"/app-revision.signature.dat "$WORKINGDIR"/"$archdir"/app-revision.content || exit 1

    # validate privileges signature if exists
    if [ -f "$WORKINGDIR"/"$archdir"/app-privileges.signature ]; then
      echo -n "app-privileges: "
      echo "$APP_PRIVILEGES_CERT" > "$WORKINGDIR"/app-privileges-cert.pem
      openssl x509 -pubkey -noout -in "$WORKINGDIR"/app-privileges-cert.pem > "$WORKINGDIR"/app-privs-key.pub
      openssl base64 -d < "$WORKINGDIR"/"$archdir"/app-privileges.signature > "$WORKINGDIR"/"$archdir"/app-privileges.signature.dat
      openssl dgst -sha384 -verify "$WORKINGDIR"/app-privs-key.pub -signature "$WORKINGDIR"/"$archdir"/app-privileges.signature.dat "$WORKINGDIR"/"$archdir"/app-privileges.content || exit 1
    fi

    # print status (errors have been caught by 'exit 1' above)
    echo -e "${GREEN}Signature verified${NC}"

  done
done 

rm -rf "$WORKINGDIR"

echo && echo -e "${GREEN}===> All signature verifications successful${NC}"
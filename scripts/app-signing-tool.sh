#!/bin/bash

OEM_CERT=$1
SIGNING_KEY=$2
DIR_SNAPS_INPUT=$3
DIR_APPS_OUTPUT=$"${4:-$DIR_SNAPS_INPUT}"

rm -rf $DIR_SNAPS_INPUT/signing_temp && mkdir -p $DIR_SNAPS_INPUT/signing_temp

# get publisher certificate
openssl x509 -in $OEM_CERT -outform DER | base64 > $DIR_SNAPS_INPUT/signing_temp/cert.der.base64
publisher=$(openssl x509 -noout -subject -nameopt multiline -in $OEM_CERT | grep commonName | awk '{print $3}')
organization=$(openssl x509 -noout -subject -nameopt multiline -in $OEM_CERT | grep organizationName | awk '{print $3}')

for snap_file in $DIR_SNAPS_INPUT/*.snap; do
  SIGNING_DIR=$DIR_SNAPS_INPUT/signing_temp/sign_$(basename $snap_file) && mkdir -p "$SIGNING_DIR"

  # Expected snap nameing scheme: <APPNAME>_<VERSION>_<ARCHITECTURE>.snap
  snapfilename=$(basename $snap_file)
  snapfilebase=${snapfilename%_*}
  app_name=${snapfilebase%_*}
  appFileName=$(basename "${snapfilename%_*}.app")
  arch=$(echo $snapfilename | cut -d'_' -f3 | cut -d'.' -f1)

  # generate digest of snap
  snap_digest=$(sha384sum $snap_file | awk '{ print $1 }')

  echo "+----< Singing snap file: $snapfilename"
  echo -e "| arch:        $arch \n| digest:      $snap_digest \n| destination: $DIR_APPS_OUTPUT/$appFileName\n"

  # create signature file
  export SIGNING_DIR snap_digest app_name publisher organization timestamp=$(date -u +"%Y-%m-%dT%H:%M:%S.%6NZ")
  mkdir -p $SIGNING_DIR/public/snaps/$arch/release
  ( echo "cat <<EOF >$SIGNING_DIR/public/snaps/$arch/release/$snapfilebase.signature";
    echo 'type:                   app-revision';
    echo 'app-name:               ${app_name}';
    echo 'publisher:              ${publisher}';
    echo 'organization:           ${organization}';
    echo 'timestamp:              ${timestamp}';
    echo 'snap-digest:            ${snap_digest}';
  ) >$SIGNING_DIR/temp.yml && source $SIGNING_DIR/temp.yml 2> /dev/null

  # generate signature of assertion
  SIG=$(openssl dgst -sha384 -sign $SIGNING_KEY $SIGNING_DIR/public/snaps/$arch/release/$snapfilebase.signature | openssl base64)
  echo -e "\n$SIG" >> $SIGNING_DIR/public/snaps/$arch/release/$snapfilebase.signature

  # repack
  cp $snap_file $SIGNING_DIR/public/snaps/$arch/release/$snapfilebase.snap
  tar --append --file=$DIR_SNAPS_INPUT/signing_temp/$appFileName --directory=$SIGNING_DIR public/
  rm -r $SIGNING_DIR
done
for appfile in $DIR_SNAPS_INPUT/signing_temp/*.app; do
  mv $appfile $DIR_APPS_OUTPUT && echo -e "-> Signed app: $DIR_APPS_OUTPUT/$(basename $appfile)"
done
  rm -r $DIR_SNAPS_INPUT/signing_temp
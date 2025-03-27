#!/bin/bash

OEM_CERT=$1
APPS_DIR=$2

WORKINGDIR=$APPS_DIR/verify_temp
GREEN='\033[0;32m'
NC='\033[0m'

for app_file in $APPS_DIR/*.app; do
  echo -e "\n=> Verifying $(basename $app_file)"
  # get app name
  rm -rf $WORKINGDIR && mkdir -p $WORKINGDIR
  cp $app_file $WORKINGDIR/
  tar --wildcards --directory $WORKINGDIR/ -xf $WORKINGDIR/$(basename $app_file) public/snaps/*/release/*.{snap,signature} -C $WORKINGDIR

  for d in $WORKINGDIR/public/snaps/*; do
    archdir=$(basename $d) && echo "$archdir:"
    
    # validate signature of file
    mkdir -p $WORKINGDIR/$archdir
    head -n -3 $WORKINGDIR/public/snaps/$archdir/release/*.signature > $WORKINGDIR/$archdir/content
    tail -2 $WORKINGDIR/public/snaps/$archdir/release/*.signature > $WORKINGDIR/$archdir/signature

    # validate origin
    openssl base64 -d < $WORKINGDIR/$archdir/signature > $WORKINGDIR/$archdir/signature.dat
    openssl x509 -pubkey -noout -in $OEM_CERT > $WORKINGDIR/signing-key.pub
    openssl dgst -sha384 -verify $WORKINGDIR/signing-key.pub -signature $WORKINGDIR/$archdir/signature.dat $WORKINGDIR/$archdir/content || exit 1

    # get snap digest and compare
    snap_digest_orig=$(cat $WORKINGDIR/$archdir/content | grep snap-digest | awk '{print $2}')
    snap_digest=$(sha384sum $WORKINGDIR/public/snaps/$archdir/release/*.snap | awk '{ print $1 }')

    if [ "$snap_digest_orig" == "$snap_digest" ]; then
      echo -e "${GREEN}Signature verified${NC}"
    else
      echo "Signature verification failed"
      exit 1
    fi

  done
done 

rm -rf $WORKINGDIR

echo -e "\n${GREEN}===> All signature verifications successful${NC}"
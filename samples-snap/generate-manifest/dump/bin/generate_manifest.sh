#!/bin/bash -x

# Update json
NAME=$(shuf -n2  $SNAP/usr/share/dict/words | tr '\n' ' ')
mkdir -p $SNAP_DATA/package-assets/$SNAP_NAME
$SNAP/usr/bin/jq ".menus.sidebar[].title = \"$NAME\"" \
    $SNAP/package-assets/sdk-generate-manifest.package-manifest.json.template > $SNAP_DATA/package-assets/sdk-generate-manifest/sdk-generate-manifest.package-manifest.json

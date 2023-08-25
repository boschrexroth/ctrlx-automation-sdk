#!/usr/bin/env bash

# Pack all files under scripts/ into a zip archive and insert this file
# into the cloud-config files as gzip base64 encoded text.

# scripts.zip handling ----------------------------------------------------
echo "___________________________________________________________"
rm scripts.zip
mkdir -p scripts/cloud-config/ && cp cloud-config scripts/cloud-config/

echo "Creating scripts.zip ..."

# Add all files into one zip archive
zip -q -r scripts.zip scripts/

# Create search and replace text --------------------------------------------
SEARCH_ZIP=insert-scripts.zip-here
SEARCH_SH=insert-check-cloud-init-log.sh-here

# Cloud-init function write_files supports gzip format as base64 encoded text, so these steps are necessary:
# 1. Convert zip archive into gzip format.
# 2. Convert the gzip (binary) into base64 encoded text.
REPLACE_ZIP=$(gzip -c scripts.zip | base64 --wrap=0)
REPLACE_SH=$(base64 --wrap=0 check-cloud-init-log.sh)

# amd64 handling -----------------------------------------------------------------
ARCH=amd64
ARCH_REMOVE=aarch64
# Remove these lines
grep -v "#${ARCH_REMOVE}-ONLY" cloud-config >cloud-config-${ARCH}-generated
# Insert base64 encoded content
python3 replace-text.py cloud-config-${ARCH}-generated $SEARCH_ZIP $REPLACE_ZIP
python3 replace-text.py cloud-config-${ARCH}-generated $SEARCH_SH $REPLACE_SH
# Remove flags
python3 replace-text.py cloud-config-${ARCH}-generated "#${ARCH}-ONLY" " "

# aarch64 (arm64) handling -----------------------------------------------------------------
ARCH=aarch64
ARCH_REMOVE=amd64
# Remove these lines
grep -v "#${ARCH_REMOVE}-ONLY" cloud-config >cloud-config-${ARCH}-generated
# Insert base64 encoded content
python3 replace-text.py cloud-config-${ARCH}-generated $SEARCH_ZIP $REPLACE_ZIP
python3 replace-text.py cloud-config-${ARCH}-generated $SEARCH_SH $REPLACE_SH

# Remove flags
python3 replace-text.py cloud-config-${ARCH}-generated "#${ARCH}-ONLY" " "

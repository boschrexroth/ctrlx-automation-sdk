#!/usr/bin/env bash

# Pack all files under scripts/ into a zip archive and insert this file
# into the cloud-config files as gzip base64 encoded text.

# archive.zip handling ----------------------------------------------------
echo "___________________________________________________________"
rm archive.zip 2>/dev/null
mkdir -p cloud-init/ && cp cloud-config cloud-init/

echo "Creating archive.zip ..."

# Add all files into one zip archive
zip -q -r archive.zip install-sdk-from-github.sh check-cloud-init-log.sh cloud-init/

# Create search and replace text --------------------------------------------
SEARCH_ZIP=insert-archive.zip-here
REPLACE_ZIP=$(gzip -c archive.zip | base64 --wrap=0)

# amd64 handling -----------------------------------------------------------------
ARCH=amd64
ARCH_REMOVE=aarch64
# Remove these lines
grep -v "#${ARCH_REMOVE}-ONLY" cloud-config >cloud-config-${ARCH}-generated
# Insert base64 encoded content
python3 replace-text.py cloud-config-${ARCH}-generated $SEARCH_ZIP $REPLACE_ZIP
# Remove flags
python3 replace-text.py cloud-config-${ARCH}-generated "#${ARCH}-ONLY" " "

# aarch64 (arm64) handling -----------------------------------------------------------------
ARCH=aarch64
ARCH_REMOVE=amd64
# Remove these lines
grep -v "#${ARCH_REMOVE}-ONLY" cloud-config >cloud-config-${ARCH}-generated
# Insert base64 encoded content
python3 replace-text.py cloud-config-${ARCH}-generated $SEARCH_ZIP $REPLACE_ZIP

# Remove flags
python3 replace-text.py cloud-config-${ARCH}-generated "#${ARCH}-ONLY" " "

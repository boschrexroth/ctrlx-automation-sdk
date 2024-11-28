#!/bin/bash

TAG=${1}
echo "TAG=${TAG}"

SDK_TMP_DIR=/tmp/sdk
SDK_TMP_FILE=/tmp/sdk.zip

rm -fr ${SDK_TMP_DIR}
rm -f ${SDK_TMP_FILE}

wget_output=$(wget --tries=2 --timeout=60 https://github.com/boschrexroth/ctrlx-automation-sdk/releases/download/${TAG}/ctrlx-automation-sdk-${TAG}.zip -O ${SDK_TMP_FILE})
if [ $? -ne 0 ]; then
    echo "ctrlx-automation-sdk-${TAG}.zip not found"
    exit 0
fi

unzip ${SDK_TMP_FILE} -d ${SDK_TMP_DIR}

if [ ! -d "${SDK_TMP_DIR}/ctrlx-automation-sdk/doc/api" ]; then
    echo "doc/api not found"
    exit 0
fi

mv "${SDK_TMP_DIR}/ctrlx-automation-sdk/doc/api" "doc/api"
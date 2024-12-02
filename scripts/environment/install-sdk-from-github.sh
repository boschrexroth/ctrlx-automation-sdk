#!/usr/bin/env bash

echo " "
echo "============================================"
echo Installing ctrlX AUTOMATION SDK from github
echo "============================================"
echo " "

function WGetFile ()
{
    SUB_DIR=$1
    FILE=$2
    rm ~/scripts/$FILE* 2>/dev/null
    wget https://raw.githubusercontent.com/boschrexroth/ctrlx-automation-sdk/main/$SUB_DIR/$FILE -P ~/scripts
    chmod 777 ~/scripts/$FILE
}

function WGetMdAndShFiles ()
{
    SUB_DIR=$1
    # Get JSON object with list of files (items)
    TREE=$(curl https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/$SUB_DIR | jq .payload.tree)
    
    # Debug
    # echo $TREE | jq -r '.items[]'

    # https://stackoverflow.com/questions/68121082/how-to-iterate-over-json-array-with-jq
    # Iterate through list and download each file
    echo $TREE | jq -r '.items[].name' | while read name ; do
        case $name in 
            *.md) 
                WGetFile $SUB_DIR $name
                ;; 
            *.sh) 
                WGetFile $SUB_DIR $name
                ;; 
        esac
    done
}

WGetMdAndShFiles scripts/environment/scripts
WGetMdAndShFiles scripts

source ~/scripts/install-sdk.sh



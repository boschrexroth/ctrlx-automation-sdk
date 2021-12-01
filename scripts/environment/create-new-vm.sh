#!/usr/bin/env bash

if [ -z $1 ]
then
      echo "Parameter 1 (destination path) is empty"
      exit 1
fi

if [ -z $2 ]
then
      echo "Parameter 2 (destination path) is empty"
      exit 1
fi

# Create destination directory
mkdir -p "$1" 

# Install neccessary files
rsync --files-from=$2 ./ "$1"/
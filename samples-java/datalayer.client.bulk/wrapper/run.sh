#!/bin/bash

pushd "$SNAP/jar" || return

#export LD_LIBRARYPATH
#export java.lib.path
#java -jar app.jar

java -cp "./*" datalayer.client.bulk.App

popd || return

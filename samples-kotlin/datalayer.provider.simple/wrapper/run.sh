#!/bin/bash

pushd "$SNAP/jar" || return

# AppKt is the JVM class generated from the top-level main() in App.kt
java -cp "./*" datalayer.provider.simple.AppKt

popd || return

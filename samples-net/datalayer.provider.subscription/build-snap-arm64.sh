#!/usr/bin/env bash
set -e
TARGET_DOTNET=linux-arm64
PUBLISH_DIR=./publish

# Clean publish dir
rm -rf ${PUBLISH_DIR}

# Run dotnet publish to generate self-contained .NET executable with given runtime.
dotnet publish --configuration Release --runtime ${TARGET_DOTNET} --property:PublishDir=${PUBLISH_DIR} --self-contained true /property:GenerateFullPaths=true /property:PublishSingleFile=true /property:PublishTrimmed=true /property:DebugType=None /property:DebugSymbols=false

# Build snap
../../scripts/build-snap-arm64.sh
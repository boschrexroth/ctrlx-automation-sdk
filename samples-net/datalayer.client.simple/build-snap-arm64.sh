#!/usr/bin/env bash
set -e
TARGET_DOTNET=linux-arm64
TARGET_ARCH=arm64

# Remove ./publish/${TARGET_ARCH} folder of last dotnet publish call
rm -rfv ./publish/${TARGET_ARCH}

# Run dotnet publish to generate self-contained .NET executable with given runtime.
dotnet publish --configuration Release --runtime ${TARGET_DOTNET} --output ./publish/${TARGET_ARCH} --self-contained true /property:GenerateFullPaths=true /property:PublishSingleFile=true /property:PublishTrimmed=true /property:DebugType=None /property:DebugSymbols=false

# Clean all snap part assets
snapcraft clean --destructive-mode

# Build snap without separate container
snapcraft --destructive-mode --enable-experimental-target-arch --target-arch ${TARGET_ARCH}
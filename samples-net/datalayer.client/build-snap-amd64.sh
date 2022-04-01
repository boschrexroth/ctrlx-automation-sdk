#!/usr/bin/env bash
TARGET_DOTNET=linux-x64
TARGET_ARCH=amd64

# Run dotnet publish to generate self-contained .NET executable with given runtime.
dotnet publish --configuration Release --runtime ${TARGET_DOTNET} --output ./publish/${TARGET_ARCH} --self-contained true /property:GenerateFullPaths=true /property:PublishSingleFile=true /property:PublishTrimmed=true /property:DebugType=None /property:DebugSymbols=false

# Clean all snap part assets
snapcraft clean --destructive-mode

# Build snap without separate container
snapcraft --destructive-mode --enable-experimental-target-arch --target-arch ${TARGET_ARCH}
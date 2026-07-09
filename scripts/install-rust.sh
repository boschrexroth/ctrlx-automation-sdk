#!/usr/bin/env bash

RUST_CHANNEL="1.95.0"

echo " "
echo "============================================"
echo "Installing rust '$RUST_CHANNEL'..."
echo "============================================"
echo " "

sudo apt install -y clang

sudo snap install rustup --classic

rustup toolchain install "${RUST_CHANNEL}"
rustup default "${RUST_CHANNEL}"

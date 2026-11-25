#!/bin/bash

# Prerequisite: VS Code installed
# Script must be executed in the VS Code terminal.

# Export
# code --list-extensions | xargs -L 1 echo code --install-extension
# code --list-extensions > extensions.list
# cat extensions.list |% { code --install-extension $_}

extensions=(
"mhutchie.git-graph" \
"ms-vscode.cmake-tools" \
"ms-vscode.cpptools" \
"ms-vscode.cpptools-extension-pack" \
"ms-vscode.cpptools-themes" \
"ms-vscode.makefile-tools" \
"rust-lang.rust-analyzer" \
"timonwong.shellcheck" \
"twxs.cmake" \
"editorconfig.editorconfig" \
"vadimcn.vscode-lldb"
)

for ext in "${extensions[@]}"
do
  echo "Extension: ${ext}"
  code --install-extension "${ext}"
done
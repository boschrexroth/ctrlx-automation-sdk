#!/bin/bash


# Script must be executed in the VCode terminal.

# Export
# code --list-extensions | xargs -L 1 echo code --install-extension
# code --list-extensions > extensions.list
# cat extensions.list |% { code --install-extension $_}

extensions=(
"davidanson.vscode-markdownlint" \
"mhutchie.git-graph" \
"timonwong.shellcheck" \
"editorconfig.editorconfig" \
"vscjava.vscode-gradle" \
"vscjava.vscode-java-debug" \
"vscjava.vscode-java-dependency" \
"vscjava.vscode-java-pack" \
"vscjava.vscode-java-test" \
"vscjava.vscode-maven" \
"madhavd1.javadoc-tools"
)

for ext in "${extensions[@]}"
do
  echo "Extension: ${ext}"
  code --install-extension "${ext}"
done
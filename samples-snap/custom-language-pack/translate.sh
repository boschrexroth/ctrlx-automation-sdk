#!/bin/bash
# Important: Install json-translator first: npm i -g @parvineyvazov/json-translator
# Please see: https://github.com/mololab/json-translator for more information.
# Important: Jsontt translator does not work inside Bosch-Proxy (and possibly other proxy). 

# Check if the folder argument is provided
if [ -z "$1" ]; then
  echo "Please provide the folder as an argument."
  exit 1
fi

# Change to the specified folder
cd "$1" || exit 1
# Interate over all files in directory and translate it using jsontt
for file in *.json; do
  printf "$file \n"

  #https://github.com/mololab/json-translator
  ~/npm/bin/jsontt $file -m google2 -f en -t fr -fb no -cl 3 -c true -n ../../../language-files/fr/$file 
done
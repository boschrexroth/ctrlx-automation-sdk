#!/bin/bash

echo "============================================"
echo "Installing ctrlx-datalayer"
echo "============================================"
# --- Install ctrlx-datalayer ---
bash ./install-ctrlx-datalayer.sh

if [ $? -eq 0 ]; then
    echo "============================================"
    echo "ctrlx-datalayer installed successfully"
    echo "============================================"
else
    echo "============================================"
    echo "Warning: The ctrlx-datalayer install script exited with an error"
    echo "============================================"
fi
echo "============================================"
echo "Installing ctrlx-scheduler"
echo "============================================"
# --- Install ctrlx-scheduler ---
sudo apt-get install ctrlx-scheduler -y
pwd
echo "============================================"
echo "All tasks complete."
echo "============================================"


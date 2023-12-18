This document describes how to work with App Build Environments (QEMU based VMs).

## Best Practice working with App Build Environments

We recommend to use App Build Environments (QEMU VMs) as follows:

1. Create a new App Build Environment with ctrlX WORKS and start it (the Ubuntu Server Cloud Image is automatically downloaded).
2. After first boot login and install only necessary additional software components using the install scripts in /home/boschrexroth/e.g.: install-sdk.sh, install-deb.sh and install-go.sh
3. We recommend to work with the  free and open source distributed version control system __git__: Create a repository for your project and clone it into your App Buiulder Environment.
4. Create a new branch for your working package.
5. Do your work within this branch, commit and push your changes as often as it makes sense.
6. If your work is done and all changes are pushed remove your App Build Environment.

__Working in such a manner ensures that you are always working with the latest software components and avoids problems with the file size of your App Build Environment. Furthermore creating backups is superfluous.__

For more information about file handling regarding the App Build Environment see [ctrlX App Build Environment Ubuntu 22.04](scripts/environment/builder/README.md).


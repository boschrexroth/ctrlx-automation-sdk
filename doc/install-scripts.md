The directory __/home/boschrexroth/scripts/__ of an App Build Environment contains a set of important scripts.

In the file `/home/boschrexroth/scripts/README.md` all files are listed and described. 

__Hint:__ The ctrlX AUTOMATION SDK contains these scripts also in the folder scripts/environment/scripts.

## Some important scripts

### install-sdk.sh

!!! important
    Run this script first. 
    
In addition to the ctrlX AUTOMATION SDK, the ctrlx-datalayer debian package, snapcraft and libraries for cross build are installed.

    `sh -c "$(curl -fsSL https://raw.githubusercontent.com/boschrexroth/ctrlx-automation-sdk/main/scripts/clone-install-sdk.sh)"`

Now the directory ctrlx-automation-sdk/ contains the files of the ctrlX AUTOMATION SDK.

### install-go, -dotnet, -nodejs

These scripts are installing programming language dependend packages to be able to build apps in the according language.

### install-ctrlx-os-dev-tools.sh

This script installs packages needed to build a ctrlX CORE image.

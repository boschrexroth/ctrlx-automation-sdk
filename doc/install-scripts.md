An App Build Environment instance created by ctrlX WORKS or based on the scripts under the SDK folder public/scripts/environment has a set of important install scripts in the directory __/home/boschrexroth/.__

## install-sdk.sh

!!! important
    Starting this script must be the first step using a new App Build Environment. 

This script downloads the ctrlX AUTOMATION SDK archive from github, extracts the content into the actual directory, sets some x permission flags and installs the ctrlx-datalayer debian package.

Also snapcraft is installed because in the initialization phase during the first boot this snap cannot be installed.

Here the archive is directly installed into the home directory:

    cd 
    ./install-sdk.sh
    cd ctrlx-automation-sdk

## install-deb.sh

!!! important
    Calling this script once is mandatory for architectural cross-builds of C++ apps.

    cd
    ./install-deb.sh
    
Important libraries for the Data Layer communication are installed for both amd64 and arm64 (aarch64) architecture.

## install-go.sh

If you want to develop apps in golang call this script:

    cd
    ./install-go.sh

## install-dotnet.sh

If you want to develop apps in C# call this script:

    cd
    ./install-dotnet.sh

## install-nodejs-npm.sh

If you want to develop apps in Node.js call this script:

    cd
    ./install-nodejs-npm.sh

The Node.js snap inclusively the Node Package Manager npm ist installed.

## install-snapcraft.sh

This script installs snapcraft, it is called implicitly by the other scripts.
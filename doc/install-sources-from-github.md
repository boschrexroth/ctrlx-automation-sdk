## Introduction

If you want to contribute the ctrlX AUTOMATION SDK you have to clone the github repository https://github.com/boschrexroth/ctrlx-automation-sdk.git.

After this step you have to merge the runtime content of ths ctrlX AUTOMATION SDKs zip archive into this repo.

These jobs can be initiated by __install-sdk-from-github.sh__ located in the ctrlX AUTOMATION SDK folder __scripts/__

## Installing a Runnable Repository

* Open the github site [https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/scripts)
* Download the script install-sdk-from-github.sh
* Install the script e.g. in the home directory of your App Build Environment
* Create a working directory e.g. ~/github
* Change into this directory and call the script

    ~/install-sdk-from-github.sh

When the script is finished the ctrlX AUTOMATION SDK is installed into the directory ctrlx-automation-sdk and sample projects can be build at once.

## Build Sample Project

To build a sample project do following steps:

Change directory e.g.: 

    cd ctrlx-automation-sdk/public/samples-cpp/datalayer.provider.all-data


Build snap: 

    build-snap-amd64.sh


## Troubleshooting

github grants only a a limited number of accesses to its repositories. If you are behind a proxy this number can be exceeded very fast. In this case the download of the zip archive fails.

To avoid this you need a github account and a personal access token - see [Creating a personal access token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token)

Open the script install-sdk-from-github.sh search the line beginning with LATEST=$(curl -s and add this information:

From

    LATEST=$(curl -s ...

change to e.g.

    LATEST=$(curl -s -u usrxyz:tokenxyz ...




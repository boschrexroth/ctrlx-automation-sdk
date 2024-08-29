## Introduction

Follow the installation instructions of [ctrlX AUTOMATION SDK](https://github.com/boschrexroth/ctrlx-automation-sdk.git).

## Build Sample Project

To build a sample project do following steps:

Change directory e.g.:

    cd ~/ctrlx-automation-sdk/samples-cpp/datalayer.provider.all-data

Build snap:

    ./build-snap-amd64.sh

## Troubleshooting

github grants only a a limited number of accesses to its repositories. If you are behind a proxy this number can be exceeded very fast. In this case the download of the zip archive fails.

To avoid this you need a github account and a personal access token - see [Creating a personal access token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token)

Open the script install-sdk-from-github.sh search the line beginning with LATEST=$(curl -s and add this information:

From

    LATEST=$(curl -s ...

change to e.g.

    LATEST=$(curl -s -u usrxyz:tokenxyz ...

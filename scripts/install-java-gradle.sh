#!/bin/bash

GRADLE_VERSION="9.2.1"

GRADLE_DIR="oss.gradle"
GRADLE_INSTALL_DIR="../${GRADLE_DIR}"

SCRIPTS_SAMPLES_JAVA_DIR="../scripts/samples-java"

## Java
sudo apt-get update
sudo apt-get -y install openjdk-21-jdk-headless wget


## Gradle
if [ ! -f "${GRADLE_INSTALL_DIR}/gradle-${GRADLE_VERSION}-bin.zip" ]; then
    echo "nein"
    mkdir -p ${GRADLE_INSTALL_DIR}

    pushd ${GRADLE_INSTALL_DIR} || return
        wget https://services.gradle.org/distributions/gradle-${GRADLE_VERSION}-bin.zip
    popd || return
fi

JAVA_DIR=$(pwd)
#echo "JAVA_DIR1=${JAVA_DIR}"
pushd "../java" || return
    JAVA_DIR=$(pwd)
popd || return

echo "JAVA_DIR2=${JAVA_DIR}"

## Examples

pushd "../samples-java" || return

JAVA_EXAMPLES_DIRS=$(find "./" -maxdepth 1 -type d)
echo "$JAVA_EXAMPLES_DIRS"

for dir in ${JAVA_EXAMPLES_DIRS}; do

    if [ "${dir}" == "./" ] ; then
        continue
    fi
    ## gradle to the java projects
    cp -f "${GRADLE_INSTALL_DIR}/"gradle-${GRADLE_VERSION}-bin.zip "${dir}"/gradle/wrapper/
    ## Java data layer API as local storage
    rm -rf "${dir}/java"
    ln -sf "${JAVA_DIR}" "${dir}"

    cp -f "${SCRIPTS_SAMPLES_JAVA_DIR}/"libs.versions.toml "${dir}"/gradle/

done

popd || return


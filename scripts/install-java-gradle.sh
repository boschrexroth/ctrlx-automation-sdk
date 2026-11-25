#!/usr/bin/env bash
#
# Installs the Java JDK (LTS) and the Gradle Build Tool, and prepares the
# Java/Kotlin sample projects:
#   * copies the Gradle distribution zip into each project's gradle wrapper
#   * symlinks the local Java data-layer API into each project
#   * copies the shared libs.versions.toml into each project
#
# Exit immediately if a command exits with a non-zero status.
set -euo pipefail

readonly GRADLE_VERSION="9.2.1"
readonly JDK_PACKAGE="openjdk-21-jdk-headless"

# Resolve script directory so the script can be called from anywhere.
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
PUBLIC_DIR=$(cd -- "${SCRIPT_DIR}/.." && pwd)
readonly SCRIPT_DIR PUBLIC_DIR

readonly GRADLE_INSTALL_DIR="${PUBLIC_DIR}/oss.gradle"
readonly GRADLE_ZIP="${GRADLE_INSTALL_DIR}/gradle-${GRADLE_VERSION}-bin.zip"
readonly GRADLE_URL="https://services.gradle.org/distributions/gradle-${GRADLE_VERSION}-bin.zip"

readonly JAVA_API_DIR="${PUBLIC_DIR}/java"
readonly LIBS_VERSIONS_TOML="${SCRIPT_DIR}/samples-java/libs.versions.toml"

readonly SAMPLES_ROOTS=(
    "${PUBLIC_DIR}/samples-java"
    "${PUBLIC_DIR}/samples-kotlin"
)

banner() {
    echo " "
    echo "============================================"
    echo "$1"
    echo "============================================"
    echo " "
}

install_jdk() {
    banner "Installing ${JDK_PACKAGE} and wget"
    sudo apt-get update
    sudo apt-get -y install "${JDK_PACKAGE}" wget
}

download_gradle() {
    banner "Downloading Gradle ${GRADLE_VERSION}"
    if [[ -f "${GRADLE_ZIP}" ]]; then
        echo "Gradle distribution already present: ${GRADLE_ZIP}"
        return
    fi
    mkdir -p "${GRADLE_INSTALL_DIR}"
    wget -O "${GRADLE_ZIP}" "${GRADLE_URL}"
}

# Prepare a single sample project:
#   * copy the gradle distribution into the wrapper directory
#   * replace dir/java with a symlink to the local Java data-layer API
#   * copy the shared libs.versions.toml
#   * restore execute permission on gradlew (lost by cp)
prepare_sample_project() {
    local project_dir="$1"
    echo "Preparing: ${project_dir}"

    cp -f "${GRADLE_ZIP}" "${project_dir}/gradle/wrapper/"

    rm -rf "${project_dir}/java"
    ln -sf "${JAVA_API_DIR}" "${project_dir}/java"

    cp -f "${LIBS_VERSIONS_TOML}" "${project_dir}/gradle/"

    chmod +x "${project_dir}/gradlew"
}

prepare_samples_in() {
    local samples_root="$1"
    if [[ ! -d "${samples_root}" ]]; then
        echo "Skipping (not found): ${samples_root}"
        return
    fi

    banner "Preparing sample projects in ${samples_root}"
    local project_dir
    for project_dir in "${samples_root}"/*/; do
        [[ -d "${project_dir}" ]] || continue
        prepare_sample_project "${project_dir%/}"
    done
}

main() {
    install_jdk
    download_gradle
    echo "JAVA_API_DIR=${JAVA_API_DIR}"
    for samples_root in "${SAMPLES_ROOTS[@]}"; do
        prepare_samples_in "${samples_root}"
    done
}

main "$@"

# Samples Rust

## Basic Samples

* [ctrlX Data Layer Client](./datalayer.client.simple/README.md)
* [ctrlX Data Layer Client Subscription](./datalayer.client.sub/README.md)
* [ctrlX Data Layer Client Bulk](./datalayer.client.bulk/README.md)
* [ctrlX Data Layer Provider](./datalayer.provider.simple/README.md)
* [ctrlX Data Layer Provider Subscription](./datalayer.provider.sub/README.md)

## Prerequisites

### Rust & Cargo

Rustup and Rust/Cargo used for development.

**Rust** and **Cargo** are installed using the following script as snap version.

```bash
    cd ctrlx-automation-sdk/scripts
    ./install-rust.sh
```

Or to start using Rust, download the installer, then run the program and follow the onscreen instructions.

```bash
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

Cargo Builds

For a simple cargo build, it is recommended that you use the script. Library paths ('crate') are set here.
With a ctrlX Data Layer library update, it is recommended to delete this directory manually.

```bash
    ./build-cargo.sh
```

**Hint:** Error message "fatal error: 'stddef.h' file not found", please install `sudo apt install clang`, this is a part of `install-rust.sh`

### Visual Studio Code

#### Rust Extensions

Rust Extensions for Visual Studio Code can be loaded using this script or see [Rust in Visual Studio Code](https://code.visualstudio.com/docs/languages/rust).

This script must be executed in a Visual Stuido Code terminal/console.

```bash
    ./install-vcode-extensions-rust.sh
```

### Install __ctrlX Data Layer__ build and runtime environment

```bash
    ctrlx-automation-sdk/scripts/install-ctrlx-datalayer.sh
```

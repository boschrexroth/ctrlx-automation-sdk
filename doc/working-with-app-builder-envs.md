# Introduction

This document describes how to work with App Build Environments (QEMU-based VMs).

## Best Practices for Working with App Build Environments

We recommend using a **App Build Environment (QEMU VM)** as follows:

1. Create a new **App Build Environment (QEMU VM)** with ctrlX WORKS and start it (the Ubuntu Server Cloud Image is automatically downloaded).
2. We recommend working with the free and open-source distributed version control system **git**: Create a repository for your project and clone it into your App Build Environment (QEMU VM).
3. Create a new branch for your work package.
4. Do your work within this branch, and commit and push your changes as often as it makes sense.
5. When your work is done and all changes are pushed, remove your **App Build Environment (QEMU VM)**.

> Working in this manner ensures that you always work with the latest software components and avoids problems with the file size of your **App Build Environment (QEMU VM)**. Furthermore, creating backups is superfluous.

For more information about file handling regarding the App Build Environment, see [ctrlX App Build Environment Ubuntu](scripts/environment/builder/README.md).

# Introduction

If the function **ctrlX CORE App Build Environment** is enabled during the ctrlX WORKS setup, you can create App Build Environments with ctrlX WORKS. These environments are QEMU Virtual Machines running Ubuntu Server LTS. They are configured automatically during the first startup.

From the ctrlX WORKS user interface, you can start, stop, or delete each environment. You can also open an SSH session to log in to it.

In most cases, no additional software is required besides ctrlX WORKS and Px Proxy.

## Check the Internet Connection

> !!! important: An internet connection is required to create and set up the App Build Environment (QEMU VM).

If your Windows host is behind a **Corporate Proxy**, you have to install [Px Proxy](px.md), first.

### AI-assisted Px Proxy setup

> !!! info AI-assisted Px Proxy setup using skill [install-pxproxy](ai.md#install-pxproxy).

You can test the connection by opening this link: [Ubuntu LTS](https://cloud-images.ubuntu.com/releases/). If the page loads and shows Ubuntu Server cloud images, the connection is working.

## Install the App Build Environment (QEMU VM) Function in ctrlX WORKS

During the ctrlX WORKS setup, select the function **ctrlX CORE App Build Environment**.

If ctrlX WORKS is already installed, you can start the setup wizard from: **Settings -> Setup -> Add or remove features**.

## Manage App Build Environments

Select **App Build Environments** in the sidebar of ctrlX WORKS. The list of available environments is shown there.

### Create an App Build Environment (QEMU VM)

* Click the **+** icon in the upper-right corner of the page.
* The dialog **Add ctrlX CORE App Build Environment** appears, and you can configure the new instance.
* **Name:** Enter a unique name.
* **Storage location:** Choose the folder where the environment files will be stored.
* **Port forwarding:** This defines how ports from the Windows host are forwarded to the VM guest. By default, port **10022** on the Windows host is forwarded to port **22** on the guest VM for SSH. Do not change port **22**. You can change **10022** if you want to run several environments at the same time.
* Select **Use HTTP and HTTPS proxy on localhost:3128**, if your Windows host is behind a **Corporate Proxy** and [Px Proxy](px.md) is already running.
* Create the environment with **OK**. To cancel, click **Cancel**.

> !!! important: The new environment uses the default user account **USER: boschrexroth** and **PASSWORD: boschrexroth**.

### Start an App Build Environment (QEMU VM)

* Start the environment by clicking the play icon (**>**).
* On the first start, the Ubuntu image is downloaded and the environment is installed. This may take some time. During this phase, you cannot log in.
* When the setup is complete, the VM shuts down automatically.
* Click the play icon again to restart the App Build Environment (QEMU VM).
* When the environment is ready, ctrlX WORKS shows **Online** in the **State** column.

## SSH access to App Build Environment (QEMU VM)

After starting the environment, it is better not to work directly in the console. Instead, use SSH for normal development work. This makes development easier, especially when combined with the Visual Studio Code extension [Remote - SSH](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh).

### AI-assisted SSH setup

> !!! info AI-assisted SSH setup using skill [setup-ssh](ai.md#setup-ssh).

### Manual SSH setup

To connect by SSH, either click the connection link for the environment in ctrlX WORKS or use `ssh.exe` in a Windows command prompt. If you have not changed the port number, the command looks like this:

```Shell
    ssh.exe -p 10022 boschrexroth@127.0.0.1
```

You can then log in with the password `boschrexroth`.

> For more information about SSH, see [SSH Connection](ssh_connection.md).

#### SSH Preparation

Typing a password every time is inconvenient during regular development work. For this reason, it is recommended to set up SSH public key authentication.

> Note: The examples below use the standard connection values (for example, port 10022). You can adjust them if needed. If you run multiple App Build Environment (QEMU VM) instances, different port numbers are recommended.

#### Step 1: Create the Key Pair (Once)

Create a public/private key pair once with the following command. The key files are stored in **~/.ssh** on Linux or in **%USERPROFILE%/.ssh** on Windows.

```Shell
    ssh-keygen -t rsa -b 4096
    # or # ssh-keygen -t ed25519
```

* **Public key:** ~/.ssh/id_rsa.pub  #(id_ed25519.pub)
* **Private key:** ~/.ssh/id_rsa     #(id_ed25519)

> Note: You can leave the passphrase empty for easier use.

#### Step 2: Register the Public Key

The public key must be registered on the App Build Environment (QEMU VM) instance in the file **/home/boschrexroth/.ssh/authorized_keys**.

```Shell
    # delete an existing entry in .ssh\known_hosts for this host and port
    ssh-keygen.exe -R [127.0.0.1]:10022
    # create the '.ssh' folder
    ssh -o "StrictHostKeyChecking no" -p 10022 boschrexroth@127.0.0.1 mkdir -p ~/.ssh/
    # register the public key
    type %USERPROFILE%\.ssh\id_rsa.pub | ssh -o "StrictHostKeyChecking no" -p 10022 boschrexroth@127.0.0.1 "cat >> ~/.ssh/authorized_keys"
```

> Note: After this step, you can connect with the public key without entering a password.

```Shell
    ssh -p 10022 boschrexroth@127.0.0.1
```

#### Step 3: Use a config Helper File for Easier SSH Access

You can organize SSH connections using the file `~/.ssh/config`. For more information, see [SSH Config meistern](https://thomas-krampe.com/posts/SSH-Config-meistern/) or [Using the SSH Config File](https://linuxize.com/post/using-the-ssh-config-file/).

A typical entry in this file might look like this:

```Shell
Host appbuilder
    HostName 127.0.0.1
    User boschrexroth
    Port 10022
    IdentityFile ~/.ssh/id_rsa
```

Then you can connect with:

```Shell
    ssh appbuilder
```

> Note: The Visual Studio Code extension **Remote - SSH** can use this config file.

### Start an SSH Session

You can open an SSH connection by clicking the corresponding link in the **SSH** column of the ctrlX WORKS **App Build Environments** view.

If the setup is correct, no password is required. Otherwise, enter **boschrexroth**.

## Install the ctrlX AUTOMATION SDK

### AI-assisted installation

> !!! info AI-assisted SDK setup using skill [install-sdk](ai.md#install-sdk).

### Manual installation 
See [README](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/README.md).

#### Stop an App Build Environment (QEMU VM)

* Click the **Stop** icon.
* If your public key is not contained in `~/.ssh/authorized_keys`, you will be asked for a password.
* If the login is successful, the shutdown command is sent and the environment stops safely.

> !!! important: Do not stop the environment by clicking the close icon in the title bar of the console window. This can damage the file system of the environment.

#### Change an App Build Environment (QEMU VM)

If the environment is **Offline**, click the **Pencil** icon to open the configuration page.

> !!! important: After the environment is created, the settings **Storage location** and **Use HTTP...** cannot be changed.

#### Delete an App Build Environment (QEMU VM)

If the environment is **Offline**, you can delete it by clicking the **Waste Bin** icon. This removes the entire folder.
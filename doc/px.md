# Internet Proxy for Windows

If your Windows host is behind a **Corporate Proxy**, you need to create the **App Build Environment (QEMU VM)** with option **Use HTTP and HTTPS proxy on localhost:3128** and install [Px Proxy](px.md). 

Px Proxy forwards HTTP and HTTPS requests from the **App Build Environment (QEMU VM)** to the configured **Corporate Proxy** and uses your **Windows credentials** for authentication.

For more information, see [Px Proxy](https://github.com/genotrance/px).

## Install Px Proxy

### AI-assisted installation

> !!! info AI-assisted Px Proxy setup using skill [install-pxproxy](ai.md#install-pxproxy).

### Manual Installation

1. Open the [Releases](https://github.com/genotrance/px/releases/) page.
2. Click **Latest** and scroll down to the Windows assets.
3. Download the **px-v*-windows-amd64.zip** file.
4. Extract the contents to a folder, for example `C:\tools\px`.
5. Create a file named `px-start.bat` with the following content:

```Shell
    cd C:\tools\px

    px.exe
```

6. Run `px-start.bat` manually before starting your **App Build Environment (QEMU VM)**. Alternatively, you can install Px Proxy as a Windows service. Follow the instructions in [AI Agents setup](ai.md).

> !!! important
> Px Proxy provides HTTP and HTTPS access through the default port **3128**. If this port is already in use on the host, you can change it in `px.ini`. You must also change the port in the VM. See [Setup a QEMU VM](setup_qemu_ubuntu.md) and [Setup a VirtualBox VM](setup_windows_virtualbox_ubuntu.md).

## Details: Proxy configuration on the App Build Environment (QEMU VM)
!!! info If you create the App Build Environment (QEMU VM) with option **Use HTTP and HTTPS proxy on localhost:3128**, the proxy configuration is set up automatically.

The address `10.0.2.2` is the host computer's IP address from the VM's point of view. See also [QEMU Virtual Machine Networking](setup_qemu_ubuntu.md#qemu-virtual-machine-networking).

The proxy URL is stored in the App Build Environment (QEMU VM) proxy configuration:

* `/etc/environment`
* `/etc/wgetrc`
* `~/.nuget/NuGet/NuGet.Config`

These environment variables are defined in `/etc/environment`:

* `http_proxy="http://10.0.2.2:3128"`
* `https_proxy="http://10.0.2.2:3128"`
* `HTTP_PROXY="http://10.0.2.2:3128"`
* `HTTPS_PROXY="http://10.0.2.2:3128"`
* `no_proxy=localhost,127.0.0.1,.local`

## Troubleshooting

To test the proxy connection from the App Build Environment (QEMU VM), run:

```code
    wget http://www.boschrexroth.com
```

If the command completes without an error, internet access through the proxy is working. Remove the downloaded file afterwards:

```code
    rm index.html*
```

If Px does not work, check the following:

* Do the proxy settings in the App Build Environment (QEMU VM) match the settings in `px.ini` on the host?
* For more information about `px.ini`, see [px.ini on GitHub](https://github.com/genotrance/px/blob/master/px.ini).
* Is another proxy, such as cntlm, already running on the host?
* To enable logging, set `log = 1` in `px.ini` on the host.

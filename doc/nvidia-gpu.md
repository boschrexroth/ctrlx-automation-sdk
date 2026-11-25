# Using an NVIDIA GPU in a Snap

## Introduction

A strictly confined snap can use an NVIDIA graphics card for compute workloads (CUDA) on ctrlX OS. This guide describes what the snap has to contain and which interfaces it needs.

A complete working example is provided in [samples-snap/nvidia-cuda-check](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/public/samples-snap/nvidia-cuda-check/README.md).

## Prerequisites

- A ctrlX OS device with an NVIDIA GPU and the NVIDIA kernel driver loaded by ctrlX OS.
- Snaps for ctrlX OS are built on Ubuntu 24.04 (core24). Use an Ubuntu 24.04 build environment, e.g. the [App Build Environment](setup_qemu_ctrlx_works.md). Building on another Ubuntu release may fail with glibc errors.

## The driver version must match exactly

The NVIDIA kernel module is part of ctrlX OS. The userspace libraries (e.g. `libcuda.so`) in your snap **must have exactly the same version** as the kernel module, otherwise CUDA fails (see [Troubleshooting](#troubleshooting)).

To find the driver version of your device:

1. Open the ctrlX OS Web UI.
2. Go to **Apps**.
3. Open the description of the **Linux Kernel App**. It lists the NVIDIA driver version.

The example uses version **580.65.06**. Replace it by the version of your device.

!!! note
    The userspace packages of the exact version may not be available in the Ubuntu archive. The example therefore downloads the official NVIDIA `.run` installer of the exact version at build time and verifies its checksum. The installer is not stored in the repository. Observe the NVIDIA license terms when you distribute the libraries in your snap.

## Bundling the NVIDIA userspace libraries

The libraries are taken from the official NVIDIA `.run` installer of the exact driver version. Bundling is described in two steps: first the complete set, then a reduced subset for a plain CUDA driver API app.

### Step 1: Complete set of libraries

Pull the pinned installer in a snapcraft part, extract it and install all userspace libraries and tools. This is the safest choice, e.g. if your app needs more than the CUDA driver API (NVML, NVENC/NVDEC, OpenCL, Vulkan, ...) or if you are not yet sure which libraries are needed:

```yaml
parts:
  nvidia-userspace:
    plugin: nil
    source: https://download.nvidia.com/XFree86/Linux-x86_64/580.65.06/NVIDIA-Linux-x86_64-580.65.06.run
    source-type: file
    source-checksum: sha256/04b10867af585e765cfbfdcf39ed5f4bd112375bebab0172eaa187c6aa5024ff
    override-build: |
      VERSION=580.65.06
      sh NVIDIA-Linux-x86_64-$VERSION.run --extract-only --target extracted
      LIBDIR=$CRAFT_PART_INSTALL/nvidia/lib
      mkdir -p $LIBDIR $CRAFT_PART_INSTALL/nvidia/bin
      cp -a extracted/*.so* $LIBDIR/
      # The installer normally creates the soname links (e.g. libcuda.so.1); they are missing in the extracted files
      for f in $LIBDIR/*.so.$VERSION; do
        soname=$(readelf -d "$f" | sed -n 's/.*(SONAME).*\[\(.*\)\]/\1/p')
        if [ -n "$soname" ] && [ "$soname" != "$(basename "$f")" ]; then
          ln -sf "$(basename "$f")" "$LIBDIR/$soname"
        fi
      done
      for tool in nvidia-smi nvidia-cuda-mps-control nvidia-cuda-mps-server; do
        if [ -f extracted/$tool ]; then
          install -m 755 extracted/$tool $CRAFT_PART_INSTALL/nvidia/bin/
        fi
      done
    prime:
      - nvidia
```

Calculate the checksum with `sha256sum` of the downloaded installer. When you change the driver version, update the URL, the checksum and `VERSION` together.

!!! note
    The driver package contains only the *driver* userspace (e.g. `libcuda.so`). If your app needs the **CUDA toolkit** (runtime libraries such as `libcudart`, cuBLAS, cuDNN, `nvcc`, ...), add it to your snap in addition, e.g. by a part which installs it from the NVIDIA CUDA repository for Ubuntu 24.04 or from the toolkit installer. Choose a toolkit version that is supported by the driver version of the device. The toolkit libraries are located in your snap, so add them to `LD_LIBRARY_PATH` as well (see below).

### Step 2: Only a subset of the libraries

To keep the snap small, bundle only what your app really loads. For a plain CUDA driver API app (no toolkit, no other NVIDIA features) the following subset is sufficient:

```yaml
parts:
  nvidia-userspace:
    plugin: nil
    source: https://download.nvidia.com/XFree86/Linux-x86_64/580.65.06/NVIDIA-Linux-x86_64-580.65.06.run
    source-type: file
    source-checksum: sha256/04b10867af585e765cfbfdcf39ed5f4bd112375bebab0172eaa187c6aa5024ff
    override-build: |
      VERSION=580.65.06
      sh NVIDIA-Linux-x86_64-$VERSION.run --extract-only --target extracted
      LIBDIR=$CRAFT_PART_INSTALL/nvidia/lib
      mkdir -p $LIBDIR $CRAFT_PART_INSTALL/nvidia/bin
      for lib in libcuda libnvidia-ml libnvidia-ptxjitcompiler libnvidia-nvvm libnvidia-gpucomp; do
        install -m 755 extracted/$lib.so.$VERSION $LIBDIR/
        ln -s $lib.so.$VERSION $LIBDIR/$lib.so.1
      done
      install -m 755 extracted/libnvidia-nvvm70.so.4 $LIBDIR/
      install -m 755 extracted/nvidia-smi $CRAFT_PART_INSTALL/nvidia/bin/
    prime:
      - nvidia
```

!!! note
    To find out which libraries your app needs, run it from a snap shell ( `sudo snap run --shell <snap>.<app>` ) with the complete set and `LD_DEBUG=libs`. This lists every library the loader opens, including those loaded with `dlopen`. Exercise all features of your app, because some libraries are loaded lazily. Keep the loaded libraries and their dependencies and remove the rest. The subset is only an example and depends on your app; the exact file list may change with the driver version.


## Plugs

The app needs these plugs:

```yaml
apps:
  my-gpu-app:
    command: bin/launch.sh
    plugs:
      - hardware-observe
      - nvidia-drivers-support
      - network-bind
      - opengl
```

| Plug | Purpose |
| --- | --- |
| `nvidia-drivers-support` | Access to the `/dev/nvidia*` device nodes |
| `opengl` | `/proc/driver/nvidia`, `/sys/devices/system/{memory,node}` and device nodes read by CUDA |
| `hardware-observe` | Lets `nvidia-smi` discover the hardware |
| `network-bind` | `cuInit` listens on an abstract Unix socket (`@cuda-uvmfd-*`); the default seccomp filter blocks `listen()` without it and `cuInit` fails with error 304 |

!!! note
    A working `nvidia-smi` does not prove that CUDA works. `cuInit` additionally needs `/dev/nvidia-uvm` and the `/proc` and `/sys` entries above.

## Using the bundled libraries at runtime

Let the app use the bundled libraries by a launcher script:

```sh
#!/bin/sh
export LD_LIBRARY_PATH="$SNAP/nvidia/lib:$LD_LIBRARY_PATH"
exec "$SNAP/bin/my-gpu-app" "$@"
```

If you bundled the CUDA toolkit, add its library directory to `LD_LIBRARY_PATH` too.

If your code uses the CUDA driver API, it can load `libcuda.so.1` with `dlopen`; no CUDA headers or toolkit are then needed at build time.

## Build, install and run

```sh
snapcraft pack --build-for=amd64
sudo snap install --dangerous ./<your-snap>.snap
sudo snap connect <your-snap>:hardware-observe
sudo snap connect <your-snap>:nvidia-drivers-support
snap connections <your-snap>
```

`network-bind` and `opengl` are auto-connected and need no manual `snap connect`; `snap connections` shows all plugs and whether they are connected.

If a proxy is required to reach `download.nvidia.com`, set `http_proxy` and `https_proxy` for the build.

The sample prints information about your driver and your gpu on success, e.g.:

```text
Userspace:     580.65.06
Kernel driver: 580.65.06
GPU 0: NVIDIA RTX 2000 Ada Generation Embedded GPU (UUID: GPU-265aaf8b-cb15-48a2-0c0f-90cc3b8c5b9c)
CUDA driver version: 13.0
CUDA devices: 1
  [0] NVIDIA RTX 2000 Ada Generation Embedded GPU, compute 8.9, 4095 MiB
GPU memory round trip: OK
```

## Troubleshooting

| Symptom | Cause / action |
| --- | --- |
| `Cannot load libcuda.so.1` | The bundled libraries are missing in the snap or `LD_LIBRARY_PATH` is not set. |
| `cuInit returned 803` | Kernel module and bundled userspace versions differ. Compare both versions. |
| `cuInit returned 100` | No NVIDIA device or driver is available. |
| `cuInit returned 304` while `nvidia-smi` works | Usually an AppArmor denial or missing `/dev/nvidia-uvm`. Check `ls -l /dev/nvidia*` (module `nvidia_uvm` loaded) and `sudo journalctl -k \| grep -i 'apparmor.*DENIED'` or `snappy-debug`. |
| Still 304, no AppArmor denials | Probably seccomp (not logged). Find the call with `sudo snap run --strace="-f -z" <snap>.<app> 2>&1 \| grep -E 'EPERM\|EACCES\|ENOSYS'`. Also verify that `network-bind` is declared. |
| Permission denied errors | Run `snappy-debug` and `snap logs` to see which plug is missing. |

The `--strace` option of `snap run` needs the snap `strace-static` on the device. Without it you get `error: cannot find an installed strace, please try 'snap install strace-static'`. Install it with `sudo snap install strace-static`.

The device cgroup of a snap only allows `/dev/nvidia[0-9]*`, `nvidiactl`, `nvidia-uvm` and `nvidia-modeset`. `/dev/nvidia-uvm-tools` reports `Operation not permitted`, which is expected and not needed by CUDA.

To distinguish sandbox problems from driver problems, install the snap with `--devmode` and run it again.

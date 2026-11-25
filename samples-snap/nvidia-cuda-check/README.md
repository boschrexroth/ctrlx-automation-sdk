# nvidia-cuda-check

A minimal example of a strictly confined snap that accesses NVIDIA hardware and calls CUDA.
See [Using an NVIDIA GPU in a Snap](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/public/doc/nvidia-gpu.md) for the full background.

The snap contains the NVIDIA compute libraries, a tiny C program and a launcher (about 76 MB).

## How it works

The NVIDIA kernel module and the userspace libraries must be the exact same version. This sample targets driver **580.65.06**. Check the driver version of your ctrlX OS in the Web UI under *Apps* in the description of the *Linux Kernel App*.

- **Userspace:** the `nvidia-userspace` part downloads the official `NVIDIA-Linux-x86_64-580.65.06.run` (checksum-pinned), extracts it and keeps only `libcuda`, `libnvidia-ml`, `libnvidia-ptxjitcompiler`, `libnvidia-nvvm`, `libnvidia-gpucomp` and `nvidia-smi` under `$SNAP/nvidia`. No CUDA toolkit is needed.
- **Device access:** the plugs `nvidia-drivers-support` and `opengl` grant `/dev/nvidia*` plus the `/proc` and `/sys` entries CUDA reads during `cuInit`; `hardware-observe` lets `nvidia-smi` discover the hardware. `network-bind` is needed because `cuInit` listens on an abstract Unix socket, which the default seccomp filter would block.
- **CUDA:** `src/cuda_check.c` loads `libcuda.so.1` with `dlopen` and uses the CUDA driver API, so no toolkit is needed at build time.
- **Launcher:** `src/launch.sh` prints kernel module vs. bundled userspace version, runs `nvidia-smi -L`, then the CUDA check.

To change the driver version, update the URL, checksum and `VERSION` in `snap/snapcraft.yaml` to match the driver of your ctrlX OS.

## Build

Build in an Ubuntu 24.04 (noble) environment matching the ctrlX OS base (core24), e.g. the ctrlX App Build Environment:

```sh
./build-snap-amd64.sh
```

If a proxy is required to reach download.nvidia.com, set `http_proxy` / `https_proxy`.

## Install and run

```sh
sudo snap install --dangerous ./nvidia-cuda-check_*.snap
sudo snap connect nvidia-cuda-check:hardware-observe
sudo snap connect nvidia-cuda-check:nvidia-drivers-support
nvidia-cuda-check.cuda-check
```

Example for expected output:

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

See the troubleshooting section in [Using an NVIDIA GPU in a Snap](https://github.com/boschrexroth/ctrlx-automation-sdk/blob/main/public/doc/nvidia-gpu.md#troubleshooting).

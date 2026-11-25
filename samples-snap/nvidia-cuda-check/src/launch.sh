#!/bin/sh
# Use the bundled userspace libraries; they must match the kernel module version.
export LD_LIBRARY_PATH="$SNAP/nvidia/lib:$LD_LIBRARY_PATH"

echo "Userspace:     $(basename "$(readlink -f "$SNAP/nvidia/lib/libcuda.so.1")" | sed 's/libcuda.so.//')"

echo "Kernel driver: $("$SNAP/nvidia/bin/nvidia-smi" --query-gpu=driver_version --format=csv,noheader 2>/dev/null | head -n1)"
"$SNAP/nvidia/bin/nvidia-smi" -L || true

exec "$SNAP/bin/cuda_check" "$@"

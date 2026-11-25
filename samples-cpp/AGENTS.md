# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-cpp/` contains the C/C++ sample applications published as part of
the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build pipeline). 
These samples are edited in place here — this folder is the source of truth for C++ sample code.

`README.md` is the index of all samples (Basic vs. Advanced), with
prerequisites for the ctrlX AUTOMATION App Build Environment / VS Code
tooling needed to build them.

## Layout per sample

Each `<sample-name>/` (e.g. `datalayer.provider.simple`) generally contains:
- `CMakeLists.txt` — the CMake project; sets `TARGET_PROJECT_NAME` (must
  match the Snap "app name"/ExeName), references `SDK_ROOT_DIR` as
  `../../`, and invokes `build-fbs.sh` / `build-mddb.sh` via
  `execute_process`.
- `main.cpp`, `ctrlx_datalayer_helper.h` — sample source.
- `sampleSchema.fbs` — FlatBuffers schema, compiled by `build-fbs.sh`.
- `metadata.csv` — Snap packaging metadata.
- `build-debug.sh` — local debug build.
- `build-fbs.sh` / `build-mddb.sh` — generate FlatBuffers bindings /
  metadata database as part of the CMake configure step.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — package the built binary
  as a Snap.
- `snap/` — Snapcraft packaging manifest.

Follow the naming pattern `datalayer.<role>.<variant>` (e.g.
`datalayer.client.simple`, `datalayer.provider.sub`) for new Datalayer
samples, matching the conventions used across the other `samples-<lang>/`
folders. Mirror the existing per-sample layout when adding a new one.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index (Basic vs. Advanced
  section) and to `../samples.md` if applicable.
- Keep `TARGET_PROJECT_NAME` in `CMakeLists.txt` consistent with the Snap
  app name declared in `snap/snapcraft.yaml`.

## Validation

There is no separate unit test suite — validate changes by configuring and
building the affected sample with CMake:
```bash
cd <sample-name>
./build-debug.sh
```
For Snap packaging changes, run `build-snap-amd64.sh` (or `-arm64.sh`),
which requires the ctrlX AUTOMATION App Build Environment / Snapcraft
tooling.

## Licensing

Code here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent with
the rest of the repository.

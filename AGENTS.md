# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

This is the **published product** of the ctrlX AUTOMATION Software
Development Kit (`boschrexroth/ctrlx-automation-sdk`).

- Sample apps under `samples-<lang>/` are usually edited in place here and
  are the actual source of truth for those samples, 
- Each `samples-<lang>/` directory has its own `AGENTS.md` with language-specific conventions —
  read it before editing samples in that language; the most specific one wins.

## Key entry points

- `samples.md` — top-level index of all sample applications across
  languages; keep it in sync when adding/removing samples.
- `samples-<lang>/README.md` — per-language sample index (cpp, go,
  iec61131, java, kotlin, net, node, python, rust, snap).
- `mkdocs.yml` — MkDocs site configuration (theme: material).
- `CHANGELOG.md` — SDK release notes; update when shipping user-facing
  changes to this published content.

## Sample app conventions

Each `samples-<lang>/<sample-name>/` follows a consistent per-app layout,
e.g. for C++ (`samples-cpp/datalayer.provider.simple/`): `CMakeLists.txt`,
`README.md`, `main.cpp`, `metadata.csv` (snap packaging metadata),
`build-debug.sh` / `build-snap-<arch>.sh` build helper scripts, and a
`snap/` directory for Snapcraft packaging. Follow the naming pattern
`datalayer.<role>.<variant>` (e.g. `datalayer.client.simple`,
`datalayer.provider.sub`) for new Datalayer samples. Mirror this structure
when adding a sample in another language directory rather than inventing a
new layout. Add new samples to both the per-language `README.md` and the
top-level `samples.md`.

## Validation

There is no unit test suite here — validate changes by re-running the
relevant pipeline stage from the repository root and inspecting this
folder's output.

## Licensing

Content here is MIT-licensed (`SPDX-License-Identifier: MIT`), except the
prebuilt binaries `datalayer.dll` and `comm.datalayer.so*` under `bin/` /
`lib/`, which carry a separate Bosch Rexroth free-of-charge license (see
`README.md`). Do not relicense or strip headers from those binaries.

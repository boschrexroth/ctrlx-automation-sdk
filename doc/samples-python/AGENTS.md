# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-python/` contains the Python sample applications published as
part of the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build
pipeline). These samples are edited in place here — this folder is the source of truth for Python sample code.

`README.md` is the index of all samples.

## Layout per sample

Each `<sample-name>/` (e.g. `datalayer.provider`, `datalayer.client.simple`)
generally contains:
- `app/` — sample source.
- `pyproject.toml` / `setup.cfg` / `requirements.txt` — Python packaging
  and dependency definitions.
- `install-venv.sh` — creates a local virtual environment and installs
  dependencies.
- `wrapper/` — packaging wrapper scripts/entry points used for Snap
  packaging.
- `fbs/` / `bfbs/` (where present) — FlatBuffers schema sources and
  compiled binary schema; `make-bfbs.sh` regenerates these.
- `sampleSchema.fbs` (where present) — FlatBuffers schema definition.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — package the sample as a
  Snap.
- `snap/` — Snapcraft packaging manifest.

Follow the naming pattern `datalayer.<role>.<variant>` (e.g.
`datalayer.client.sub`, `datalayer.provider.all-data`) for new Datalayer
samples, matching the conventions used across the other `samples-<lang>/`
folders. Mirror the existing per-sample layout when adding a new one.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index and to
  `../samples.md` if applicable.
- Keep `requirements.txt` / `pyproject.toml` dependency versions minimal
  and pinned consistently with sibling samples.

## Validation

There is no separate unit test suite — validate changes by setting up the
virtual environment and running/importing the affected sample:
```bash
cd <sample-name>
./install-venv.sh
```
For Snap packaging changes, run `build-snap-amd64.sh` (or `-arm64.sh`),
which requires the ctrlX AUTOMATION App Build Environment / Snapcraft
tooling.

## Licensing

Code here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent with
the rest of the repository.

# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-snap/` contains standalone Snap-packaging sample apps published as
part of the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build
pipeline). Unlike the per-language `samples-<lang>/` folders (which focus
on Data Layer client/provider APIs), these samples demonstrate
Snap/Snapcraft packaging techniques and ctrlX CORE platform integration
features — they are edited in place here and are the source of truth for
this content.

`README.md` is the index of all samples.

## Samples in this directory

- `custom-language-pack/` — packaging a custom language pack into a Snap.
- `generate-manifest/` — generating a Snap manifest.
- `self-health/` — implementing a Snap app's self-health check
  integration (contains a `bin/` directory with example scripts).
- `storage-extension/` — using a Snap storage extension.
- `tpm2-webserver/` — a webserver sample using TPM2 (contains
  `configs/` and a `webserver/` app), building on Go/webserver sample
  patterns.
- `ulimit/` — configuring resource limits (ulimit) for a Snap app.

## Layout per sample

Each sample generally contains:
- `README.md` — sample-specific documentation, linked from the top-level
  `README.md`.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — build/package the Snap.
- `snap/` — Snapcraft packaging manifest (`snapcraft.yaml`).
- Sample-specific source/config directories (e.g. `bin/`, `configs/`,
  `webserver/`) as appropriate to what the sample demonstrates.

Mirror the existing per-sample layout when adding a new packaging sample
rather than inventing a new structure.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index and to
  `../samples.md` if applicable.
- Keep each sample focused on demonstrating one specific Snap/platform
  feature, consistent with the existing samples here.

## Validation

There is no separate unit test suite — validate changes by running the
sample's `build-snap-amd64.sh` (or `-arm64.sh`) script, which requires the
ctrlX AUTOMATION App Build Environment / Snapcraft tooling, and, where
applicable, deploying/testing the resulting Snap on a ctrlX CORE or ctrlX
CORE<sup>virtual</sup>.

## Licensing

Content here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent
with the rest of the repository.

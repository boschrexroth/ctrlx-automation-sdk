# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-rust/` contains the Rust sample applications published as part of
the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build pipeline). 
These samples are edited in place here — this folder is the source of truth for Rust sample code.

`README.md` is the index of all samples.

## Layout per sample

Each `<sample-name>/` (e.g. `datalayer.provider.simple`) generally contains:
- `Cargo.toml` / `Cargo.lock` — crate definition; depends on the SDK's own
  `ctrlx-datalayer` and `ctrlx-fbs` crates referenced by local `path =
  "crate/crates/..."`, plus public crates (`chrono`, `flatbuffers`,
  `env_logger`, `log`, `signal-hook`, etc.) — do not repoint the
  `ctrlx-*` path dependencies at crates.io.
- `src/` — sample source.
- `fbs/` / `bfbs/` — FlatBuffers schema sources and compiled binary schema.
- `build-cargo.sh` — local build script wrapping `cargo build`.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — package the built binary
  as a Snap.
- `snap/` — Snapcraft packaging manifest.

Follow the naming pattern `datalayer.<role>.<variant>` (e.g.
`datalayer.client.bulk`, `datalayer.provider.sub`) for new Datalayer
samples, matching the conventions used across the other `samples-<lang>/`
folders. Mirror the existing per-sample layout when adding a new one.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index and to
  `../samples.md` if applicable.
- After changing dependencies in `Cargo.toml`, regenerate `Cargo.lock`
  (`cargo generate-lockfile` or a normal `cargo build`) and keep it
  committed.

## Validation

There is no separate unit test suite — validate changes by building the
affected sample:
```bash
cd <sample-name>
./build-cargo.sh
# or directly:
cargo build --release
```
For Snap packaging changes, run `build-snap-amd64.sh` (or `-arm64.sh`),
which requires the ctrlX AUTOMATION App Build Environment / Snapcraft
tooling.

## Licensing

Code here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent with
the rest of the repository.

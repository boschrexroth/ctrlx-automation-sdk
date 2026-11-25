# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-go/` contains the Go sample applications published as part of the
ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build pipeline). 
These samples are edited in place here — this folder is the source of truth for Go sample code. 

Each sample is its own Go module (`go.mod`), vendoring its
dependencies (`vendor/`) so it builds without network access.

`README.md` is the index of all samples.

## Layout per sample

Each `<sample-name>/` (e.g. `datalayer.provider.simple`) generally contains:
- `go.mod` / `go.sum` — module definition; depends on
  `github.com/boschrexroth/ctrlx-datalayer-golang/v2` and
  `github.com/google/flatbuffers`.
- `vendor/` — vendored dependencies (keep in sync with `go.mod`/`go.sum`
  via `go mod vendor` after any dependency change).
- `cmd/`, `pkg/`, `provider/` — sample source, organized by standard Go
  project layout conventions.
- `fbs/` / `bfbs/` — FlatBuffers schema sources and compiled binary schema.
- `build.sh` — local build script.
- `update.sh` — updates/regenerates dependencies or generated code.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — package the built binary
  as a Snap.
- `snap/` — Snapcraft packaging manifest.

Follow the naming pattern `datalayer.<role>.<variant>` (e.g.
`datalayer.client.ext`, `datalayer.provider.sub`) for new Datalayer
samples, matching the conventions used across the other `samples-<lang>/`
folders. Mirror the existing per-sample layout when adding a new one.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index and to
  `../samples.md` if applicable.
- After changing dependencies in `go.mod`, run `go mod tidy && go mod
  vendor` inside the sample directory so `vendor/` and `go.sum` stay
  consistent.

## Validation

There is no separate unit test suite — validate changes by building the
affected sample:
```bash
cd <sample-name>
./build.sh
# or directly:
go build ./...
```
For Snap packaging changes, run `build-snap-amd64.sh` (or `-arm64.sh`),
which requires the ctrlX AUTOMATION App Build Environment / Snapcraft
tooling.

## Licensing

Code here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent with
the rest of the repository.

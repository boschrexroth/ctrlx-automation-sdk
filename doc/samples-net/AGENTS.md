# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-net/` contains the .NET sample applications published as part of
the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build pipeline). 
These samples are edited in place here — this folder is the source of truth for .NET sample code.

`README.md` is the index of all samples (Basic vs. Advanced); `samples.sln`
is the umbrella solution referencing every sample project.

## Layout per sample

Each `<sample-name>/` (e.g. `datalayer.provider.simple`) generally contains:
- `<sample-name>.csproj` / `<sample-name>.sln` — the .NET project/solution.
- `Program.cs` and other `*.cs` source files.
- `README.md` — sample-specific documentation, linked from the top-level
  `README.md`.
- `nuget.config` — points `packageSources` at the local `../nuget` folder
  (the SDK's own NuGet feed produced by the build), not nuget.org.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — run `dotnet publish`
  (self-contained, single-file, trimmed, `linux-x64`/`linux-arm64`) then
  invoke `../../scripts/build-snap-<arch>.sh` to package a Snap.
- `snap/snapcraft.yaml` — Snapcraft packaging manifest.
- `bin/`, `obj/`, `docs/`, `bfbs/`, `fbs/`, `sample/` — build artifacts /
  generated or supporting content; don't hand-edit build output under
  `bin/`/`obj/`.

Follow the naming pattern `datalayer.<role>.<variant>` (e.g.
`datalayer.client.simple`, `datalayer.provider.sub`) for new Datalayer
samples, matching the conventions used in the other `samples-<lang>/`
folders. Mirror the existing per-sample layout when adding a new one rather
than inventing a new structure.

## Adding or changing a sample

- Add new samples to both the sample's own `README.md` and the top-level
  `README.md` index (Basic vs. Advanced section), and to `samples.sln` and
  `../samples.md` if applicable.
- Keep `nuget.config` pointing at `../nuget` — do not repoint samples at a
  public NuGet feed.
- If a sample needs Snap packaging, add a `snap/snapcraft.yaml` and
  `build-snap-<arch>.sh` script mirroring an existing sample (e.g.
  `datalayer.provider.simple`).

## Validation

There is no separate unit test suite — validate changes by building the
affected sample:
```bash
dotnet build <sample-name>/<sample-name>.csproj
```
or build the whole solution:
```bash
dotnet build samples.sln
```
For Snap packaging changes, run the sample's `build-snap-amd64.sh` (or
`-arm64.sh`) script, which requires the self-contained `dotnet publish`
step and the Snap tooling.

## Licensing

Code here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent with
the rest of the repository.

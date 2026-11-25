# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-java/` contains the Java sample applications published as part of
the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build pipeline). 
These samples are edited in place here — this folder is the source of truth for Java sample code. 

Each sample is a Gradle project using the Gradle wrapper.

`README.md` is the index of all samples.

## Layout per sample

Each `<sample-name>/` (e.g. `datalayer.provider.simple`) generally contains:
- `app/` — sample source (standard Gradle project layout).
- `settings.gradle.kts` — Gradle settings (Kotlin DSL).
- `gradlew` / `gradlew.bat` / `gradle/wrapper/` — Gradle wrapper, use these
  rather than a system-installed Gradle.
- `fbs/` / `bfbs/` — FlatBuffers schema sources and compiled binary schema.
- `build-gradle.sh` — local build script wrapping the Gradle build.
- `gradle_proxy.sh` — configures Gradle proxy settings for restricted
  build environments.
- `build-snap-amd64.sh` / `build-snap-arm64.sh` — package the built
  artifact as a Snap.
- `snap/` — Snapcraft packaging manifest.

Follow the naming pattern `datalayer.<role>.<variant>` (e.g.
`datalayer.client.bulk`, `datalayer.provider.sub`) for new Datalayer
samples, matching the conventions used across the other `samples-<lang>/`
folders. Mirror the existing per-sample layout when adding a new one.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index and to
  `../samples.md` if applicable.
- Use the checked-in Gradle wrapper (`./gradlew`) for consistent builds
  rather than a system Gradle.

## Validation

There is no separate unit test suite — validate changes by building the
affected sample:
```bash
cd <sample-name>
./build-gradle.sh
# or directly:
./gradlew build
```
For Snap packaging changes, run `build-snap-amd64.sh` (or `-arm64.sh`),
which requires the ctrlX AUTOMATION App Build Environment / Snapcraft
tooling.

## Licensing

Code here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent with
the rest of the repository.

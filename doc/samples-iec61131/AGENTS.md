# AGENTS.md

Guidance for AI coding agents working in this directory.

## What this directory is

`samples-iec61131/` contains IEC 61131-3 (PLC) sample projects published as
part of the ctrlX AUTOMATION SDK (see `public/AGENTS.md` for how this fits into the overall build
pipeline). These samples are edited in place here — this folder is the
source of truth for the IEC 61131-3 sample content.

`README.md` is the index of all samples. This directory is smaller and
structured differently from the other `samples-<lang>/` folders (no shared
per-sample build script convention).

## Layout

- `fbs-read-write/` — sample project with an exported PLC project file
  (`fbs-read-write.xml`, ctrlX PLC Engineering export format) and a
  `start.bat` helper.
- `fbs2plc/` — related sample/tooling for FlatBuffers-to-PLC data
  conversion.

Each sample's PLC project is typically distributed as an XML export
(`*.xml`) intended to be imported into ctrlX PLC Engineering rather than
compiled from source directly in this repo.

## Adding or changing a sample

- Add new samples to the top-level `README.md` index and to
  `../samples.md` if applicable.
- When editing exported PLC project XML, prefer re-exporting from ctrlX
  PLC Engineering after making changes in the IDE rather than hand-editing
  the XML, to avoid corrupting the project structure.

## Validation

There is no automated build/test here — validate changes by importing the
project into ctrlX PLC Engineering (or the referenced tooling) and
confirming it loads/builds/downloads correctly to a ctrlX CORE / ctrlX
CORE<sup>virtual</sup>.

## Licensing

Content here is MIT-licensed (`SPDX-License-Identifier: MIT`), consistent
with the rest of the repository.

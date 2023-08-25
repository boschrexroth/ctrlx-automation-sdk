# README tpm2.srk

The sample app __tpm2.srk__ shows the basic usage of a TPM2 in Golang with go-tpm and go-tpm-tools.

Also see ["TPM2"](../../tpm2.md).

## Introduction

It demonstrates how you can use the TPM2.0 of the ctrlX CORE or the emulated TPM2.0 on the ctrlX CORE<sup>virtual</sup> inside a Golang application.

## Getting Started

```bash
cd samples-go/tpm2.srk
go build ./cmd/tpm2
export TPM2_SOCKET="<path to socket>"
./tpm2 create
./tpm2 load
```

## Support

If you've any questions visit the [ctrlX AUTOMATION Community](https://developer.community.boschrexroth.com/)

___

## License

SPDX-FileCopyrightText: Bosch Rexroth AG
SPDX-License-Identifier: MIT

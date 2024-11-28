#!/bin/bash

go vet ./...
go build ./cmd/tpm2

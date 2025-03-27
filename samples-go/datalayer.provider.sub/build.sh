#!/bin/bash

go vet ./...
go build ./cmd/provider
go build ./cmd/client
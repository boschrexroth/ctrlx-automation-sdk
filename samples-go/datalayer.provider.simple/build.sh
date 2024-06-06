#!/bin/bash

go vet ./...
go build ./cmd/provider

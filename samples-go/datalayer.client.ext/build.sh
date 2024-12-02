#!/bin/bash

go vet ./...
go build ./cmd/client

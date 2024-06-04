# !/bin/bash

go get -u ./...
go mod vendor
go mod tidy
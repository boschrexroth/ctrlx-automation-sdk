/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package main

import (
	"crypto/tls"
	"fmt"
	"net/http"

	"github.com/dustin/go-humanize"
	"github.com/emersion/go-webdav"
)

const (
	prefix string = "/solutions/webdav/"
)

var (
	host     string = "https://localhost:8443"
	user     string = "boschrexroth" // The user needs manage configurations to access the files
	password string = "boschrexroth"
)

func main() {
	httpClient := webdav.HTTPClientWithBasicAuth(&http.Client{
		Transport: &http.Transport{
			TLSClientConfig: &tls.Config{
				InsecureSkipVerify: true,
			},
		},
	}, user, password)
	client, err := webdav.NewClient(httpClient, host+prefix)
	if err != nil {
		panic(err)
	}

	// list the files of the active configuration
	printDir(client, "./appdata")

	// remove archive webdav.client
	err = client.RemoveAll("./webdav.client")
	if err != nil {
		fmt.Println(err)
	}

	// create archive webdav.client
	err = client.Mkdir("./webdav.client")
	if err != nil {
		fmt.Println(err)
	}

	// create a text file
	w, err := client.Create("./webdav.client/hello.txt")
	if err != nil {
		panic(err)
	}
	_, _ = w.Write([]byte(`
	I was there!
	Best regards
	webdav.client
	`))
	_ = w.Close()
	printDir(client, "./webdav.client")

	// create a copy of appdata folder
	err = client.CopyAll("./appdata", "webdav.client.copy", true)
	if err != nil {
		fmt.Println(err)
	}
	printDir(client, "./webdav.client.copy")
}

func printDir(client *webdav.Client, path string) {
	fInfos, err := client.Readdir(path, true)
	if err != nil {
		panic(err)
	}
	for _, info := range fInfos {
		fmt.Printf("%s\t%s\t%s\n", info.ModTime, humanize.Bytes(uint64(info.Size)), info.Path)
	}
}

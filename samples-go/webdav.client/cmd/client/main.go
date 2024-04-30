/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package main

import (
	"context"
	"crypto/tls"
	"fmt"
	"net/http"
	"os"
	"os/signal"
	"syscall"

	"github.com/dustin/go-humanize"
	"github.com/emersion/go-webdav"
)

const (
	prefix string = "/solutions/webdav/"
)

var (
	host string = "https://localhost:8443"
	//host     string = "https://10.0.2.2:8443"
	user     string = "boschrexroth" // The user needs manage configurations to access the files
	password string = "boschrexroth"
)

func isSnap() bool {
	return os.Getenv("SNAP") != ""
}

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
	err = client.RemoveAll(context.TODO(), "./webdav.client")
	if err != nil {
		fmt.Println(err)
	}

	// create archive webdav.client
	err = client.Mkdir(context.TODO(), "./webdav.client")
	if err != nil {
		fmt.Println(err)
	}

	// create a text file
	w, err := client.Create(context.TODO(), "./webdav.client/hello.txt")
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
	err = client.Copy(context.TODO(), "./appdata", "webdav.client.copy", &webdav.CopyOptions{NoRecursive: false, NoOverwrite: false})
	if err != nil {
		fmt.Println(err)
	}
	printDir(client, "./webdav.client.copy")

	if isSnap() {
		// Handle process exit event (SIGTERM)
		sigterm := make(chan os.Signal, 1) // we need to reserve to buffer size 1, so the notifier are not blocked
		signal.Notify(sigterm, os.Interrupt, syscall.SIGINT, syscall.SIGTERM, syscall.SIGABRT)
		// Wait for process termination
		<-sigterm
	}
}

func printDir(client *webdav.Client, path string) {
	fInfos, err := client.ReadDir(context.TODO(), path, true)
	if err != nil {
		panic(err)
	}
	for _, info := range fInfos {
		fmt.Printf("%s\t%s\t%s\n", info.ModTime, humanize.Bytes(uint64(info.Size)), info.Path)
	}
}

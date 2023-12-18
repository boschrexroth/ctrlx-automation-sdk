/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package main

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"path/filepath"
)

func main() {

	www := ""

	// change commonpath if app is not running as a snap
	if !isSnap() {
		fmt.Printf("app is running as external application (Debug)")
		dir, _ := os.Getwd()
		www = filepath.FromSlash(filepath.Join(dir, "www"))
	} else {
		fmt.Printf("app is running as snap inside ctrlX")
		www = filepath.FromSlash(filepath.Join(os.Getenv("SNAP"), "www"))
	}

	// Check if directory exist
	fmt.Printf("check if serve path exist: %s \n", www)
	if _, err := os.Stat(www); os.IsNotExist(err) {
		// If directory not exist, cancel operation because nothing to serve
		fmt.Printf("serve path does not exist \n")
		log.Fatalf("nothing to serve")
	} else {
		fmt.Printf("Common-Path exist \n")
	}

	// Create http handle
	http.Handle("/sdk-go-webserver/", http.StripPrefix("/sdk-go-webserver/", http.FileServer(http.Dir(www))))

	// Differentiate between app is snapped (-> unix sockets) and app is running external (-> tcp)
	if isSnap() {

		sockpath := filepath.Join(os.Getenv("SNAP_DATA"), "/package-run/sdk-go-webserver/")
		sockfile := filepath.Join(sockpath, "web.sock")
		if _, err := os.Stat(sockpath); os.IsNotExist(err) {
			os.MkdirAll(sockpath, os.ModePerm)

		}
		os.Remove(sockfile)

		unixListener, err := net.Listen("unix", sockfile)
		if err != nil {
			panic(err)
		}

		fmt.Printf("server UP - UNIX SOCKET - File: %s ", sockfile)
		http.Serve(unixListener, nil)

	} else {
		port := "1234"

		tcpListener, err := net.Listen("tcp", ":"+port)
		if err != nil {
			panic(err)
		}

		fmt.Printf("server UP - TCP - Port: %s ", port)
		http.Serve(tcpListener, nil)
	}
}

// Indicates whether the application is running inside a snap environment.
func isSnap() bool {
	return os.Getenv("SNAP") != ""
}

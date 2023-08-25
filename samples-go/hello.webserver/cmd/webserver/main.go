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
	snapped := false
	// change commonpath if app is not running as a snap
	if _, snapped = os.LookupEnv("SNAP"); !snapped {
		fmt.Printf("App is running as external Application (Debug)")
		dir, _ := os.Getwd()
		www = filepath.FromSlash(filepath.Join(dir, "www"))
	} else {
		fmt.Printf("App is running as snap inside ctrlX")
		www = filepath.FromSlash(filepath.Join(os.Getenv("SNAP"), "www"))
	}

	// Check if directory exist
	fmt.Printf("Check if serve path exist: %s \n", www)
	if _, err := os.Stat(www); os.IsNotExist(err) {
		// If directory not exist, cancel operation because nothing to serve
		fmt.Printf("Serve path does not exist \n")
		log.Fatalf("Nothing to serve")
	} else {
		fmt.Printf("Common-Path exist \n")
	}

	// Create http handle
	http.Handle("/hello-webserver/", http.StripPrefix("/hello-webserver/", http.FileServer(http.Dir(www))))

	// Differentiate between app is snapped (-> unix sockets) and app is running external (-> tcp)
	if snapped {

		sockpath := filepath.Join(os.Getenv("SNAP_DATA"), "/package-run/rexroth-hello-webserver/")
		sockfile := filepath.Join(sockpath, "web.sock")
		if _, err := os.Stat(sockpath); os.IsNotExist(err) {
			os.MkdirAll(sockpath, os.ModePerm)

		}
		os.Remove(sockfile)

		unixListener, err := net.Listen("unix", sockfile)
		if err != nil {
			panic(err)
		}

		fmt.Printf("Server UP - UNIX SOCKET - File: %s ", sockfile)
		http.Serve(unixListener, nil)

	} else {
		port := "1234"

		tcpListener, err := net.Listen("tcp", ":"+port)
		if err != nil {
			panic(err)
		}

		fmt.Printf("Server UP - TCP - Port: %s ", port)
		http.Serve(tcpListener, nil)
	}

}

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
)

func main() {

	// Start the AppData Server
	go func() {
		err := StartAppDataServer()
		if err != nil {
			fmt.Printf("failed to start the AppData server: %s", err.Error())
			return
		}
	}()

	// Graceful shutdown
	defer StopAppDataListener()

	// Handle process exit event (SIGTERM)
	sigterm := make(chan os.Signal, 1) // we need to reserve to buffer size 1, so the notifier are not blocked
	signal.Notify(sigterm, os.Interrupt, syscall.SIGINT, syscall.SIGTERM, syscall.SIGABRT)

	// Wait for process termination
	<-sigterm
}

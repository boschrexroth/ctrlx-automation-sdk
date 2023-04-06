/**
 * MIT License
 *
 * Copyright (c) 2021-2023 Bosch Rexroth AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
package main

import (
	"fmt"
	"os"
	"strconv"
	"time"

	dlc "datalayer.client/pkg/client"

	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
)

// Get client connection
func main() {

	system := datalayer.NewSystem("")
	defer datalayer.DeleteSystem(system)
	fmt.Println("INFO starting Data Layer system")
	system.Start(false)

	// Try SSL port 8443
	connection := getClientConnection(8443)
	c := system.Factory().CreateClient(connection)

	defer datalayer.DeleteClient(c)

	if !c.IsConnected() {
		// client not connected
		fmt.Println("WARNING connecting", connection, "failed")
		datalayer.DeleteClient(c)

		// Try SSL port 443
		connection = getClientConnection(443)
		c = system.Factory().CreateClient(connection)
	}

	// client connected
	fmt.Println("INFO creating Data Layer client instance")

	callClient := dlc.NewCallDatalayerClient(c)

	for c.IsConnected() {
		callClient.Run()
		time.Sleep(1 * time.Second)
	}

	dlc.DeleteCallDatalayerClient(callClient)

	system.Stop(false)
	fmt.Println("INFO stopping Data Layer system")
}

// This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:2070?sslport=SSL_PORT
// Please check and change according your environment:
//
// - USER:       Enter your user name here - default is boschrexroth
// - PASSWORD:   Enter your password here - default is boschrexroth
// - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
//               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
//               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater
// - SSL_PORT:   8443 If you use a ctrlX virtual with default port forwarding

// Connection address dependent on the runtime
func getClientConnection(p int) string {
	if os.Getenv("SNAP") != "" {
		return "ipc://"
	}

	return "tcp://boschrexroth:boschrexroth@10.0.2.2:2069?sslport=" + strconv.Itoa(p)
}

/**
 * MIT License
 *
 * Copyright (c) 2021-2022 Bosch Rexroth AG
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
	"os/signal"
	"syscall"

	"datalayer.provider/pkg/node"
	"datalayer.provider/pkg/utils"
	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
)

func main() {

	fmt.Println("Register 'sdk-go-datalayer-provider' as root element with 2 nodes 'string-value', 'inertial-value'")

	system := datalayer.NewSystem("")
	defer datalayer.DeleteSystem(system)
	system.Start(false)

	// Create a connection string with the parameters according to your environment (see ConnectionBuilder.go in package utils)
	var connection = utils.NewConnectionBuilder().Ip("192.168.1.1").SslPort(443).Build()
	runProvider(system, connection)

	system.Stop(false)
	fmt.Println("INFO End: ", connection)
}

// run provider and its nodes
func runProvider(system *datalayer.System, con string) {
	provider := system.Factory().CreateProvider(con)
	defer datalayer.DeleteProvider(provider)

	fmt.Println("INFO Connecting: ", con)

	result := provider.Start()

	if result != datalayer.ResultOk {
		fmt.Println("ERROR Starting ctrlX Data Layer Provider failed with: ", result)
		return
	}

	runNodes(provider)

	provider.Stop()
	fmt.Println("INFO stop connection: ", con)
}

// register and run provider nodes
func runNodes(provider *datalayer.Provider) {

	fmt.Println("INFO register nodes")

	// Register a node as string value
	nds := node.NewNodeDataString("string-value")
	defer node.DeleteNodeData(&nds.NodeData)
	go node.StartNodeDataHandler(nds)
	r := provider.RegisterNode(nds.Name(), nds.Node())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Registering node ", nds.Name(), " failed: ", r)
	}

	// Register a node as flatbuffer value
	ndfbs := node.NewNodeDataFbs("inertial-value", provider)
	defer node.DeleteNodeData(&ndfbs.NodeData)
	go node.StartNodeDataHandler(ndfbs)
	r = provider.RegisterNode(ndfbs.Name(), ndfbs.Node())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Registering node ", ndfbs.Name(), " failed: ", r)
	}

	ndt := node.NewNodeDataTimestamp("timestamp-value")
	defer node.DeleteNodeData(&ndt.NodeData)
	go node.StartNodeDataHandler(ndt)
	r = provider.RegisterNode(ndt.Name(), ndt.Node())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Registering node ", ndt.Name(), " failed: ", r)
	}

	ndat := node.NewNodeDataArrayTimestamp("array-of-timestamp-value")
	defer node.DeleteNodeData(&ndat.NodeData)
	go node.StartNodeDataHandler(ndat)
	r = provider.RegisterNode(ndat.Name(), ndat.Node())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Registering node ", ndat.Name(), " failed: ", r)
	}

	b := provider.IsConnected()
	if !b {
		fmt.Println("ctrlX Data Layer Provider not connectet")
	}

	// waits on process end
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM, syscall.SIGABRT)
	s := <-sigs
	fmt.Println("ctrlX Data Layer Provider signal: ", s)

	r = provider.UnregisterNode(ndat.Name())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Unregistering node ", ndat.Name(), " failed: ", r)
	}
	r = provider.UnregisterNode(ndt.Name())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Unregistering node ", ndt.Name(), " failed: ", r)
	}

	fmt.Println("INFO unregister nodes")
	// Unregister node of the flatbuffer
	r = provider.UnregisterNode(ndfbs.Name())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Unregistering node ", ndfbs.Name(), " failed: ", r)
	}
	ndfbs.UnregisterType(provider)

	// Unregister node of the string
	r = provider.UnregisterNode(nds.Name())
	if r != datalayer.ResultOk {
		fmt.Println("ERROR Unregistering node ", nds.Name(), " failed: ", r)
	}
}

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

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"

	"datalayer.provider/pkg/node"
	"datalayer.provider/pkg/utils"
)

func main() {

	fmt.Println("Register 'sdk-go-provider' as root element with nodes")

	system := datalayer.NewSystem("")
	defer datalayer.DeleteSystem(system)
	system.Start(false)

	// Create a connection string with the parameters according to your environment
	var connection = utils.NewRemote().Ip("10.0.2.2").SslPort(8443).Build()
	runProvider(system, connection)

	system.Stop(false)
	fmt.Println("end: ", connection)
}

// run provider and its nodes
func runProvider(system *datalayer.System, con string) {
	provider := system.Factory().CreateProvider(con)
	defer datalayer.DeleteProvider(provider)

	fmt.Println("connecting to: ", con)

	result := provider.Start()

	if result != datalayer.ResultOk {
		fmt.Println("starting ctrlX Data Layer Provider failed with: ", result)
		return
	}

	runNodes(provider)

	provider.Stop()
	fmt.Println("stop connection: ", con)
}

// register and run provider nodes
func runNodes(provider *datalayer.Provider) {

	fmt.Println("register nodes")

	// Register a node as string value
	nds := node.NewNodeDataString("string-value")
	defer node.DeleteNodeData(&nds.NodeData)
	go node.StartNodeDataHandler(nds)
	r := provider.RegisterNode(nds.Name(), nds.Node())
	if r != datalayer.ResultOk {
		fmt.Println("registering node ", nds.Name(), " failed: ", r)
	}

	// Register a node as flatbuffer value
	ndfbs := node.NewNodeDataFbs("inertial-value", provider)
	defer node.DeleteNodeData(&ndfbs.NodeData)
	go node.StartNodeDataHandler(ndfbs)
	r = provider.RegisterNode(ndfbs.Name(), ndfbs.Node())
	if r != datalayer.ResultOk {
		fmt.Println("registering node ", ndfbs.Name(), " failed: ", r)
	}

	ndt := node.NewNodeDataTimestamp("timestamp-value")
	defer node.DeleteNodeData(&ndt.NodeData)
	go node.StartNodeDataHandler(ndt)
	r = provider.RegisterNode(ndt.Name(), ndt.Node())
	if r != datalayer.ResultOk {
		fmt.Println("registering node ", ndt.Name(), " failed: ", r)
	}

	ndat := node.NewNodeDataArrayTimestamp("array-of-timestamp-value")
	defer node.DeleteNodeData(&ndat.NodeData)
	go node.StartNodeDataHandler(ndat)
	r = provider.RegisterNode(ndat.Name(), ndat.Node())
	if r != datalayer.ResultOk {
		fmt.Println("registering node ", ndat.Name(), " failed: ", r)
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
		fmt.Println("unregistering node ", ndat.Name(), " failed: ", r)
	}
	r = provider.UnregisterNode(ndt.Name())
	if r != datalayer.ResultOk {
		fmt.Println("unregistering node ", ndt.Name(), " failed: ", r)
	}

	fmt.Println("INFO unregister nodes")
	// Unregister node of the flatbuffer
	r = provider.UnregisterNode(ndfbs.Name())
	if r != datalayer.ResultOk {
		fmt.Println("unregistering node ", ndfbs.Name(), " failed: ", r)
	}
	ndfbs.UnregisterType(provider)

	// Unregister node of the string
	r = provider.UnregisterNode(nds.Name())
	if r != datalayer.ResultOk {
		fmt.Println("unregistering node ", nds.Name(), " failed: ", r)
	}
}

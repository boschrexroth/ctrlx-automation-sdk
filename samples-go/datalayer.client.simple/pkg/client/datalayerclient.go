/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package client

import (
	"fmt"
	"time"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
)

var (
	rootStatic  = "sdk/cpp/datalayer/provider/alldata/static/"
	rootDynamic = "sdk/cpp/datalayer/provider/alldata/dynamic/"
)

// datalayerClient is a client wrapper.
type datalayerClient struct {
	client *datalayer.Client
}

// callbackvalue is the callback value of an asynchronous call.
type callbackvalue struct {
	res datalayer.Result
	val *datalayer.Variant
}

// NewDatalayerClient creates a new client wrapper.
func NewDatalayerClient(c *datalayer.Client) *datalayerClient {
	call := &datalayerClient{client: c}
	return call
}

// Delete deletes the client wrapper.
func (c *datalayerClient) Delete() {
	if c.client == nil {
		return
	}
	datalayer.DeleteClient(c.client)
	c.client = nil
}

// Run runs the operations.
func (c *datalayerClient) Run() {
	c.ping()
	c.read()
	c.create()
	c.remove()
	c.browse()
	c.write()

	c.pingAsync()
	c.readAsync()
	c.createAsync()
	c.removeAsync()
	c.browseAsync()
	c.writeAsync()
}

// ping pings a device.
func (c *datalayerClient) ping() {
	r := c.client.PingSync()
	fmt.Println("ping result: ", r)

	if r != datalayer.ResultOk {
		fmt.Println("ping error: ", r)
		return
	}
}

// read reads a value.
func (c *datalayerClient) read() {
	a := rootStatic + "bool8"
	r, v := c.client.ReadSync(a)
	defer datalayer.DeleteVariant(v)

	if r != datalayer.ResultOk {
		fmt.Println("read error: ", r)
		return
	}

	fmt.Println("Read result: ", r, v)
}

// create creates a node in the address space.
func (c *datalayerClient) create() {
	a := rootDynamic + "_go/lang"
	// Remove node if exist
	c.client.RemoveSync(a)

	// Create new variant
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(false)

	r := c.client.CreateSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("create error: ", r)
		return
	}

	fmt.Println("create result: ", r)
}

// remove removes a node from the address space.
func (c *datalayerClient) remove() {
	a := rootDynamic + "_go/goSync"

	// Create new variant
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetString("will be removed sync")

	// Create node to remove
	r := c.client.CreateSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("create error: ", r)
		return
	}

	// Remove this node
	rem := c.client.RemoveSync(a)
	if rem != datalayer.ResultOk {
		fmt.Println("remove error: ", rem)
		return
	}
	fmt.Println("remove result: ", rem)
}

// browse browses a node's children.
func (c *datalayerClient) browse() {
	r, v := c.client.BrowseSync(rootStatic)
	defer datalayer.DeleteVariant(v)

	if r != datalayer.ResultOk {
		fmt.Println("browse error: ", r)
		return
	}

	fmt.Println("Bbowse result: ", r, v.GetArrayString())
}

// write writes a value to the node.
func (c *datalayerClient) write() {
	a := rootDynamic + "bool8"

	// Create new variant
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(false)

	r := c.client.WriteSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("write error: ", r)
		return
	}
	fmt.Println("Write result: ", r)
}

// pingAsync is similar to ping, but asynchronously.
func (c *datalayerClient) pingAsync() {

	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		done <- callbackvalue{res: res, val: val}
	}

	// Ping the variant asynchronous
	r := c.client.PingAsync(rc)
	if r != datalayer.ResultOk {
		fmt.Println("pingAsync error: ", r)
		return
	}
	fmt.Println("ping result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("ping async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("ping async callback timeout")
	}
	close(done)
}

// readAsync is similar to read, but asynchronously.
func (c *datalayerClient) readAsync() {

	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		if res == datalayer.ResultOk {
			v.Copy(val)
		}
		done <- callbackvalue{res: res, val: val}
	}

	a := rootStatic + "bool8"
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)

	// Read the variant asynchronous
	r := c.client.ReadAsync(a, v, rc)
	if r != datalayer.ResultOk {
		fmt.Println("readAsync error: ", r)
		return
	}
	fmt.Println("read result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("read async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("read async callback timeout")
	}
	close(done)
}

// createAsync is similar to create, but asynchronously.
func (c *datalayerClient) createAsync() {
	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		if res == datalayer.ResultOk && v != nil {
			v.Copy(val)
		}
		done <- callbackvalue{res: res, val: val}
	}

	a := rootDynamic + "_go/lang"
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(true)

	// Create the variant asynchronous
	r := c.client.CreateAsync(a, v, rc)
	if r != datalayer.ResultOk {
		fmt.Println("createAsync error: ", r)
		return
	}
	fmt.Println("create result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("create async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("create async callback timeout")
	}
	close(done)
}

// removeAsync is similar to remove, but asynchronously.
func (c *datalayerClient) removeAsync() {
	a := rootDynamic + "_go/goAsync"

	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		if res == datalayer.ResultOk && v != nil {
			v.Copy(val)
		}
		done <- callbackvalue{res: res, val: val}
	}
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetString("will be removed asynch")

	// Create node to remove
	r := c.client.CreateSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("create result error: ", r)
		return
	}

	// Remove the node asynchronous
	rem := c.client.RemoveAsync(a, rc)
	if r != datalayer.ResultOk {
		fmt.Println("removeAsync error: ", rem)
		return
	}
	fmt.Println("remove result asynchronous: ", rem)

	select {
	case vals := <-done:
		fmt.Println("remove async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("remove async callback timeout")
	}
	close(done)
}

// browseAsync is similar to browse, but asynchronously.
func (c *datalayerClient) browseAsync() {
	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		if res == datalayer.ResultOk {
			v.Copy(val)
		}
		done <- callbackvalue{res: res, val: val}
	}

	r := c.client.BrowseAsync(rootStatic, rc)
	if r != datalayer.ResultOk {
		fmt.Println("browseAsync error: ", r)
		return
	}
	fmt.Println("browse result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("browse async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("browse async callback timeout")
	}
	close(done)
}

// writeAsync is similar to write, but asynchronously.
func (c *datalayerClient) writeAsync() {
	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		if res == datalayer.ResultOk {
			v.Copy(val)
		}
		done <- callbackvalue{res: res, val: val}
	}

	a := rootDynamic + "bool8"
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(true)

	r := c.client.WriteAsync(a, v, rc)
	if r != datalayer.ResultOk {
		fmt.Println("writeAsync error: ", r)
		return
	}
	fmt.Println("write result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("write async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("write async callback timeout")
	}
	close(done)
}

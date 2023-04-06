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
package client

import (
	"fmt"
	"time"

	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
)

var (
	rootS = "sdk-cpp-alldata/static/"
	rootD = "sdk-cpp-alldata/dynamic/"
)

type CallDatalayerClient struct {
	client *datalayer.Client
}

func NewCallDatalayerClient(c *datalayer.Client) *CallDatalayerClient {
	call := &CallDatalayerClient{client: c}
	return call
}

func DeleteCallDatalayerClient(c *CallDatalayerClient) {
	if c == nil {
		return
	}
	if c.client == nil {
		return
	}
	datalayer.DeleteClient(c.client)
	c.client = nil
}

type callbackvalue struct {
	res datalayer.Result
	val *datalayer.Variant
}

func (c *CallDatalayerClient) Run() {
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

func (c *CallDatalayerClient) ping() {
	r := c.client.PingSync()
	fmt.Println("Ping result: ", r)

	if r != datalayer.ResultOk {
		fmt.Println("Ping result error: ", r)
		return
	}
}

func (c *CallDatalayerClient) read() {
	a := rootS + "bool8"
	r, v := c.client.ReadSync(a)
	defer datalayer.DeleteVariant(v)

	if r != datalayer.ResultOk {
		fmt.Println("Read result error: ", r)
		return
	}

	fmt.Println("Read result: ", r, v)
}

func (c *CallDatalayerClient) create() {
	a := rootD + "_go/lang"
	// Remove node if exist
	c.client.RemoveSync(a)

	// Create new variant
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(false)

	r := c.client.CreateSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("Create result error: ", r)
		return
	}

	fmt.Println("Create result: ", r)
}

func (c *CallDatalayerClient) remove() {
	a := rootD + "_go/goSync"

	// Create new variant
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetString("Will be removed synch")

	// Create node to remove
	r := c.client.CreateSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("Create result error: ", r)
		return
	}

	// Remove this node
	rem := c.client.RemoveSync(a)
	if rem != datalayer.ResultOk {
		fmt.Println("Remove result error: ", rem)
		return
	}
	fmt.Println("Remove result: ", rem)
}

func (c *CallDatalayerClient) browse() {
	r, v := c.client.BrowseSync(rootS)
	defer datalayer.DeleteVariant(v)

	if r != datalayer.ResultOk {
		fmt.Println("Browse result error: ", r)
		return
	}

	fmt.Println("Browse result: ", r, v.GetArrayString())
}

func (c *CallDatalayerClient) write() {
	a := rootD + "bool8"

	// Create new variant
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(false)

	r := c.client.WriteSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("Write result error: ", r)
		return
	}
	fmt.Println("Write result: ", r)
}

func (c *CallDatalayerClient) pingAsync() {

	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		v.Copy(val)
		done <- callbackvalue{res: res, val: val}
	}

	// Ping the variant asynchronous
	r := c.client.PingAsync(rc)
	if r != datalayer.ResultOk {
		fmt.Println("pingAsync result error: ", r)
		return
	}
	fmt.Println("Ping result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("Ping async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("Ping async callback timeout")
	}
	close(done)
}

func (c *CallDatalayerClient) readAsync() {

	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		v.Copy(val)
		done <- callbackvalue{res: res, val: val}
	}

	a := rootS + "bool8"
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)

	// Read the variant asynchronous
	r := c.client.ReadAsync(a, v, rc)
	if r != datalayer.ResultOk {
		fmt.Println("readAsync result error: ", r)
		return
	}
	fmt.Println("Read result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("Read async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("Read async callback timeout")
	}
	close(done)
}

func (c *CallDatalayerClient) createAsync() {
	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		v.Copy(val)
		done <- callbackvalue{res: res, val: val}
	}

	a := rootD + "_go/lang"
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(true)

	// Create the variant asynchronous
	r := c.client.CreateAsync(a, v, rc)
	if r != datalayer.ResultOk {
		fmt.Println("createAsync result error: ", r)
		return
	}
	fmt.Println("Create result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("Create async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("Create async callback timeout")
	}
	close(done)
}

func (c *CallDatalayerClient) removeAsync() {
	a := rootD + "_go/goAsync"

	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		v.Copy(val)
		done <- callbackvalue{res: res, val: val}
	}
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetString("Will be removed asynch")

	// Create node to remove
	r := c.client.CreateSync(a, v)
	if r != datalayer.ResultOk {
		fmt.Println("Create result error: ", r)
		return
	}

	// Remove the node asynchronous
	rem := c.client.RemoveAsync(a, rc)
	if r != datalayer.ResultOk {
		fmt.Println("removeAsync result error: ", rem)
		return
	}
	fmt.Println("Remove result asynchronous: ", rem)

	select {
	case vals := <-done:
		fmt.Println("Remove async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("Remove async callback timeout")
	}
	close(done)
}

func (c *CallDatalayerClient) browseAsync() {
	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		v.Copy(val)
		done <- callbackvalue{res: res, val: val}
	}

	r := c.client.BrowseAsync(rootS, rc)
	if r != datalayer.ResultOk {
		fmt.Println("browseAsync result error: ", r)
		return
	}
	fmt.Println("Browse result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("Browse async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("Browse async callback timeout")
	}
	close(done)
}

func (c *CallDatalayerClient) writeAsync() {
	done := make(chan callbackvalue)

	rc := func(res datalayer.Result, v *datalayer.Variant) {
		val := datalayer.NewVariant()
		v.Copy(val)
		done <- callbackvalue{res: res, val: val}
	}

	a := rootD + "bool8"
	v := datalayer.NewVariant()
	defer datalayer.DeleteVariant(v)
	v.SetBool8(true)

	r := c.client.WriteAsync(a, v, rc)
	if r != datalayer.ResultOk {
		fmt.Println("writeAsync result error: ", r)
		return
	}
	fmt.Println("Write result asynchronous: ", r)

	select {
	case vals := <-done:
		fmt.Println("Write async ", vals.res)
		datalayer.DeleteVariant(vals.val)
	case <-time.After(time.Second * 5):
		fmt.Println("Write async callback timeout")
	}
	close(done)
}

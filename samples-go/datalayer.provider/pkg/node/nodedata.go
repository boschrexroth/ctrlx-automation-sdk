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
package node

import (
	"fmt"

	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
)

const addressbase = "sdk-go-datalayer-provider/"

// NodeDataHandler is an interface for accessing NodeData
type NodeDataHandler interface {
	Name() string
	Node() *datalayer.ProviderNode
	Value() *datalayer.Variant
	OnMetadata() (datalayer.Result, *datalayer.Variant)
}

// NodeData contains basic elements for managing provider nodes
type NodeData struct {
	name string
	node *datalayer.ProviderNode
	val  *datalayer.Variant
}

// DeleteNodeData removes ctrlX Data Layer elements
func DeleteNodeData(n *NodeData) {
	if n == nil {
		return
	}
	datalayer.DeleteProviderNode(n.node)
	datalayer.DeleteVariant(n.val)
}

func (n *NodeData) Name() string {
	return n.name
}

func (n *NodeData) Node() *datalayer.ProviderNode {
	return n.node
}

func (n *NodeData) Value() *datalayer.Variant {
	return n.val
}

// StartNodeDataHandler is a handler function for ctrlX Data Layer Provider callbacks.
// NodeData is accessed via the interface NodeDataHandler
func StartNodeDataHandler(d NodeDataHandler) {
	for {
		if d.Node() == nil {
			return
		}
		select {
		case event, ok := <-d.Node().Channels().OnCreate:
			if !ok {
				return
			}
			fmt.Println("event: oncreate: ", d.Name())
			event.Callback(datalayer.Result(0), nil)

		case event, ok := <-d.Node().Channels().OnRemove:
			if !ok {
				return
			}

			fmt.Println("event: OnRemove: ", d.Name())
			event.Callback(datalayer.ResultUnsupported, nil)

		case event, ok := <-d.Node().Channels().OnBrowse:
			func() {
				if !ok {
					return
				}

				newData := datalayer.NewVariant()
				defer datalayer.DeleteVariant(newData)
				newData.SetArrayString([]string{})
				fmt.Println("event: OnBrowse: ", d.Name())
				event.Callback(datalayer.Result(0), newData)
			}()

		case event, ok := <-d.Node().Channels().OnRead:
			func() {
				if !ok {
					return
				}

				newData := datalayer.NewVariant()
				defer datalayer.DeleteVariant(newData)
				d.Value().Copy(newData)
				fmt.Println("event: OnRead: ", d.Name())
				event.Callback(datalayer.Result(0), newData)
			}()

		case event, ok := <-d.Node().Channels().OnWrite:
			if !ok {
				return
			}

			event.Data.Copy(d.Value())
			fmt.Println("event: OnWrite: ", d.Name())
			event.Callback(datalayer.Result(0), event.Data)

		case event, ok := <-d.Node().Channels().OnMetadata:
			func() {
				if !ok {
					return
				}

				fmt.Println("event: OnMetadata: ", d.Name())

				r, v := d.OnMetadata()
				defer datalayer.DeleteVariant(v)
				event.Callback(r, v)
			}()

		case <-d.Node().Channels().Done:
			fmt.Println("event: Done: ", d.Name())
			return
		}
	}
}

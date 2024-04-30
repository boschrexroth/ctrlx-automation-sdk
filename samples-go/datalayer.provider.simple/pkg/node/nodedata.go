/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"fmt"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
)

const addressroot = "sdk/go/provider/simple/"

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
			if !ok {
				return
			}

			func(e datalayer.ProviderNodeEvent) {
				newData := datalayer.NewVariant()
				defer datalayer.DeleteVariant(newData)
				newData.SetArrayString([]string{})
				fmt.Println("event: OnBrowse: ", d.Name())
				e.Callback(datalayer.Result(0), newData)
			}(event)

		case event, ok := <-d.Node().Channels().OnRead:
			if !ok {
				return
			}

			func(e datalayer.ProviderNodeEventData) {
				newData := datalayer.NewVariant()
				defer datalayer.DeleteVariant(newData)
				d.Value().Copy(newData)
				fmt.Println("event: OnRead: ", d.Name())
				e.Callback(datalayer.Result(0), newData)
			}(event)

		case event, ok := <-d.Node().Channels().OnWrite:
			if !ok {
				return
			}

			event.Data.Copy(d.Value())
			fmt.Println("event: OnWrite: ", d.Name())
			event.Callback(datalayer.Result(0), event.Data)

		case event, ok := <-d.Node().Channels().OnMetadata:
			if !ok {
				return
			}

			func(e datalayer.ProviderNodeEvent) {
				fmt.Println("event: OnMetadata: ", d.Name())
				r, v := d.OnMetadata()
				defer datalayer.DeleteVariant(v)
				e.Callback(r, v)
			}(event)

		case <-d.Node().Channels().Done:
			fmt.Println("event: Done: ", d.Name())
			return
		}
	}
}

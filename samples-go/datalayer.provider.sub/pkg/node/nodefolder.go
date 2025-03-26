/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"fmt"
	"sync"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/fbs/comm/datalayer"
)

const addressroot = "sdk/go/provider/sub/**"

// NodeFolder contains basic elements for managing provider nodes
type NodeFolder struct {
	name          string
	node          *datalayer.ProviderNode
	val           *datalayer.Variant
	Subscriptions map[uint64]*ClientPublisher
	SubMutex      sync.RWMutex
}

// NewNodeFolder creates a NodeFolder instance
func NewNodeFolder() *NodeFolder {
	nf := &NodeFolder{name: addressroot,
		node:          datalayer.NewProviderNodeSub(),
		val:           datalayer.NewVariant(),
		Subscriptions: make(map[uint64]*ClientPublisher),
	}
	nf.val.SetInt32(123)
	return nf
}

// DeleteNodeFolder removes ctrlX Data Layer elements
func DeleteNodeFolder(n *NodeFolder) {
	if n == nil {
		return
	}
	datalayer.DeleteProviderNode(n.node)
	datalayer.DeleteVariant(n.val)
}

func (n *NodeFolder) Name() string {
	return n.name
}

func (n *NodeFolder) Node() *datalayer.ProviderNode {
	return n.node
}

func (n *NodeFolder) Value() *datalayer.Variant {
	return n.val
}

// StartNodeFolderHandler is a handler function for ctrlX Data Layer Provider callbacks.
func StartNodeFolderHandler(d *NodeFolder) {
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
				m := datalayer.NewMetaDataBuilder(datalayer.AllowedOperationRead|datalayer.AllowedOperationWrite, "folder", "folder_url")
				m.NodeClass(fbs.NodeClassFolder)
				v := m.Build()
				defer datalayer.DeleteVariant(v)
				e.Callback(datalayer.ResultOk, v)
			}(event)
		case event, ok := <-d.Node().Channels().OnSubscribe:
			func() {
				if !ok {
					return
				}

				fmt.Println("event: OnSubscribe: ", d.Name(), event.Address, event.Subsciption.GetUniqueId())
				fmt.Println("    Props: PublishInterval:", event.Subsciption.GetProps().PublishInterval())
				_, ok := d.Subscriptions[event.Subsciption.GetUniqueId()]
				if ok {
					return
				}
				d.SubMutex.Lock()
				cp := NewClientPublisher(event.Subsciption, &d.SubMutex)
				d.Subscriptions[event.Subsciption.GetUniqueId()] = cp
				d.SubMutex.Unlock()
				cp.Start()
			}()

		case event, ok := <-d.Node().Channels().OnUnsubscribe:
			func() {
				if !ok {
					return
				}

				fmt.Println("event: OnUnsubscribe: ", d.Name(), event.Address, event.Subsciption.GetUniqueId())
				if len(event.Subsciption.GetNodes()) != 0 {
					return
				}
				cp := d.Subscriptions[event.Subsciption.GetUniqueId()]
				if cp != nil {
					cp.Stop()
				}
				d.SubMutex.Lock()
				delete(d.Subscriptions, event.Subsciption.GetUniqueId())
				d.SubMutex.Unlock()
			}()

		case <-d.Node().Channels().Done:
			fmt.Println("event: Done: ", d.Name())
			return
		}
	}
}

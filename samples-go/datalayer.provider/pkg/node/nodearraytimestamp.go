/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"time"

	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/pkg/fbs/comm/datalayer"
)

const typeaddressarraytimestamp = "types/datalayer/array-of-timestamp"

// NodeDataTimestamp is a specialization of the NodeData
// and describes the access and handling for type of the 'array of timestamp'.
type NodeDataArrayTimestamp struct {
	NodeData
}

func NewNodeDataArrayTimestamp(n string) *NodeDataArrayTimestamp {
	nd := &NodeDataArrayTimestamp{NodeData{name: addressbase + n, node: datalayer.NewProviderNode(), val: datalayer.NewVariant()}}
	times := []time.Time{
		time.Date(2014, 7, 13, 0, 0, 0, 0, time.UTC),
		time.Date(1990, 6, 8, 0, 0, 0, 0, time.UTC),
	}
	nd.val.SetArrayTime(times)
	return nd
}

// OnMetadata descript 'array of timestamp' MetaData of the provider node
func (n *NodeDataArrayTimestamp) OnMetadata() (datalayer.Result, *datalayer.Variant) {
	m := datalayer.NewMetaDataBuilder(datalayer.AllowedOperationRead, "array-of-timestamp variable", "array-of-timestamp_variable_url")
	m.Unit("ns").DisplayName("array-of-timestamp").NodeClass(fbs.NodeClassVariable)
	m.AddReference(datalayer.ReferenceTypeRead, typeaddressarraytimestamp)
	v := m.Build()
	return datalayer.ResultOk, v
}

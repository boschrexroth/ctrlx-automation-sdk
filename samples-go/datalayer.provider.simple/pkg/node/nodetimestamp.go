/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"time"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/fbs/comm/datalayer"
)

const typeaddresstimestamp = "types/datalayer/timestamp"

// NodeDataTimestamp is a specialization of the NodeData
// and describes the access and handling for type of the 'timestamp'.
type NodeDataTimestamp struct {
	NodeData
}

func NewNodeDataTimestamp(n string) *NodeDataTimestamp {
	nd := &NodeDataTimestamp{NodeData{name: addressroot + n, node: datalayer.NewProviderNode(), val: datalayer.NewVariant()}}
	nd.val.SetTime(time.Date(2014, 7, 13, 0, 0, 0, 0, time.UTC))
	return nd
}

// OnMetadata descript 'timestamp' MetaData of the provider node
func (n *NodeDataTimestamp) OnMetadata() (datalayer.Result, *datalayer.Variant) {
	m := datalayer.NewMetaDataBuilder(datalayer.AllowedOperationRead, "timestamp variable", "timestamp_variable_url")
	m.Unit("ns").DisplayName("timestamp").NodeClass(fbs.NodeClassVariable)
	m.AddReference(datalayer.ReferenceTypeRead, typeaddresstimestamp)
	v := m.Build()
	return datalayer.ResultOk, v
}

/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/fbs/comm/datalayer"
)

const typeaddressstring = "types/datalayer/string"

// NodeDataString is a specialization of the NodeData
// and describes the access and handling for type of the 'string'.
type NodeDataString struct {
	NodeData
}

func NewNodeDataString(n string) *NodeDataString {
	nd := &NodeDataString{NodeData{name: addressroot + n, node: datalayer.NewProviderNode(), val: datalayer.NewVariant()}}
	nd.val.SetString("test")
	return nd
}

// OnMetadata descript 'string' MetaData of the provider node
func (n *NodeDataString) OnMetadata() (datalayer.Result, *datalayer.Variant) {
	m := datalayer.NewMetaDataBuilder(datalayer.AllowedOperationRead|datalayer.AllowedOperationWrite, "String variable", "String_variable_url")
	m.Unit("-").DisplayName("string-value").NodeClass(fbs.NodeClassVariable)
	m.AddReference(datalayer.ReferenceTypeRead, typeaddressstring)
	m.AddReference(datalayer.ReferenceTypeWrite, typeaddressstring)
	v := m.Build()
	return datalayer.ResultOk, v
}

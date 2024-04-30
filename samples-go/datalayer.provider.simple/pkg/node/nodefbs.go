/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
package node

import (
	"fmt"
	"os"
	"path"

	"github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/v2/pkg/fbs/comm/datalayer"

	"datalayer.provider/pkg/sample/schema"
	flatbuffers "github.com/google/flatbuffers/go"
)

const bfbsfile = "sampleSchema.bfbs"
const typeaddressfbs = "types/" + addressroot + "inertial-value"

// NodeDataFbs is a specialization of the NodeData
// and describes the access and handling for type of the 'flatbuffer'.
type NodeDataFbs struct {
	NodeData
}

func NewNodeDataFbs(n string, p *datalayer.Provider) *NodeDataFbs {
	nd := &NodeDataFbs{NodeData{name: addressroot + n, node: datalayer.NewProviderNode()}}

	nd.registerBfbs(p)

	// set flatbuffer values
	builder := flatbuffers.NewBuilder(1024)
	schema.InertialValueStart(builder)
	schema.InertialValueAddX(builder, 127)
	schema.InertialValueAddY(builder, 128)
	schema.InertialValueAddZ(builder, 129)
	iv := schema.InertialValueEnd(builder)
	builder.Finish(iv)
	v := datalayer.NewVariant()
	v.SetFlatbuffers(builder.FinishedBytes())
	nd.val = v
	return nd
}

func (nd *NodeDataFbs) registerBfbs(p *datalayer.Provider) {
	wd, _ := os.Getwd()
	bfbs := path.Join(wd, "bfbs")
	bfbs = path.Join(bfbs, bfbsfile)
	if os.Getenv("SNAP") != "" {
		bfbs = path.Join(os.Getenv("SNAP"), bfbsfile)
	}
	r := p.RegisterType(typeaddressfbs, bfbs)
	if r != datalayer.ResultOk {
		fmt.Println("RegisterType node ", nd.Name(), " failed: ", r)
	}
}

// OnMetadata descript 'flatbuffers' MetaData of the provider node
func (n *NodeDataFbs) OnMetadata() (datalayer.Result, *datalayer.Variant) {
	m := datalayer.NewMetaDataBuilder(datalayer.AllowedOperationRead|datalayer.AllowedOperationWrite|datalayer.AllowedOperationBrowse, "Simple flatbuffers variable", "")
	m.Unit("g").DisplayName("inertial-value").NodeClass(fbs.NodeClassVariable)
	m.AddReference(datalayer.ReferenceTypeRead, typeaddressfbs)
	m.AddReference(datalayer.ReferenceTypeWrite, typeaddressfbs)
	v := m.Build()
	return datalayer.ResultOk, v
}

func (n *NodeDataFbs) UnregisterType(p *datalayer.Provider) {
	r := p.UnregisterType(typeaddressfbs)
	if r != datalayer.ResultOk {
		fmt.Println("UnregisterType node ", n.Name(), " failed: ", r)
	}
}

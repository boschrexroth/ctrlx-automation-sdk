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
	"os"
	"path"

	"datalayer.provider/pkg/sample/schema"
	"datalayer.provider/pkg/utils"
	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/pkg/fbs/comm/datalayer"
	flatbuffers "github.com/google/flatbuffers/go"
)

const bfbsfile = "sampleSchema.bfbs"
const typeaddressfbs = "types/sample-go/schema/inertial-value"

// NodeDataFbs is a specialization of the NodeData
// and describes the access and handling for type of the 'flatbuffer'.
type NodeDataFbs struct {
	NodeData
}

func NewNodeDataFbs(n string, p *datalayer.Provider) *NodeDataFbs {
	nd := &NodeDataFbs{NodeData{name: addressbase + n, node: datalayer.NewProviderNode()}}

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
		fmt.Println("ERROR RegisterType node ", nd.Name(), " failed: ", r)
	}
}

// OnMetadata descript 'flatbuffers' MetaData of the provider node
func (n *NodeDataFbs) OnMetadata() (datalayer.Result, *datalayer.Variant) {
	m := utils.NewMetaDataBuilder(utils.AllowedOperationRead|utils.AllowedOperationWrite|utils.AllowedOperationBrowse, "Simple flatbuffers variable", "")
	m.Unit("g").DisplayName("inertial-value").NodeClass(fbs.NodeClassVariable)
	m.AddReference(utils.ReferenceTypeRead, typeaddressfbs)
	m.AddReference(utils.ReferenceTypeWrite, typeaddressfbs)
	v := m.Build()
	return datalayer.ResultOk, v
}

func (n *NodeDataFbs) UnregisterType(p *datalayer.Provider) {
	r := p.UnregisterType(typeaddressfbs)
	if r != datalayer.ResultOk {
		fmt.Println("ERROR UnregisterType node ", n.Name(), " failed: ", r)
	}
}

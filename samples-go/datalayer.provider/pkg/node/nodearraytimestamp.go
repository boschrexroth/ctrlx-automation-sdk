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

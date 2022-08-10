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
	"datalayer.provider/pkg/utils"
	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/pkg/fbs/comm/datalayer"
)

const typeaddressstring = "types/datalayer/string"

// NodeDataString is a specialization of the NodeData
// and describes the access and handling for type of the 'string'.
type NodeDataString struct {
	NodeData
}

func NewNodeDataString(n string) *NodeDataString {
	nd := &NodeDataString{NodeData{name: addressbase + n, node: datalayer.NewProviderNode(), val: datalayer.NewVariant()}}
	nd.val.SetString("test")
	return nd
}

// OnMetadata descript 'string' MetaData of the provider node
func (n *NodeDataString) OnMetadata() (datalayer.Result, *datalayer.Variant) {
	m := utils.NewMetaDataBuilder(utils.AllowedOperationRead|utils.AllowedOperationBrowse|utils.AllowedOperationWrite, "String variable", "String_variable_url")
	m.Unit("-").DisplayName("string-value").NodeClass(fbs.NodeClassVariable)
	m.AddReference(utils.ReferenceTypeRead, typeaddressstring)
	m.AddReference(utils.ReferenceTypeWrite, typeaddressstring)
	m.AddReference(utils.ReferenceTypeCreate, typeaddressstring)
	v := m.Build()
	return datalayer.ResultOk, v
}

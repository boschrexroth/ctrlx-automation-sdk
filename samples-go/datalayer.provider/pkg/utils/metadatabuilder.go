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
package utils

import (
	"sort"

	"github.com/boschrexroth/ctrlx-datalayer-golang/pkg/datalayer"
	fbs "github.com/boschrexroth/ctrlx-datalayer-golang/pkg/fbs/comm/datalayer"
	flatbuffers "github.com/google/flatbuffers/go"
)

type AllowedOperation uint

const (
	AllowedOperationNone   AllowedOperation = 0x00000
	AllowedOperationRead   AllowedOperation = 0x00001
	AllowedOperationWrite  AllowedOperation = 0x00010
	AllowedOperationCreate AllowedOperation = 0x00100
	AllowedOperationDelete AllowedOperation = 0x01000
	AllowedOperationBrowse AllowedOperation = 0x10000
	AllowedOperationAll    AllowedOperation = AllowedOperationRead | AllowedOperationWrite | AllowedOperationCreate | AllowedOperationDelete | AllowedOperationBrowse
)

type ReferenceType uint8

const (
	ReferenceTypeRead     ReferenceType = 0
	ReferenceTypeReadIn   ReferenceType = 1
	ReferenceTypeReadOut  ReferenceType = 2
	ReferenceTypeWrite    ReferenceType = 3
	ReferenceTypeWriteIn  ReferenceType = 4
	ReferenceTypeWriteOut ReferenceType = 5
	ReferenceTypeCreate   ReferenceType = 6
	ReferenceTypeUses     ReferenceType = 7
	ReferenceTypeHasSave  ReferenceType = 8
)

var EnumNamesReferenceType = map[ReferenceType]string{
	ReferenceTypeRead:     "readType",
	ReferenceTypeReadIn:   "readInType",
	ReferenceTypeReadOut:  "readOutType",
	ReferenceTypeWrite:    "writeType",
	ReferenceTypeWriteIn:  "writeInType",
	ReferenceTypeWriteOut: "writeOutType",
	ReferenceTypeCreate:   "createType",
	ReferenceTypeUses:     "uses",
	ReferenceTypeHasSave:  "hasSave",
}

func ref2name(r ReferenceType) string {
	if s, ok := EnumNamesReferenceType[r]; ok {
		return s
	}
	return ""
}

type MetaDataBuilder struct {
	name           string
	desciption     string
	descriptionurl string
	unit           string
	allowed        AllowedOperation
	nodeclass      fbs.NodeClass
	displayformat  fbs.DisplayFormat
	refers         map[string]string
	extensions     map[string]string
}

func NewMetaDataBuilder(a AllowedOperation, d string, du string) *MetaDataBuilder {
	m := &MetaDataBuilder{desciption: d, descriptionurl: du}
	m.name = ""
	m.unit = ""
	m.nodeclass = fbs.NodeClassNode
	m.displayformat = fbs.DisplayFormatAuto
	m.refers = make(map[string]string)
	m.extensions = make(map[string]string)
	m.Operations(a)
	return m
}

func (m *MetaDataBuilder) Build() *datalayer.Variant {
	builder := flatbuffers.NewBuilder(1024)

	dbs := builder.CreateString(m.desciption)
	dubs := builder.CreateString(m.descriptionurl)
	nbs := builder.CreateString(m.name)
	ubs := builder.CreateString(m.unit)

	//Serialize References data
	references := m.buildreferences(builder)

	//Serialize Extensions data
	extensions := m.buildextensions(builder)

	//Serialize AllowedOperations data
	operations := m.operations(builder)

	//Serialize Metadata data
	fbs.MetadataStart(builder)
	fbs.MetadataAddNodeClass(builder, m.nodeclass)
	fbs.MetadataAddOperations(builder, operations)
	fbs.MetadataAddDescription(builder, dbs)
	fbs.MetadataAddDescriptionUrl(builder, dubs)
	fbs.MetadataAddDisplayName(builder, nbs)
	fbs.MetadataAddDisplayFormat(builder, m.displayformat)
	fbs.MetadataAddUnit(builder, ubs)

	fbs.MetadataAddReferences(builder, references)
	fbs.MetadataAddExtensions(builder, extensions)

	md := fbs.MetadataEnd(builder)
	builder.Finish(md)
	v := datalayer.NewVariant()
	v.SetFlatbuffers(builder.FinishedBytes())
	return v
}

func (m *MetaDataBuilder) buildextensions(builder *flatbuffers.Builder) flatbuffers.UOffsetT {

	extensionsOffsets := []flatbuffers.UOffsetT{}
	keys := sortkeys(m.extensions)

	for _, k := range keys {
		re := m.addExt(builder, k, m.extensions[k])
		extensionsOffsets = append(extensionsOffsets, re)
	}
	extensionsLength := len(extensionsOffsets)
	fbs.MetadataStartExtensionsVector(builder, len(extensionsOffsets))
	for j := extensionsLength - 1; j >= 0; j-- {
		builder.PrependUOffsetT(extensionsOffsets[j])
	}
	return builder.EndVector(extensionsLength)
}

func (m *MetaDataBuilder) addExt(builder *flatbuffers.Builder, k string, v string) flatbuffers.UOffsetT {
	kbs := builder.CreateString(k)
	vbs := builder.CreateString(v)
	fbs.ExtensionStart(builder)
	fbs.ExtensionAddKey(builder, kbs)
	fbs.ExtensionAddValue(builder, vbs)
	return fbs.ExtensionEnd(builder)
}

func sortkeys(m map[string]string) []string {
	keys := make([]string, 0, len(m))
	for k := range m {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	return keys
}

func (m *MetaDataBuilder) buildreferences(builder *flatbuffers.Builder) flatbuffers.UOffsetT {

	referencesOffsets := []flatbuffers.UOffsetT{}
	keys := sortkeys(m.refers)
	for _, k := range keys {
		//fmt.Println("refs: ", k, m.refers[k])
		re := m.addRef(builder, k, m.refers[k])
		referencesOffsets = append(referencesOffsets, re)
	}

	fbs.MetadataStartReferencesVector(builder, len(referencesOffsets))
	referencesLength := len(referencesOffsets)
	for j := referencesLength - 1; j >= 0; j-- {
		builder.PrependUOffsetT(referencesOffsets[j])
	}
	return builder.EndVector(referencesLength)
}

func (m *MetaDataBuilder) addRef(builder *flatbuffers.Builder, t string, p string) flatbuffers.UOffsetT {
	tpbs := builder.CreateString(p)
	rtbs := builder.CreateString(t)
	fbs.ReferenceStart(builder)
	fbs.ReferenceAddType(builder, rtbs)
	fbs.ReferenceAddTargetAddress(builder, tpbs)
	return fbs.ReferenceEnd(builder)
}

func (m *MetaDataBuilder) operations(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	fbs.AllowedOperationsStart(builder)
	fbs.AllowedOperationsAddCreate(builder, m.isallowed(AllowedOperationCreate))
	fbs.AllowedOperationsAddDelete(builder, m.isallowed(AllowedOperationDelete))
	fbs.AllowedOperationsAddRead(builder, m.isallowed(AllowedOperationRead))
	fbs.AllowedOperationsAddWrite(builder, m.isallowed(AllowedOperationWrite))
	fbs.AllowedOperationsAddBrowse(builder, m.isallowed(AllowedOperationBrowse))
	operations := fbs.AllowedOperationsEnd(builder)
	return operations
}

func (m *MetaDataBuilder) Operations(a AllowedOperation) *MetaDataBuilder {
	m.allowed = a
	return m
}

func (m *MetaDataBuilder) isallowed(a AllowedOperation) bool {
	return m.allowed&a == a
}

func (m *MetaDataBuilder) Unit(u string) *MetaDataBuilder {
	m.unit = u
	return m
}

func (m *MetaDataBuilder) DisplayName(n string) *MetaDataBuilder {
	m.name = n
	return m
}

func (m *MetaDataBuilder) NodeClass(nc fbs.NodeClass) *MetaDataBuilder {
	m.nodeclass = nc
	return m
}

func (m *MetaDataBuilder) DisplayFormat(df fbs.DisplayFormat) *MetaDataBuilder {
	m.displayformat = df
	return m
}

func (m *MetaDataBuilder) AddReference(r ReferenceType, t string) *MetaDataBuilder {
	m.refers[ref2name(r)] = t
	return m
}

func (m *MetaDataBuilder) AddExtension(k string, v string) *MetaDataBuilder {
	m.extensions[k] = v
	return m
}

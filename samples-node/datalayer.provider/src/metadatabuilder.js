// Copyright (c) 2021 Bosch Rexroth AG

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
const flatbuffers = require('flatbuffers');
const metadata_generated = require('ctrlx-datalayer-nodejs/fbs/js/metadata_generated');

module.exports = class MetadataBuilder {

    constructor() {
        this.builder = new flatbuffers.Builder(1024);
        this.descriptionOffset = this.builder.createString("");
        this.descriptionUrlOffset = this.builder.createString("");
        this.displayNameOffset = this.builder.createString("");
        this.unitOffset = this.builder.createString("");
        this.nodeClass = metadata_generated.comm.datalayer.NodeClass.Variable;
        this.references = [];
        this.extensions = [];
    }

    setAllowedOperations($read, $write, $create, $delete, $browse) {
        this.operationsOffset = metadata_generated.comm.datalayer.AllowedOperations.createAllowedOperations(
            this.builder, $read, $write, $create, $delete, $browse);
        return this;
    }

    setDescription(description) {
        this.descriptionOffset = this.builder.createString(description);
        return this;
    }

    setDescriptionUrl(descriptionUrl) {
        this.descriptionUrlOffset = this.builder.createString(descriptionUrl); 
        return this;
    }

    setDisplayName(displayName) {
        this.displayNameOffset = this.builder.createString(displayName); 
        return this;
    }

    setUnit(unit) {
        this.unitOffset = this.builder.createString(unit);
        return this;
    }

    setNodeClass(nodeClass) {
        this.nodeClass = metadata_generated.comm.datalayer.NodeClass[nodeClass];
        return this;
    }

    addReference(type, targetAddress) {
        var typeOffset = this.builder.createString(type);
        var targetAddressOffset = this.builder.createString(targetAddress);
        var reference = metadata_generated.comm.datalayer.Reference.createReference(this.builder, typeOffset, targetAddressOffset);
        this.references.push(reference);
        return this;
    }

    addExtensions(key, value) {
        var keyOffset = this.builder.createString(key);
        var valueOffset = this.builder.createString(value);
        var extensionOffset = metadata_generated.comm.datalayer.Reference.createExtension(this.builder, keyOffset, valueOffset);
        this.references.push(extensionOffset);
        return this;
    }

    build() {
        var referencesVector = metadata_generated.comm.datalayer.Metadata.createReferencesVector(this.builder, this.references);
        var extensionsVector = metadata_generated.comm.datalayer.Metadata.createExtensionsVector(this.builder, this.extensions);
        metadata_generated.comm.datalayer.Metadata.startMetadata(this.builder);
        metadata_generated.comm.datalayer.Metadata.addDescription(this.builder, this.descriptionOffset);
        metadata_generated.comm.datalayer.Metadata.addDescriptionUrl(this.builder, this.descriptionUrlOffset); 
        metadata_generated.comm.datalayer.Metadata.addDisplayName(this.builder, this.displayNameOffset);
        metadata_generated.comm.datalayer.Metadata.addUnit(this.builder, this.unitOffset);
        metadata_generated.comm.datalayer.Metadata.addNodeClass(this.builder, this.nodeClass);
        metadata_generated.comm.datalayer.Metadata.addOperations(this.builder, this.operationsOffset);
        metadata_generated.comm.datalayer.Metadata.addReferences(this.builder, referencesVector);
        metadata_generated.comm.datalayer.Metadata.addExtensions(this.builder, extensionsVector);
        var offset = metadata_generated.comm.datalayer.Metadata.endMetadata(this.builder);
        metadata_generated.comm.datalayer.Metadata.finishMetadataBuffer(this.builder, offset);
        return this.builder.asUint8Array();
    }
}


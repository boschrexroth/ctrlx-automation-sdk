# MIT License
#
# Copyright (c) 2020-2021 Bosch Rexroth AG
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import datalayer
from datalayer.provider import Provider
from datalayer.provider_node import ProviderNodeCallbacks, NodeCallback
from datalayer.variant import Result, Variant

import flatbuffers
from comm.datalayer import Metadata, NodeClass, AllowedOperations, Reference

class ProviderNodeAllData:

    def __init__(self, 
        provider: Provider, 
        addressType: str, 
        address: str, 
        name : str, 
        unit : str, 
        description : str, 
        dynamic: bool,
        data : Variant):

        self.provider = provider

        self.address = address
        self.name= name
        self.dynamic = dynamic
        self.data = data

        self.cbs = ProviderNodeCallbacks(
            self.__on_create,
            self.__on_remove,
            self.__on_browse,
            self.__on_read,
            self.__on_write,
            self.__on_metadata
        )
        self.providerNode = datalayer.provider_node.ProviderNode(self.cbs)

        self.metadata = self.create_metadata(addressType, unit, description)

    def create_metadata(self, addressType: str, unit: str, description: str):

        # Create `FlatBufferBuilder`instance. Initial Size 1024 bytes (grows automatically if needed)
        builder = flatbuffers.Builder(1024)

        # Serialize AllowedOperations data
        AllowedOperations.AllowedOperationsStart(builder)
        AllowedOperations.AllowedOperationsAddRead(builder, True)
        AllowedOperations.AllowedOperationsAddWrite(builder, self.dynamic)
        AllowedOperations.AllowedOperationsAddCreate(builder, False)
        AllowedOperations.AllowedOperationsAddDelete(builder, False)
        operations = AllowedOperations.AllowedOperationsEnd(builder)

        # Metadata description strings
        descriptionBuilderString = builder.CreateString(description)
        urlBuilderString = builder.CreateString("tbd")
        displayNameString = builder.CreateString(self.name)
        unitString = builder.CreateString(unit)

        # Store string parameter into builder
        readTypeBuilderString = builder.CreateString("readType")
        writeTypeBuilderString = builder.CreateString("writeType")
        targetAddressBuilderString = builder.CreateString(addressType)

        # Serialize Reference data (for read operation)
        Reference.ReferenceStart(builder)
        Reference.ReferenceAddType(builder, readTypeBuilderString)
        Reference.ReferenceAddTargetAddress(
            builder, targetAddressBuilderString)
        reference_read = Reference.ReferenceEnd(builder)

        # Serialize Reference data (for write operation)
        Reference.ReferenceStart(builder)
        Reference.ReferenceAddType(builder, writeTypeBuilderString)
        Reference.ReferenceAddTargetAddress(
            builder, targetAddressBuilderString)
        reference_write = Reference.ReferenceEnd(builder)

        # Create FlatBuffer vector and prepend reference data. Note: Since we prepend the data, prepend them in reverse order.
        Metadata.MetadataStartReferencesVector(builder, 2)
        builder.PrependSOffsetTRelative(reference_write)
        builder.PrependSOffsetTRelative(reference_read)
        references = builder.EndVector(2)

        # Serialize Metadata data
        Metadata.MetadataStart(builder)
        Metadata.MetadataAddNodeClass(builder, NodeClass.NodeClass.Variable)
        Metadata.MetadataAddOperations(builder, operations)
        Metadata.MetadataAddDescription(builder, descriptionBuilderString)
        Metadata.MetadataAddDescriptionUrl(builder, urlBuilderString)
        Metadata.MetadataAddDisplayName(builder, displayNameString)
        Metadata.MetadataAddUnit(builder, unitString)

        # Metadata reference table
        Metadata.MetadataAddReferences(builder, references)
        metadata_internal = Metadata.MetadataEnd(builder)

        # Closing operation
        builder.Finish(metadata_internal)
        
        metadata = Variant()
        metadata.set_flatbuffers(builder.Output())
        return metadata



    def __on_create(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        # print("__on_create", address)
        cb(Result.OK, None)

    def __on_remove(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # Not implemented because no wildcard is registered
        print("__on_remove", address)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # print("__on_browse", address)
        cb(Result.OK, None)
        
    def __on_read(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        # print("__on_read", address) - this command slows the performance down
        new_data = self.data
        cb(Result.OK, new_data)

    def __on_write(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):

        if self.dynamic is False:
            print("__on_write PERMISSION_DENIED", address, data.get_type())
            cb(Result.PERMISSION_DENIED, None)
            return

        if self.data.get_type() != data.get_type():
            print("__on_write TYPE_MISMATCH", address, data.get_type())
            cb(Result.TYPE_MISMATCH, None)
            return

        print("__on_write", address, data.get_type())
        _, self.data = data.clone()
        cb(Result.OK, data)

    def __on_metadata(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # print("__on_metadata", address)
        cb(Result.OK, self.metadata)

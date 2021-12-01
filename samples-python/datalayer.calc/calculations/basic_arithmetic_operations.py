# MIT License
#
# Copyright (c) 2021 Bosch Rexroth AG
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

import typing

import flatbuffers
from comm.datalayer import Metadata, NodeClass, AllowedOperations, Reference

import datalayer
from datalayer.provider import Provider
from datalayer.client import Client
from datalayer.provider_node import ProviderNodeCallbacks, NodeCallback
from datalayer.variant import Result, Variant, VariantType

type_address_string = "types/datalayer/string"

class BasicArithmeticOperations:

    def __init__(self, provider: Provider, client: Client,  addressRoot: str, id: str, mode : str):

        self.provider = provider
        self.client = client

        self.addressRoot = addressRoot + "/" + id + "/"

        self.in1_address = Variant()
        self.in1_address.set_string(
            "framework/metrics/system/cpu-utilisation-percent")
        self.in1_value = Variant()
        data = self.create_metadata(type_address_string, id, "", "Enter address of input value 1 here", True)
        self.in1_metadata = Variant()
        self.in1_metadata.set_flatbuffers(data)

        self.in2_address = Variant()
        self.in2_address.set_string(
            "framework/metrics/system/cpu-utilisation-percent")
        self.in2_value = Variant()
        data = self.create_metadata(type_address_string, id, "", "Enter address of input value 2 here", True)
        self.in2_metadata = Variant()
        self.in2_metadata.set_flatbuffers(data)

        self.mode = Variant()
        self.mode.set_string(mode)
        data = self.create_metadata(type_address_string, id, "", "Enter the operation mode here: + - / *", True)
        self.mode_metadata = Variant()
        self.mode_metadata.set_flatbuffers(data)

        self.out = Variant()
        data = self.create_metadata(type_address_string, id, "", "The result of the operation", False)
        self.out_metadata = Variant()
        self.out_metadata.set_flatbuffers(data)
        self.out_error = True

        self.cbs = ProviderNodeCallbacks(
            self.__on_create,
            self.__on_remove,
            self.__on_browse,
            self.__on_read,
            self.__on_write,
            self.__on_metadata
        )

        self.providerNode = datalayer.provider_node.ProviderNode(self.cbs)

    def rncb(self, result: Result, items: typing.List[datalayer.subscription.NotifyItem], userdata: datalayer.clib.userData_c_void_p):

        if result != Result.OK:
            # TODO Error
            self.out_error = True
            return

        if items is None:
            # TODO Warn
            self.out_error = True
            return

        if len(items) <= 0:
            # TODO Warn
            self.out_error = True
            return

        for item in items:
            '''
            print("  address:", item.get_address())
            print("  type:", item.get_data().get_type())
            print("  value:", item.get_data().get_float32())
            print("  timestamp:", item.get_timestamp())
            print("  datetime:", datalayer.subscription.to_datetime(
                item.get_timestamp()))
            '''

            if item.get_address() == self.in1_address.get_string():
                self.in1_value = item.get_data()

            if item.get_address() == self.in2_address.get_string():
                self.in2_value = item.get_data()

        self.calc()

        print("INFO", self.in1_value.get_float64(), self.mode.get_string(),
              self.in2_value.get_float64(), "=", self.out.get_float64())

    def calc(self):
        if self.in1_value.get_type() != VariantType.FLOAT64:
            self.out_error = True
            return

        if self.in2_value.get_type() != VariantType.FLOAT64:
            self.out_error = True
            return

        self.out_error = False

        mode = self.mode.get_string()

        if "+" in mode:
            self.out.set_float64(
                self.in1_value.get_float64() + self.in2_value.get_float64())
            return

        if "-" in mode:
            self.out.set_float64(
                self.in1_value.get_float64() - self.in2_value.get_float64())
            return

        if "*" in mode:
            self.out.set_float64(self.in1_value.get_float64()
                                 * self.in2_value.get_float64())
            return

        if "/" in mode:
            self.division()
            return

        if ":" in mode:
            self.division()
            return

        if "in1" in mode:
            self.out.set_float64(self.in1_value.get_float64())
            return

        if "in2" in mode:
            self.out.set_float64(self.in2_value.get_float64())
            return

        # Warning
        err = "UNSUPPORTED"
        if self.mode.get_string().startswith(err) == False:
            self.mode.set_string(err + " MODE: " + self.mode.get_string())
        self.out.set_float64(self.in1_value.get_float64())

    def division(self):
        try:
            self.out.set_float64(
                self.in1_value.get_float64() / self.in2_value.get_float64())
        except ZeroDivisionError:
            self.out_error = True

    def register_nodes(self):
        self.provider.register_node(
            self.addressRoot + "in1", self.providerNode)
        self.provider.register_node(
            self.addressRoot + "in2", self.providerNode)
        self.provider.register_node(
            self.addressRoot + "mode", self.providerNode)
        self.provider.register_node(
            self.addressRoot + "out", self.providerNode)

    def subscribe(self):
        subscription_properties = datalayer.subscription.create_properties(
            "python-datalayer-client-sub", publish_interval=100)

        if subscription_properties is None:
            return Result.FAILED

        addressList = []
        addressList.append(self.in1_address.get_string())
        addressList.append(self.in2_address.get_string())

        result, self.subscription = self.client.create_subscription_sync(
            subscription_properties, self.rncb)
        if result != Result.OK:
            return result

        if self.subscription is None:
            return Result.CREATION_FAILED

        return self.subscription.subscribe_multi(addressList)

    def unsubscribe(self):
        if self.subscription is not None:
            self.subscription.unsubscribe_all()

    def create_metadata(self, typeAddress: str, name: str, unit: str, description: str, allowWrite : bool):

        # Create `FlatBufferBuilder`instance. Initial Size 1024 bytes (grows automatically if needed)
        builder = flatbuffers.Builder(1024)

        # Serialize AllowedOperations data
        AllowedOperations.AllowedOperationsStart(builder)
        AllowedOperations.AllowedOperationsAddRead(builder, True)
        AllowedOperations.AllowedOperationsAddWrite(builder, allowWrite)
        AllowedOperations.AllowedOperationsAddCreate(builder, False)
        AllowedOperations.AllowedOperationsAddDelete(builder, False)
        operations = AllowedOperations.AllowedOperationsEnd(builder)

        # Metadata description strings
        descriptionBuilderString = builder.CreateString(description)
        urlBuilderString = builder.CreateString("tbd")
        displayNameString = builder.CreateString(name)
        unitString = builder.CreateString(unit)

        # Store string parameter into builder
        readTypeBuilderString = builder.CreateString("readType")
        writeTypeBuilderString = builder.CreateString("writeType")
        #createTypeBuilderString = builder.CreateString("createType")
        targetAddressBuilderString = builder.CreateString(typeAddress)

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

        # Serialize Reference data (for create operation)
        # Reference.ReferenceStart(builder)
        #Reference.ReferenceAddType(builder, createTypeBuilderString)
        #Reference.ReferenceAddTargetAddress(builder, targetAddressBuilderString)
        #reference_create = Reference.ReferenceEnd(builder)

        # Create FlatBuffer vector and prepend reference data. Note: Since we prepend the data, prepend them in reverse order.
        Metadata.MetadataStartReferencesVector(builder, 2)
        # builder.PrependSOffsetTRelative(reference_create)
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
        metadata = Metadata.MetadataEnd(builder)

        # Closing operation
        builder.Finish(metadata)
        return builder.Output()


    def __on_create(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        cb(Result.OK, None)

    def __on_remove(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # Not implemented because no wildcard is registered
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        new_data = Variant()
        new_data.set_array_string([])
        cb(Result.OK, new_data)

    def __on_read(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):

        if address.endswith("in1"):
            cb(Result.OK, self.in1_address)
            return

        if address.endswith("in2"):
            cb(Result.OK, self.in2_address)
            return

        if address.endswith("mode"):
            cb(Result.OK, self.mode)
            return

        if address.endswith("out"):
            if self.out_error:
                cb(Result.INVALID_VALUE, None)
                return

            if self.out.get_type() != VariantType.FLOAT64:
                cb(Result.NOT_INITIALIZED, None)
                return

            cb(Result.OK, self.out)
            return

        cb(Result.INVALID_ADDRESS, None)

    def __on_write(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):

        if data.get_type() != VariantType.STRING:
            cb(Result.TYPE_MISMATCH, None)
            return

        if address.endswith("in1"):
            self.in1_address.set_string(data.get_string())
            cb(Result.OK, data)
            return

        if address.endswith("in2"):
            self.in2_address.set_string(data.get_string())
            cb(Result.OK, data)
            return

        if address.endswith("mode"):
            self.mode.set_string(data.get_string())
            cb(Result.OK, data)
            return

        cb(Result.INVALID_ADDRESS, None)

    def __on_metadata(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        if address.endswith("in1"):
            cb(Result.OK, self.in1_metadata)
            return

        if address.endswith("in2"):
            cb(Result.OK, self.in2_metadata)
            return

        if address.endswith("mode"):
            cb(Result.OK, self.mode_metadata)
            return

        if address.endswith("out"):
            cb(Result.OK, self.out_metadata)
            return

        cb(Result.OK, None)

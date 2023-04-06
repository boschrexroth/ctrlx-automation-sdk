# MIT License
#
# Copyright (c) 2021-2022 Bosch Rexroth AG
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

from comm.datalayer import NodeClass

import ctrlxdatalayer
from ctrlxdatalayer.provider import Provider
from ctrlxdatalayer.client import Client
from ctrlxdatalayer.provider_node import ProviderNodeCallbacks, NodeCallback
from ctrlxdatalayer.variant import Result, Variant, VariantType

from calculations.convert import Convert

type_address_string = "types/datalayer/string"


class BasicArithmeticOperations:

    def __init__(self, provider: Provider, client: Client,  addressRoot: str, id: str, mode: str, update_time: int):

        self.provider = provider
        self.client = client

        self.addressRoot = addressRoot + "/" + id + "/"
       
        self.in1_address = Variant()
        self.in1_address.set_string(
            "framework/metrics/system/cpu-utilisation-percent")
        self.in1_value = Variant()
        self.in1_metadata = self.create_metadata(
            type_address_string, id, "", "Enter address of input value 1 here", True)

        self.in2_address = Variant()
        self.in2_address.set_string(
            "framework/metrics/system/cpu-utilisation-percent")
        self.in2_value = Variant()
        self.in2_metadata = self.create_metadata(
            type_address_string, id, "", "Enter address of input value 2 here", True)

        self.mode = Variant()
        self.mode.set_string(mode)
        self.mode_metadata = self.create_metadata(
            type_address_string, id, "", "Enter the operation mode here: + - / *", True)

        self.out = Variant()
        self.out_metadata = self.create_metadata(
            type_address_string, id, "", "The result of the operation", False)

        self.subscription_properties = ctrlxdatalayer.subscription.create_properties(
            "python-datalayer-client-sub", publish_interval=update_time)

        self.subscription_changed = False

        self.value_changed = False

        self.out_error = True
        self.out_error_text = ""

        self.cbs = ProviderNodeCallbacks(
            self.__on_create,
            self.__on_remove,
            self.__on_browse,
            self.__on_read,
            self.__on_write,
            self.__on_metadata
        )

        self.providerNode = ctrlxdatalayer.provider_node.ProviderNode(self.cbs)

    def response_notify_callback(self, result: Result, items: typing.List[ctrlxdatalayer.subscription.NotifyItem],
                                 userdata: ctrlxdatalayer.clib.userData_c_void_p):

        if result != Result.OK:
            print("response_notify_callback parameter result:", result, flush=True)
            self.out_error = True
            return

        for item in items:

            print()
            print("Notification -------------------------")
            print("address:", item.get_address())
            print("type:", item.get_data().get_type())
            print("value:", item.get_data().get_float64())
            print("timestamp:", item.get_timestamp(), flush=True)

            if item.get_address() == self.in1_address.get_string():
                self.in1_value.close()  # Avoid mem. leak
                # Clone item because variant is only valid in callback function
                print("--> in1")
                _, v = item.get_data().clone()
                self.in1_value = v
                self.value_changed = True

            if item.get_address() == self.in2_address.get_string():
                self.in2_value.close()  # Avoid mem. leak
                print("--> in2")
                # Clone item because variant is only valid in callback function
                _, v = item.get_data().clone()
                self.in2_value = v
                self.value_changed = True

    def get_float64_safe(self, v: Variant):

        if v.get_type() == VariantType.FLOAT64:
            return v.get_float64()

        if v.get_type() == VariantType.FLOAT32:
            self.float64_value.set_float64( v.get_float32())
            return self.float64_value.get_float64()

        if v.get_type() == VariantType.INT64:
            self.float64_value.set_float64( v.get_int64())
            return self.float64_value.get_float64()

        if v.get_type() == VariantType.INT32:
            self.float64_value.set_float64( v.get_int32())
            return self.float64_value.get_float64()

        if v.get_type() == VariantType.INT16:
            self.float64_value.set_float64( v.get_int16())
            return self.float64_value.get_float64()

        if v.get_type() == VariantType.INT8:
            self.float64_value.set_float64( v.get_int8())
            return self.float64_value.get_float64()

        return None

    def calc(self):

        self.calc_internal()

        print()
        print("Result ###################################")
        if self.out_error:
            print("ERROR", self.out_error_text, flush=True)
        else:
            print(self.in1_value.get_float64(), self.mode.get_string(),
                  self.in2_value.get_float64(), "=", self.out.get_float64(), flush=True)

    def calc_internal(self):

        self.out_error = False

        v1 = Convert.get_float64(self.in1_value)
        if v1 is None:
            self.out_error_text = "Converting in1 to float64 failed"
            self.out_error = True
            return
        self.in1_value.set_float64(v1)
        
        v2 = Convert.get_float64(self.in2_value)
        if v2 is None:
            self.out_error_text = "Converting in2 to float64 failed"
            self.out_error = True
            return
        
        self.in2_value.set_float64(v2)

        mode = self.mode.get_string()

        if "+" in mode:
            self.out.set_float64(v1 + v2)
            return

        if "-" in mode:
            self.out.set_float64(v1 - v2)
            return

        if "*" in mode:
            self.out.set_float64(v1 * v2)
            return

        if "/" in mode | ":" in mode:
            try:
                self.out.set_float64(v1 / v2)
            except ZeroDivisionError:
                self.out_error = True
                self.out_error_text = "ZeroDivisionError"
            return

        if "in1" in mode:
            self.out.set_float64(v1)
            return

        if "in2" in mode:
            self.out.set_float64(v2)
            return

        # Warning
        self.out_error_text = "Unsupported mode"
        self.out_error = True

    def register_node(self, name: str):
        address = self.addressRoot + name
        print("Registering node", address, flush=True)
        self.provider.register_node(
            address, self.providerNode)

    def register_nodes(self):
        result = self.register_node("in1")
        if result != ctrlxdatalayer.variant.Result.OK:
            return result
        
        result = self.register_node("in2")
        if result != ctrlxdatalayer.variant.Result.OK:
            return result
        
        result = self.register_node("mode")
        if result != ctrlxdatalayer.variant.Result.OK:
            return result
        
        return self.register_node("out")

    def subscribe(self):

        self.subscription_changed = False

        if self.subscription_properties is None:
            return Result.FAILED

        addressList = []

        address = self.in1_address.get_string()
        print("Subscribing", address, flush=True)
        addressList.append(address)

        address = self.in2_address.get_string()
        print("Subscribing", address, flush=True)
        addressList.append(address)

        result, self.subscription = self.client.create_subscription_sync(
            self.subscription_properties, self.response_notify_callback)
        if result != Result.OK:
            return result

        if self.subscription is None:
            return Result.CREATION_FAILED

        return self.subscription.subscribe_multi(addressList)

    def unsubscribe(self):
        if self.subscription is None:
            return Result.OK

        return self.subscription.unsubscribe_all()

    def create_metadata(self, typeAddress: str, name: str, unit: str, description: str, allowWrite: bool):

        return ctrlxdatalayer.metadata_utils.MetadataBuilder.create_metadata(
            name, description, unit, description+"_url", NodeClass.NodeClass.Variable,
            read_allowed=True, write_allowed=allowWrite, create_allowed=False, delete_allowed=False, browse_allowed=True,
            type_path="dummy")

    def __on_create(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        cb(Result.OK, None)

    def __on_remove(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # Not implemented because no wildcard is registered
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        new_data = Variant()
        new_data.set_array_string([])
        cb(Result.OK, new_data)

    def __on_read(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):

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

    def __on_write(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):

        if data.get_type() != VariantType.STRING:
            cb(Result.TYPE_MISMATCH, None)
            return

        if address.endswith("in1"):
            self.in1_address.set_string(data.get_string())
            self.subscription_changed = True
            cb(Result.OK, data)
            return

        if address.endswith("in2"):
            self.in2_address.set_string(data.get_string())
            self.subscription_changed = True
            cb(Result.OK, data)
            return

        if address.endswith("mode"):
            self.mode.set_string(data.get_string())
            cb(Result.OK, data)
            return

        cb(Result.INVALID_ADDRESS, None)

    def __on_metadata(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
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

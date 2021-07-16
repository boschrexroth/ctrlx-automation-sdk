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

from datalayerprovider.providerNodeCfgUint16 import ProviderNodeCfgUint16
import datalayer
from datalayer.provider import Provider
from datalayer.provider_node import ProviderNodeCallbacks, NodeCallback
from datalayer.variant import Result, Variant

from datalayerprovider.data_container import DataContainer


class ProviderNodeAllData:

    def __init__(self, provider: Provider, addressRoot: str, dynamic: bool):
        self.provider = provider

        self.addressRoot = addressRoot
        self.addressBase = ""
        self.dynamic = dynamic
        if dynamic:
            self.addressBase = addressRoot + "dynamic/"
        else:
            self.addressBase = addressRoot + "static/"

        self.providerNodeCfgUint16CopiesName = "_number-of-dynamic-value-sets"
        self.providerNodeCfgUint16Copies = ProviderNodeCfgUint16()
        self.copies = 1

        self.providerNodeCfgUint16NumName = "_number-of-dynamic-nodes"
        self.providerNodeCfgUint16Num = ProviderNodeCfgUint16()

        self.dataContainers = []
        self.cbs = ProviderNodeCallbacks(
            self.__on_create,
            self.__on_remove,
            self.__on_browse,
            self.__on_read,
            self.__on_write,
            self.__on_metadata
        )

        self.providerNode = datalayer.provider_node.ProviderNode(self.cbs)

    def createDataContainer(self, node: str, data: Variant):
        address = self.addressBase + node
        print("Creating", address)
        newDataContainer = DataContainer(address, data)
        self.dataContainers.append(newDataContainer)
        self.provider.register_node(address, self.providerNode)

    def getDataContainer(self, address: str):
        for dataContainer in self.dataContainers:
            if address == dataContainer.address:
                return dataContainer

        return None

    def registerNodesInternal(self, num: str):

        data = Variant()
        data.set_bool8(True)
        self.createDataContainer("bool8" + num, data)

        data = Variant()
        data.set_int8(-127)
        self.createDataContainer("int8" + num, data)

        data = Variant()
        data.set_uint8(255)
        self.createDataContainer("uint8" + num, data)

        data = Variant()
        data.set_int16(-32767)
        self.createDataContainer("int16" + num, data)

        data = Variant()
        data.set_uint16(65535)
        self.createDataContainer("uint16" + num, data)

        data = Variant()
        data.set_int32(0x80000001)
        self.createDataContainer("int32" + num, data)

        data = Variant()
        data.set_uint32(4294967294)
        self.createDataContainer("uint32" + num, data)

        data = Variant()
        data.set_int64(9223372036854775807)
        self.createDataContainer("int64" + num, data)

        data = Variant()
        data.set_uint64(9223372036854775807 * 2)
        self.createDataContainer("uint64" + num, data)

        data = Variant()
        data.set_float32(-0.123456789)
        self.createDataContainer("float32" + num, data)

        data = Variant()
        data.set_float64(-0.987654321)
        self.createDataContainer("float64" + num, data)

        data = Variant()
        data.set_string("This is string")
        self.createDataContainer("string" + num, data)

        # Flatbuffers

        data = Variant()
        data.set_string("This is string")
        self.createDataContainer("string" + num, data)

        data = Variant()
        data.set_array_bool8([False, True, False])
        self.createDataContainer("array-of-bool8" + num, data)

        data = Variant()
        data.set_array_int8([-127, -1, 0, 127])
        self.createDataContainer("array-of-int8" + num, data)

        data = Variant()
        data.set_array_uint8([0, 127, 128, 255])
        self.createDataContainer("array-of-uint8" + num, data)

        data = Variant()
        data.set_array_int16([-32767, -1, 0, 32767])
        self.createDataContainer("array-of-int16" + num, data)

        data = Variant()
        data.set_array_uint16([0, 32767, 32768, 65535])
        self.createDataContainer("array-of-uint16" + num, data)

        data = Variant()
        data.set_array_int32([-2147483647, -1, 0, 2147483647])
        self.createDataContainer("array-of-int32" + num, data)

        data = Variant()
        data.set_array_uint32([0, 2147483647, 2147483648, 4294967295])
        self.createDataContainer("array-of-uint32" + num, data)

        data = Variant()
        data.set_array_int64(
            [-9223372036854775807, -1, 0, 9223372036854775807])
        self.createDataContainer("array-of-int64" + num, data)

        data = Variant()
        data.set_array_uint64(
            [0, 9223372036854775807, 9223372036854775808, 18446744073709551615])
        self.createDataContainer("array-of-uint64" + num, data)

        data = Variant()
        data.set_array_float32([32.1, 32.2, 32.3, 32.4])
        self.createDataContainer("array-of-float32" + num, data)

        data = Variant()
        data.set_array_float64([64.1, 64.2, 64.3, 64.4])
        self.createDataContainer("array-of-float64" + num, data)

        data = Variant()
        data.set_array_string(["Red", "Green", "Yellow", "Blue"])
        self.createDataContainer("array-of-string" + num, data)

    def __on_create(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        # print("__on_create", address)
        cb(Result.OK, None)

    def __on_remove(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # Not implemented because no wildcard is registered
        print("__on_remove", address)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # print("__on_browse", address)
        with Variant() as new_data:
            new_data.set_array_string([])
            cb(Result.OK, new_data)

    def __on_read(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        # print("__on_read", address) - this command slows the performance down
        dataContainer = self.getDataContainer(address)
        if dataContainer is None:
            cb(Result.FAILED, None)
            return

        new_data = dataContainer.data
        cb(Result.OK, new_data)

    def __on_write(self, userdata: datalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        print("__on_write", address, data.get_type())

        if self.dynamic is False:
            cb(Result.PERMISSION_DENIED, None)
            return

        dataContainer = self.getDataContainer(address)
        if dataContainer is None:
            cb(Result.FAILED, None)
            return

        dataContainer.data = data
        cb(Result.OK, data)

    def __on_metadata(self, userdata: datalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # print("__on_metadata", address)
        cb(Result.OK, None)

    def CheckConfig(self):
        copies = self.providerNodeCfgUint16Copies.data.get_uint16()
        if copies <= self.copies:
            return

        n = copies
        while n > self.copies:
            self.registerNodesInternal("-" + str(n))
            n = n - 1

        self.copies = copies
        self.providerNodeCfgUint16Num.data.set_uint16(len(self.dataContainers))
        return

    def RegisterNodes(self):
        address = self.addressBase+"**"
        result = self.provider.register_node(address, self.providerNode)
        print("register_node", address, result)

        self.registerNodesInternal("")

        if self.dynamic is False:
            return

        # Add Config Nodes
        self.providerNodeCfgUint16Copies.data.set_uint16(1)
        address = self.addressRoot + self.providerNodeCfgUint16CopiesName
        result = self.provider.register_node(
            address, self.providerNodeCfgUint16Copies.providerNode)

        self.providerNodeCfgUint16Num.data.set_uint16(len(self.dataContainers))
        address = self.addressRoot + self.providerNodeCfgUint16NumName
        result = self.provider.register_node(
            address, self.providerNodeCfgUint16Num.providerNode)

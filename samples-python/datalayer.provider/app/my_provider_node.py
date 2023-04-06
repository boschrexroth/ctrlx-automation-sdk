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

import ctrlxdatalayer
from ctrlxdatalayer.provider import Provider
from ctrlxdatalayer.provider_node import ProviderNode, ProviderNodeCallbacks, NodeCallback
from ctrlxdatalayer.variant import Result, Variant


class MyProviderNode:

    def __init__(self,
                 provider: Provider,
                 address: str,
                 initialValue: Variant):

        self.cbs = ProviderNodeCallbacks(
            self.__on_create,
            self.__on_remove,
            self.__on_browse,
            self.__on_read,
            self.__on_write,
            self.__on_metadata
        )

        self.providerNode = ProviderNode(self.cbs)

        self.provider = provider
        self.address = address
        self.data = initialValue

    def register_node(self):
        return self.provider.register_node(self.address, self.providerNode)

    def unregister_node(self):
        self.provider.unregister_node(self.address)

    def set_value(self, value: Variant):
        self.data = value

    def __on_create(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        print("__on_create()", "address:", address, "userdata:", userdata, flush=True)
        cb(Result.OK, data)

    def __on_remove(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        print("__on_remove()", "address:", address, "userdata:", userdata, flush=True)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        print("__on_browse()", "address:", address, "userdata:", userdata, flush=True)
        new_data = Variant()
        new_data.set_array_string([])
        cb(Result.OK, new_data)

    def __on_read(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        print("__on_read()", "address:", address,
              "data:", self.data, "userdata:", userdata, flush=True)
        new_data = self.data
        cb(Result.OK, new_data)

    def __on_write(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        print("__on_write()", "address:", address,
              "data:", data, "userdata:", userdata, flush=True)

        if self.data.get_type() != data.get_type():
            cb(Result.TYPE_MISMATCH, None)
            return

        result, self.data = data.clone()
        cb(Result.OK, self.data)

    def __on_metadata(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        print("__on_metadata()", "address:", address, flush=True)
        cb(Result.FAILED, None)  # Take metadata from metadata.mddb

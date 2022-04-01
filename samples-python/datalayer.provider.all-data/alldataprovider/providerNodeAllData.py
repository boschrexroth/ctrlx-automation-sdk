# MIT License
#
# Copyright (c) 2020-2022 Bosch Rexroth AG
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

from comm.datalayer import NodeClass

import ctrlxdatalayer

from ctrlxdatalayer.metadata_utils import MetadataBuilder
from ctrlxdatalayer.provider import Provider
from ctrlxdatalayer.provider_node import ProviderNodeCallbacks, NodeCallback
from ctrlxdatalayer.variant import Result, Variant


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
        self.providerNode = ctrlxdatalayer.provider_node.ProviderNode(self.cbs)

        self.metadata = MetadataBuilder.create_metadata(
            name, description, unit, description + "-url", NodeClass.NodeClass.Variable,
            read_allowed=True, write_allowed=dynamic, create_allowed=dynamic, delete_allowed=dynamic, browse_allowed=True,
            type_path=addressType)

    def __on_create(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        # print("__on_create", address)
        cb(Result.OK, None)

    def __on_remove(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # Not implemented because no wildcard is registered
        print("__on_remove", address)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # print("__on_browse", address)
        cb(Result.OK, None)
        
    def __on_read(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        # print("__on_read", address) - this command slows the performance down
        new_data = self.data
        cb(Result.OK, new_data)

    def __on_write(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):

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

    def __on_metadata(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        # print("__on_metadata", address)
        cb(Result.OK, self.metadata)

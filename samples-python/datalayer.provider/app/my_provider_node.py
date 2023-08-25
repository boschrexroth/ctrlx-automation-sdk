# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import ctrlxdatalayer
from ctrlxdatalayer.provider import Provider
from ctrlxdatalayer.provider_node import ProviderNode, ProviderNodeCallbacks, NodeCallback
from ctrlxdatalayer.variant import Result, Variant


class MyProviderNode:
    """MyProviderNode
    """

    def __init__(self,
                 provider: Provider,
                 address: str,
                 initialValue: Variant):
        """__init__
        """
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
        """register_node
        """
        return self.provider.register_node(self.address, self.providerNode)

    def unregister_node(self):
        """unregister_node
        """
        self.provider.unregister_node(self.address)

    def set_value(self, value: Variant):
        """set_value
        """
        self.data = value

    def __on_create(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        """__on_create
        """
        print("__on_create()", "address:", address,
              "userdata:", userdata, flush=True)
        cb(Result.OK, data)

    def __on_remove(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        """__on_remove
        """
        print("__on_remove()", "address:", address,
              "userdata:", userdata, flush=True)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        """__on_browse
        """
        print("__on_browse()", "address:", address,
              "userdata:", userdata, flush=True)
        with Variant() as new_data:
            new_data.set_array_string([])
            cb(Result.OK, new_data)

    def __on_read(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        """__on_read
        """
        print("__on_read()", "address:", address,
              "data:", self.data, "userdata:", userdata, flush=True)
        new_data = self.data
        cb(Result.OK, new_data)

    def __on_write(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        """__on_write
        """
        print("__on_write()", "address:", address,
              "data:", data, "userdata:", userdata, flush=True)

        if self.data.get_type() != data.get_type():
            cb(Result.TYPE_MISMATCH, None)
            return

        result, self.data = data.clone()
        cb(Result.OK, self.data)

    def __on_metadata(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        """__on_metadata
        """
        print("__on_metadata()", "address:", address, flush=True)
        cb(Result.FAILED, None)  # Take metadata from metadata.mddb

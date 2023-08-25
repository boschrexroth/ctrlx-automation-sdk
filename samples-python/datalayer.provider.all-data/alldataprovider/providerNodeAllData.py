# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import ctrlxdatalayer
from comm.datalayer import NodeClass
from ctrlxdatalayer.metadata_utils import MetadataBuilder
from ctrlxdatalayer.provider import Provider
from ctrlxdatalayer.provider_node import NodeCallback, ProviderNodeCallbacks
from ctrlxdatalayer.variant import Result, Variant


class ProviderNodeAllData:
    """ProviderNodeAllData
    """

    def __init__(self,
                 provider: Provider,
                 addressType: str,
                 address: str,
                 name: str,
                 unit: str,
                 description: str,
                 dynamic: bool,
                 data: Variant):
        """__init__
        """
        self.provider = provider

        self.address = address
        self.name = name
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
        """__on_create
        """
        # print("__on_create", address, flush=True)
        cb(Result.OK, None)

    def __on_remove(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        """__on_remove
        """
        # Not implemented because no wildcard is registered
        print("__on_remove", address, flush=True)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        """__on_browse
        """
        # print("__on_browse", address, flush=True)
        cb(Result.OK, None)

    def __on_read(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        """__on_read
        """
        # print("__on_read", address, flush=True) - this command slows the performance down
        new_data = self.data
        cb(Result.OK, new_data)

    def __on_write(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, data: Variant, cb: NodeCallback):
        """__on_write
        """
        if self.dynamic is False:
            print("__on_write PERMISSION_DENIED",
                  address, data.get_type(), flush=True)
            cb(Result.PERMISSION_DENIED, None)
            return

        if self.data.get_type() != data.get_type():
            print("__on_write TYPE_MISMATCH", address,
                  data.get_type(), flush=True)
            cb(Result.TYPE_MISMATCH, None)
            return

        print("__on_write", address, data.get_type(), flush=True)
        _, self.data = data.clone()
        cb(Result.OK, data)

    def __on_metadata(self, userdata: ctrlxdatalayer.clib.userData_c_void_p, address: str, cb: NodeCallback):
        """__on_metadata
        """
        # print("__on_metadata", address, flush=True)
        cb(Result.OK, self.metadata)

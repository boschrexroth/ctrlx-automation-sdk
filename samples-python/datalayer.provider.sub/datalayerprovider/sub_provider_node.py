# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import threading
from threading import Timer

import ctrlxdatalayer
from comm.datalayer import NodeClass
from ctrlxdatalayer.metadata_utils import (AllowedOperation, MetadataBuilder,
                                           ReferenceType)
from ctrlxdatalayer.provider import Provider
from ctrlxdatalayer.provider_node import (NodeCallback, ProviderNode,
                                          ProviderNodeCallbacks)
from ctrlxdatalayer.provider_subscription import NotifyItemPublish
from ctrlxdatalayer.variant import Result, Variant


class ClientPublisher:
    """ClientPublisher
        Help class for multi client instances
    """

    @staticmethod
    def get_sub_id(sub: ctrlxdatalayer.provider_node.ProviderSubscription) -> str:
        """get_sub_id

        Args:
            sub (ctrlxdatalayer.provider_node.ProviderSubscription):

        Returns:
            str: id of the subscription
        """
        return sub.get_id()


    def __init__(self, sub: ctrlxdatalayer.provider_node.ProviderSubscription, lock: threading.Lock):
        """__init__

        Args:
            sub (ctrlxdatalayer.provider_node.ProviderSubscription): subscription instance
        """
        self.__timer = None
        self.__sub = sub
        self.__counter = 0
        self.__lock = lock


    def __publish(self):
        """__publish
        """
        while not self.__timer.finished.wait(self.__timer.interval):
            list_ni = []
            successfully_acquired = self.__lock.acquire(False)
            if not successfully_acquired:
                return
            for addr in self.__sub.get_notes():
                ni = NotifyItemPublish(addr)
                    #ni.get_notify_info().set_timestamp(sub.get_datetime()).set_notify_type(....)
                self.__counter = self.__counter + 1
                print(f"publish '{self.get_id()}': {self.__counter}")
                ni.get_data().set_int32(self.__counter)
                list_ni.append(ni)

            self.__lock.release()
            if len(list_ni) != 0:
                self.__sub.publish(Result.OK, list_ni)
            for ni in list_ni:
                ni.close()

    def get_id(self) -> str:
        """get_id
        
        simple
        
        Returns:
            str: subscription id
        """
        return ClientPublisher.get_sub_id(self.__sub)

    def start(self):
        """start
        """
        self.__timer = Timer(self.__sub.get_props().PublishInterval()/1000.0, self.__publish)
        self.__timer.start()
        print(f"start: '{self.get_id()}'")

    def stop(self):
        """stop
        """
        print(f"stop: '{self.get_id()}'")
        if self.__timer is None:
            return
        self.__timer.cancel()

class SubProviderFolderNode():
    """SubProviderFolderNode"""

    def __init__(self, provider: Provider, node_address: str,
                 value: Variant):
        """__init__"""
        self._cbs = ProviderNodeCallbacks(
            self.__on_create,
            self.__on_remove,
            self.__on_browse,
            self.__on_read,
            self.__on_write,
            self.__on_metadata,
            self.__on_subscribe,
            self.__on_unsubscribe
        )

        self._provider_node = ProviderNode(self._cbs)
        self._provider = provider
        self._node_address = node_address
        self._data = value
        self._metadata = self.create_metadata()
        self.__subs = dict([])
        self.__lock = threading.Lock()

    def create_metadata(self) -> Variant:
        """create_metadata

        Returns:
            Variant: metadata
        """
        builder = MetadataBuilder(allowed=AllowedOperation.READ
                                  | AllowedOperation.WRITE)
        builder = builder.set_display_name(self._node_address)
        builder = builder.set_node_class(NodeClass.NodeClass.Folder)
        #builder.add_reference(ReferenceType.read(), self._type_address)
        #builder.add_reference(ReferenceType.write(), self._type_address)
        return builder.build()

    def register_node(self):
        """register_node"""
        return self._provider.register_node(self._node_address,
                                            self._provider_node)

    def unregister_node(self):
        """unregister_node"""
        self._provider.unregister_node(self._node_address)
        self._metadata.close()
        self._data.close()

    def set_value(self, value: Variant):
        """set_value"""
        self._data = value

    def __on_create(
        self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        address: str,
        data: Variant,
        cb: NodeCallback,
    ):
        """__on_create"""
        print("__on_create()",
              "address:",
              address,
              "userdata:",
              userdata,
              flush=True)
        cb(Result.OK, data)

    def __on_remove(
        self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        address: str,
        cb: NodeCallback,
    ):
        """__on_remove"""
        print("__on_remove()",
              "address:",
              address,
              "userdata:",
              userdata,
              flush=True)
        cb(Result.UNSUPPORTED, None)

    def __on_browse(
        self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        address: str,
        cb: NodeCallback,
    ):
        """__on_browse"""
        print("__on_browse()",
              "address:",
              address,
              "userdata:",
              userdata,
              flush=True)
        with Variant() as new_data:
            new_data.set_array_string([])
            cb(Result.OK, new_data)

    def __on_read(
        self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        address: str,
        data: Variant,
        cb: NodeCallback,
    ):
        """__on_read"""
        print(
            "__on_read()",
            "address:",
            address,
            "data:",
            self._data.get_int32(),
            "userdata:",
            userdata,
            flush=True,
        )
        new_data = self._data
        cb(Result.OK, new_data)

    def __on_write(
        self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        address: str,
        data: Variant,
        cb: NodeCallback,
    ):
        """__on_write"""
        print(
            "__on_write()",
            "address:",
            address,
            "data:",
            data,
            "userdata:",
            userdata,
            flush=True,
        )

        if self._data.get_type() != data.get_type():
            cb(Result.TYPE_MISMATCH, None)
            return

        _, self._data = data.clone()
        cb(Result.OK, self._data)

    def __on_metadata(
        self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        address: str,
        cb: NodeCallback,
    ):
        """__on_metadata"""
        print("__on_metadata()", "address:", address, flush=True)
        cb(Result.OK, self._metadata)

    def __on_subscribe(self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        sub: ctrlxdatalayer.provider_node.ProviderSubscription,
        address: str):
        """__on_subscribe

        Args:
            userdata (ctrlxdatalayer.clib.userData_c_void_p):
            sub (ctrlxdatalayer.provider_node.ProviderSubscription):
            address (str):
        """
        print(f"""__on_subscribe
    address='{address}'
    datetime='{sub.get_timestamp()}""", flush=True)

        props = sub.get_props()
        print(f"""
    props:
        id={props.Id().decode('utf-8')};
        KeepaliveInterval={props.KeepaliveInterval()}
        PublishInterval={props.PublishInterval()}
                """, flush=True)

        notes = sub.get_notes()
        print(f"""
    notes: len={len(notes)}""", flush=True)

        for i, n in enumerate(notes):
            print(f"""          {i}: {n}""", flush=True)

        self.__lock.acquire()
        if not sub.get_id() in self.__subs:
            cp = ClientPublisher(sub, self.__lock)
            self.__subs[cp.get_id()] = cp
            self.__lock.release()
            cp.start()
            return
        self.__lock.release()


    def __on_unsubscribe(self,
        userdata: ctrlxdatalayer.clib.userData_c_void_p,
        sub: ctrlxdatalayer.provider_node.ProviderSubscription,
        address: str):
        """__on_unsubscribe

        Args:
            userdata (ctrlxdatalayer.clib.userData_c_void_p):
            sub (ctrlxdatalayer.provider_node.ProviderSubscription):
            address (str):
        """
        if len(sub.get_notes()) != 0:
            return
        self.__lock.acquire()
        if sub.get_id() in self.__subs:
            cp = self.__subs.pop(sub.get_id())
            cp.stop()
        self.__lock.release()

        print(f"on_unsubscribe: sub_id={sub.get_id()} len={len(self.__subs)}", address, flush=True)

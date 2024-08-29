#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import signal
import sys
import time

import ctrlxdatalayer
from ctrlxdatalayer.variant import Variant

from datalayerclient.app import (  # helper enviroment for debug.
    local_client_start, local_client_stop)
from datalayerprovider.sub_provider_node import SubProviderNode
from helper.ctrlx_datalayer_helper import get_provider, is_local_connection

# Binary sampleSchema file
__close_app = False


def handler(signum, frame):
    """handler"""
    global __close_app
    __close_app = True
    print('Here you go signum: ', signum, __close_app, flush=True)


def main():
    """main"""
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGABRT, handler)

    #connection_str = "ipc://"  #easier to debug as local runtime
    connection_str = "10.0.2.2"

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(is_local_connection(connection_str))

        # ip="10.0.2.2", ssl_port=8443: ctrlX COREvirtual
        # with port forwarding and default port mapping
        provider, connection_string = get_provider(datalayer_system,
                                                   ip=connection_str,
                                                   ssl_port=8443)
        if provider is None:
            print("ERROR Connecting", connection_string, "failed.", flush=True)
            sys.exit(1)

        with (
                provider
        ):  # provider.close() is called automatically when leaving with... block

            # Path to compiled files

            provider_node = provide_int(provider,
                                               "sdk/py/provider_sub/int",
                                               "types/datalayer/int")

            if is_local_connection(connection_str):
                local_client_start(datalayer_system)

            print("INFO Running endless loop...", flush=True)
            while provider.is_connected() and not __close_app:
                time.sleep(1.0)  # Seconds

            if is_local_connection(connection_str):
                local_client_stop()

            if provider.is_connected() is False:
                print("WARNING ctrlX Data Layer Provider is disconnected",
                    flush=True)

            provider_node.unregister_node()
            del provider_node


            print("Stopping ctrlX Data Layer provider:", end=" ", flush=True)
            result = provider.stop()
            print(result, flush=True)

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok, flush=True)

def provide_int(provider: ctrlxdatalayer.provider, node_address: str,
                   type_address: str):
    """provide_int

    Args:
        provider (ctrlxdatalayer.provider): 
        node_address (str): address of the node
        type_address (str): type of the node

    Returns:
        SubProviderNode: 
    """
    # Create and register simple string provider node
    print("Creating string  provider node " + node_address, flush=True)
    val = Variant()
    val.set_int32(1)
    return provide_node(provider, node_address, type_address, val)


def provide_node(provider: ctrlxdatalayer.provider, node_address: str,
                 type_address: str, value: Variant):
    """provide_node

    Args:
        provider (ctrlxdatalayer.provider): 
        node_address (str): address of the node
        type_address (str): type of the node
        value (Variant): value

    Returns:
        SubProviderNode: node
    """
    node = SubProviderNode(provider, node_address, type_address, value)
    result = node.register_node()
    if result != ctrlxdatalayer.variant.Result.OK:
        print(
            "ERROR Registering node " + node_address + " failed with:",
            result,
            flush=True,
        )

    return node


if __name__ == "__main__":
    main()

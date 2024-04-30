#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import os
import signal
import sys
import time

import ctrlxdatalayer
import flatbuffers
from ctrlxdatalayer.variant import Result, Variant

from app.my_provider_node import MyProviderNode
from helper.ctrlx_datalayer_helper import get_provider
from sample.schema.InertialValue import (
    InertialValueAddX,
    InertialValueAddY,
    InertialValueAddZ,
    InertialValueEnd,
    InertialValueStart,
)

# Binary sampleSchema file
bfbs_file = "sampleSchema.bfbs"

__close_app = False


def handler(signum, frame):
    """handler"""
    global __close_app
    __close_app = True
    # print('Here you go signum: ', signum, __close_app, flush=True)


def main():
    """main"""
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGABRT, handler)

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        # ip="10.0.2.2", ssl_port=8443: ctrlX COREvirtual with port forwarding and default port mapping
        provider, connection_string = get_provider(datalayer_system,
                                                   ip="10.0.2.2",
                                                   ssl_port=8443)
        if provider is None:
            print("ERROR Connecting", connection_string, "failed.", flush=True)
            sys.exit(1)

        with (
                provider
        ):  # provider.close() is called automatically when leaving with... block
            result = provider.start()
            if result != Result.OK:
                print(
                    "ERROR Starting ctrlX Data Layer Provider failed with:",
                    result,
                    flush=True,
                )
                return

            # Path to compiled files
            snap_path = os.getenv("SNAP")
            if snap_path is None:
                # Debug environment
                bfbs_path = os.path.join("./bfbs/", bfbs_file)
            else:
                # snap environment
                bfbs_path = os.path.join(snap_path, bfbs_file)

            # Register Flatbuffer type
            type_sampleSchema_inertialValue = "types/sdk/py/provider/inertial-value"
            result = provider.register_type(type_sampleSchema_inertialValue,
                                            bfbs_path)
            if result != Result.OK:
                print(
                    "WARNING Registering",
                    type_sampleSchema_inertialValue,
                    "failed with:",
                    result,
                    flush=True,
                )

            # Create nodes
            provider_node_fbs = provide_fbs(provider,
                                            "sdk/py/provider/inertial-value",
                                            type_sampleSchema_inertialValue)
            
            provider_node_str = provide_string(provider,
                                               "sdk/py/provider/string-value",
                                               "types/datalayer/string")

            print("INFO Running endless loop...", flush=True)
            while provider.is_connected() and not __close_app:
                time.sleep(1.0)  # Seconds

            print("ERROR ctrlX Data Layer Provider is disconnected",
                  flush=True)

            provider_node_fbs.unregister_node()
            del provider_node_fbs

            provider_node_str.unregister_node()
            del provider_node_str

            print("Unregistering",
                  type_sampleSchema_inertialValue,
                  end=" ",
                  flush=True)
            result = provider.unregister_type(type_sampleSchema_inertialValue)
            print(result, flush=True)

            print("Stopping ctrlX Data Layer provider:", end=" ", flush=True)
            result = provider.stop()
            print(result, flush=True)

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok, flush=True)


def provide_fbs(provider: ctrlxdatalayer.provider, nodeAddress: str,
                typeAddress: str):
    """provide_fbs"""
    # Create `FlatBufferBuilder`instance. Initial Size 1024 bytes (grows automatically if needed)
    builder = flatbuffers.Builder(1024)

    # Serialize InertialValue data (InertialValue.py auto generated from sampleSchema.fbs by flatc compiler)
    InertialValueStart(builder)
    InertialValueAddX(builder, 127)
    InertialValueAddY(builder, 128)
    InertialValueAddZ(builder, 129)
    inertialvalue = InertialValueEnd(builder)

    # Closing operation
    builder.Finish(inertialvalue)
    variantFlatbuffers = Variant()
    result = variantFlatbuffers.set_flatbuffers(builder.Output())
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR creating variant flatbuffers failed with:",
              result,
              flush=True)
        return

    # Create and register flatbuffers provider node
    print("Creating flatbuffers provider node " + nodeAddress, flush=True)
    return provide_node(provider, nodeAddress, typeAddress, variantFlatbuffers)


def provide_string(provider: ctrlxdatalayer.provider, nodeAddress: str,
                   typeAddress: str):
    """provide_string"""
    # Create and register simple string provider node
    print("Creating string  provider node " + nodeAddress, flush=True)
    variantString = Variant()
    variantString.set_string("myString")
    return provide_node(provider, nodeAddress, typeAddress, variantString)


def provide_node(provider: ctrlxdatalayer.provider, nodeAddress: str,
                 typeAddress: str, value: Variant):
    node = MyProviderNode(provider, nodeAddress, typeAddress,
                                       value)
    result = node.register_node()
    if result != ctrlxdatalayer.variant.Result.OK:
        print(
            "ERROR Registering node " + nodeAddress + " failed with:",
            result,
            flush=True,
        )

    return node


if __name__ == "__main__":
    main()

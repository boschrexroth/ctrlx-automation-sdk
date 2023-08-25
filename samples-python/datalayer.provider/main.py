#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import os
import sys
import time
import flatbuffers

import ctrlxdatalayer
from ctrlxdatalayer.variant import Variant, Result

from helper.ctrlx_datalayer_helper import get_provider

from app.my_provider_node import MyProviderNode

from sample.schema.InertialValue import InertialValueAddX
from sample.schema.InertialValue import InertialValueAddY
from sample.schema.InertialValue import InertialValueAddZ
from sample.schema.InertialValue import InertialValueEnd
from sample.schema.InertialValue import InertialValueStart

# Binary sampleSchema file
bfbs_file = "sampleSchema.bfbs"

# Binary metadata file
mddb_file = "metadata.mddb"

# addresses of provided values
address_base = "sdk-py-provider/"


def main():
    """main
    """
    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        # ip="10.0.2.2", ssl_port=8443: ctrlX COREvirtual with port forwarding and default port mapping
        provider, connection_string = get_provider(datalayer_system)
        if provider is None:
            print("ERROR Connecting", connection_string, "failed.", flush=True)
            sys.exit(1)

        with provider:  # provider.close() is called automatically when leaving with... block

            result = provider.start()
            if result != Result.OK:
                print("ERROR Starting ctrlX Data Layer Provider failed with:",
                      result, flush=True)
                return

            # Path to compiled files
            snap_path = os.getenv('SNAP')
            if snap_path is None:
                # Debug environment
                bfbs_path = os.path.join("./bfbs/", bfbs_file)
                mddb_path = os.path.join("./mddb/", mddb_file)

            else:
                # snap environment
                bfbs_path = os.path.join(snap_path, bfbs_file)
                mddb_path = os.path.join(snap_path, mddb_file)

            # Register Flatbuffer type
            # ATTENTION: Use same type as in csv file
            type_sampleSchema_inertialValue = "types/sampleSchema/inertialValue"
            result = provider.register_type(
                type_sampleSchema_inertialValue, bfbs_path)
            if result != Result.OK:
                print("WARNING Registering",
                      type_sampleSchema_inertialValue, "failed with:", result, flush=True)

            # Register Metadata Database
            result = provider.register_type("datalayer", mddb_path)
            if result != Result.OK:
                print("WARNING Registering",
                      mddb_path, "failed with:", result, flush=True)

            # Create nodes
            provider_node_fbs = provide_fbs(provider, "inertial-value")
            provider_node_str = provide_string(provider, "string-value")

            print("INFO Running endless loop...", flush=True)
            while provider.is_connected():
                time.sleep(1.0)  # Seconds

            print("ERROR ctrlX Data Layer Provider is disconnected", flush=True)

            provider_node_fbs.unregister_node()
            del provider_node_fbs

            provider_node_str.unregister_node()
            del provider_node_str

            print("Unregistering", type_sampleSchema_inertialValue,
                  end=" ", flush=True)
            result = provider.unregister_type(type_sampleSchema_inertialValue)
            print(result, flush=True)

            print("Stopping ctrlX Data Layer provider:", end=" ", flush=True)
            result = provider.stop()
            print(result, flush=True)

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok, flush=True)


def provide_fbs(provider: ctrlxdatalayer.provider, name: str):
    """provide_fbs
    """
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
        print("ERROR creating variant flatbuffers failed with:", result, flush=True)
        return

    # Create and register flatbuffers provider node
    print("Creating flatbuffers provider node " +
          address_base + name, flush=True)
    provider_node_fbs = MyProviderNode(
        provider, address_base + name,  variantFlatbuffers)
    result = provider_node_fbs.register_node()
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR Registering node " + address_base +
              name + " failed with:", result, flush=True)

    return provider_node_fbs


def provide_string(provider: ctrlxdatalayer.provider, name: str):
    """provide_string
    """
    # Create and register simple string provider node
    print("Creating string  provider node " + address_base + name, flush=True)
    variantString = Variant()
    variantString.set_string("myString")
    provider_node_str = MyProviderNode(
        provider, address_base + name, variantString)
    result = provider_node_str.register_node()
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR Registering node " + address_base +
              name + " failed with:", result, flush=True)

    return provider_node_str


if __name__ == '__main__':
    main()

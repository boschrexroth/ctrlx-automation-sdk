#!/usr/bin/env python3

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

import os
import sys
import time
import flatbuffers

import ctrlxdatalayer
from ctrlxdatalayer.variant import Variant

from helper.ctrlx_datalayer_helper import get_provider

from app.my_provider_node import MyProviderNode
from sample.schema.InertialValue import InertialValueAddX
from sample.schema.InertialValue import InertialValueAddY
from sample.schema.InertialValue import InertialValueAddZ
from sample.schema.InertialValue import InertialValueEnd
from sample.schema.InertialValue import InertialValueStart

# Relative path to the binary sampleSchema file
bfbs_file = "sampleSchema.bfbs"

# Type address of sampleSchema
type_address_fbs = "types/sdk-py-provider/schema/inertial-value"
type_address_string = "types/datalayer/string"

# addresses of provided values
address_base = "sdk-py-provider/"


def main():

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        # Try SSL port 8443
        provider, connection_string = get_provider(datalayer_system)
        if provider is None:
            print("ERROR Connecting", connection_string, "failed.")
            sys.exit(1)

        print("INFO Connecting", connection_string, "succeeded.")

        with provider:  # provider.close() is called automatically when leaving with block

            print("Starting provider")
            result = provider.start()
            if result is not ctrlxdatalayer.variant.Result.OK:
                print("ERROR Starting Data Layer Provider failed with:", result)
                return

            provider_node_fbs = provide_fbs(
                provider, "inertial-value", bfbs_file)
            provider_node_str = provide_string(provider, "string-value")

            print("Running endless loop...")
            while provider.is_connected():
                time.sleep(1.0)  # Seconds

            print("ERROR Data Layer Provider is disconnected")

            provider_node_fbs.unregister_node()
            del provider_node_fbs

            provider_node_str.unregister_node()
            del provider_node_str

            print("Unregistering provider Type", type_address_fbs, end=" ")
            result = provider.unregister_type(type_address_fbs)
            print(result)

            print("Stopping Data Layer Provider: ", end=" ")
            result = provider.stop()
            print(result)

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok)


def provide_fbs(provider: ctrlxdatalayer.provider, name: str, bfbs_file: str):
    # Path to sampleSchema.bfbs (Flatbuffers)
    snap_path = os.getenv('SNAP')
    if snap_path is None:
        # Debug environment
        bfbs_path = os.path.join("bfbs/", bfbs_file)
    else:
        # snap environment
        bfbs_path = os.path.join(snap_path, bfbs_file)

    # Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
    result = provider.register_type(type_address_fbs, bfbs_path)
    print("Registering Type with address=", bfbs_path, "result=", result)

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
        print("ERROR creating variant flatbuffers failed with:", result)
        return

    # Create and register flatbuffers provider node
    print("Creating flatbuffers provider node " + address_base + name)
    provider_node_fbs = MyProviderNode(
        provider,
        type_address_fbs,
        address_base + name,
        name,
        "g",
        "Simple flatbuffers variable",
        variantFlatbuffers)
    result = provider_node_fbs.register_node()
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR Registering node " + address_base +
              name + " failed with:", result)

    return provider_node_fbs


def provide_string(provider: ctrlxdatalayer.provider, name: str):
    # Create and register simple string provider node
    print("Creating string  provider node " + address_base + name)
    variantString = Variant()
    variantString.set_string("myString")
    provider_node_str = MyProviderNode(
        provider,
        type_address_string,
        address_base + name,
        name,
        "-",
        "String variable",
        variantString)
    result = provider_node_str.register_node()
    if result != ctrlxdatalayer.variant.Result.OK:
        print("ERROR Registering node " + address_base +
              name + " failed with:", result)

    return provider_node_str


if __name__ == '__main__':
    main()

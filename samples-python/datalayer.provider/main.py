#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2021 Bosch Rexroth AG
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
import time
import flatbuffers

import datalayer
from datalayer.variant import Variant

from app.my_provider_node import MyProviderNode
from sample.schema.InertialValue import InertialValue
from sample.schema.InertialValue import InertialValueAddX
from sample.schema.InertialValue import InertialValueAddY
from sample.schema.InertialValue import InertialValueAddZ
from sample.schema.InertialValue import InertialValueEnd
from sample.schema.InertialValue import InertialValueStart

# Relative path to the binary sampleSchema file
bfbs_file = "sampleSchema.bfbs"

# Type address of sampleSchema
type_address = "types/sample-py/schema/inertial-value"
# addresses of provided values
value_address_fbs = "my-py-nodes/my-fbs"
value_address_str = "my-py-nodes/my-string"


def main():

    with datalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        # This is the connection string for TCP in the format: tcp://USER:PASSWORD@IP_ADDRESS:PORT
        # Please check and change according your environment:
        # - USER:       Enter your user name here - default is boschrexroth
        # - PASSWORD:   Enter your password here - default is boschrexroth
        # - IP_ADDRESS: 127.0.0.1   If you develop in WSL and you want to connect to a ctrlX CORE virtual with port forwarding
        #               10.0.2.2    If you develop in a VM (Virtual Box, QEMU,...) and you want to connect to a ctrlX virtual with port forwarding
        #               192.168.1.1 If you are using a ctrlX CORE or ctrlX CORE virtual with TAP adpater

        connectionProvider = "tcp://boschrexroth:boschrexroth@127.0.0.1:2070"

        if 'SNAP' in os.environ:
            connectionProvider = "ipc://"

        print("Connecting", connectionProvider)
        with datalayer_system.factory().create_provider(connectionProvider) as provider:
            result = provider.start()
            if result is not datalayer.variant.Result.OK:
                print("ERROR Starting Data Layer Provider failed with:", result)
                return

            provider_node_fbs = provide_fbs(provider, bfbs_file)
            provider_node_str = provide_string(provider)

            print("Start provider")
            provider.start()
            print("Running endless loop...")
            while provider.is_connected():
                time.sleep(1.0)  # Seconds

            print("ERROR Data Layer Provider is disconnected")

            print("Stopping Data Layer Provider: ", end=" ")
            result = provider.stop()
            print(result)

            print("Unregister provider Type", type_address, end=" ")
            result = provider.unregister_type(type_address)
            print(result)

            print("Unregister provider Node", value_address_fbs, end=" ")
            result = provider.unregister_node(value_address_fbs)
            print(result)

            print("Unregister provider Node", value_address_str, end=" ")
            result = provider.unregister_node(value_address_str)
            print(result)

            del provider_node_fbs
            del provider_node_str

        datalayer_system.stop(True)


def provide_fbs(provider: datalayer.provider, bfbs_file: str):
    # Path to sampleSchema.bfbs (Flatbuffers)
    snap_path = os.getenv('SNAP')
    if snap_path is None:
        # Debug environment
        bfbs_path = os.path.join("bin/", bfbs_file)
    else:
        # snap environment
        bfbs_path = os.path.join(snap_path, bfbs_file)

    # Register type with binary flatbuffers schema file: sampleSchema.bfbs (auto generated from sampleSchema.fbs by flatc compiler)
    result = provider.register_type(type_address, bfbs_path)
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
    if result != datalayer.variant.Result.OK:
        print("ERROR creating variant flatbuffers failed with:", result)
        return

    # Create and register flatbuffers provider node
    print("Creating flatbuffers provider node")
    provider_node_fbs = MyProviderNode(
        provider, value_address_fbs, variantFlatbuffers)
    provider_node_fbs.create_metadata(type_address)
    provider_node_fbs.register_node()

    return provider_node_fbs


def provide_string(provider: datalayer.provider):
    # Create and register simple string provider node
    print("Creating string  provider node")
    variantString = Variant()
    variantString.set_string("myString")
    provider_node_str = MyProviderNode(
        provider, value_address_str, variantString)
    provider_node_str.create_metadata()
    provider_node_str.register_node()

    return provider_node_str


if __name__ == '__main__':
    main()

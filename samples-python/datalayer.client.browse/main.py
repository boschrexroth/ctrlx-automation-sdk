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
import sys
import time

import datalayer
import datalayer.clib

import flatbuffers
from comm.datalayer import Metadata
from comm.datalayer import AllowedOperations
from comm.datalayer import Reference
from datalayer.variant import Result, Variant, VariantType


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

        connectionClient = "tcp://boschrexroth:boschrexroth@127.0.0.1:2069"

        if 'SNAP' in os.environ:
            connectionClient = "ipc://"

        print("Connecting", connectionClient)
        with datalayer_system.factory().create_client(connectionClient) as datalayer_client:

            print("Data Layer Client created")

            # Check if client is connected
            print("Client connected:", datalayer_client.is_connected())

            # If not connected exit and retry with app daemon restart-delay (see snapcraft.yaml)
            if datalayer_client.is_connected() is False:

                print("Restarting app after restart-delay of 10 s ...")
                return

            # Browse the whole ctrlX Data Layer tree
            print("Browsing and reading nodes...")
            browse_tree(datalayer_client, datalayer_system.json_converter())

        print("datalayer_system.stop(..)",  datalayer_system.stop(True))


def browse_tree(client: datalayer.client.Client, converter: datalayer.system.Converter, address=""):

    # print current address and get value of node
    node_value = get_value(client, converter, address)
    if node_value is None:
        print(address)
    else:
        print(address, node_value)

    # Browse Data Layer tree
    result, data = client.browse_sync(address)
    if result != Result.OK:
        print("ERROR Browsing Data Layer failed with: ", result)
        return
    with data:
        # Recursive loop
        nodes = data.get_array_string()
        for node in nodes:
            browse_tree(client, converter, address + "/" + node)


def get_value(client: datalayer.client.Client, converter: datalayer.system.Converter, address: str):

    # get data with read sync
    result, data = client.read_sync(address)
    if result != Result.OK:
        # print("ERROR Reading Data Layer failed with: ", result)
        return
    with data:

        vt = data.get_type()

        if vt == VariantType.ARRAY_BOOL8:
            return data.get_array_bool8()

        if vt == VariantType.ARRAY_FLOAT32:
            return data.get_array_float32()

        if vt == VariantType.ARRAY_FLOAT64:
            return data.get_array_float64()

        if vt == VariantType.ARRAY_INT16:
            return data.get_array_int16()

        if vt == VariantType.ARRAY_INT32:
            return data.get_array_int32()

        if vt == VariantType.ARRAY_INT64:
            return data.get_array_int64()

        if vt == VariantType.ARRAY_INT8:
            return data.get_array_int8()

        if vt == VariantType.ARRAY_STRING:
            return data.get_array_string()

        if vt == VariantType.ARRAY_UINT16:
            return data.get_array_uint16()

        if vt == VariantType.ARRAY_UINT32:
            return data.get_array_uint32()

        if vt == VariantType.ARRAY_UINT64:
            return data.get_array_uint64()

        if vt == VariantType.ARRAY_UINT8:
            return data.get_array_uint8()

        if vt == VariantType.BOOL8:
            return data.get_bool8()

        if vt == VariantType.FLATBUFFERS:

            # Get type address for flatbuffers information
            typeAddress = get_typeaddress(client, address)
            if typeAddress is None:
                print("ERROR Type Address is none")
                return

            # Read type address as variant
            result, typeVar = client.read_sync(typeAddress)
            if result != Result.OK:
                print("ERROR Reading Type Value failed with: ", result)
                return

            # Convert variant flatbuffers data to json type
            result, json = converter.converter_generate_json_complex(data, typeVar,-1)
            if result != Result.OK:
                print("ERROR Converting json failed with: ", result)
                return

            return json.get_string()

        if vt == VariantType.FLOAT32:
            return data.get_float32()

        if vt == VariantType.FLOAT64:
            return data.get_float64()

        if vt == VariantType.INT16:
            return data.get_int16()

        if vt == VariantType.INT32:
            return data.get_int32()

        if vt == VariantType.INT64:
            return data.get_int64()

        if vt == VariantType.INT8:
            return data.get_int8()

        if vt == VariantType.STRING:
            return data.get_string()

        if vt == VariantType.UINT16:
            return data.get_uint16()

        if vt == VariantType.UINT32:
            return data.get_uint32()

        if vt == VariantType.UINT64:
            return data.get_uint64()

        if vt == VariantType.UINT8:
            return data.get_uint8()

        print("WARNING Unknow Variant Type:", vt)
        return None


def get_typeaddress(client: datalayer.client.Client, address: str):

    result, metadata = client.metadata_sync(address)
    if result != Result.OK:
        print("ERROR Reading metadata of ",address, " failed with: ", result)
        return

    metadata_root = Metadata.Metadata.GetRootAsMetadata(metadata.get_flatbuffers())

    if metadata_root.ReferencesLength() == 0:
        print("ERROR Metadata references are empty")
        return

    for i in range(0, metadata_root.ReferencesLength()):
        reference = metadata_root.References(i)

        if reference is None:
            continue

        if reference.Type().decode('utf-8').lower() == "readtype":
            read_typeaddress = reference.TargetAddress().decode('utf-8')
            break

    return read_typeaddress


if __name__ == '__main__':
    main()

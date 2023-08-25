#!/usr/bin/env python3

# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import sys

from comm.datalayer import Metadata

import ctrlxdatalayer
from ctrlxdatalayer.variant import Result, VariantType

from helper.ctrlx_datalayer_helper import get_client


def main():
    """main
    """
    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        datalayer_client, datalayer_client_connection_string = get_client(
            datalayer_system)
        if datalayer_client is None:
            print("ERROR Connecting", datalayer_client_connection_string, "failed.", flush=True)
            sys.exit(1)

        with datalayer_client:  # datalayer_client is closed automatically when leaving with block

            # If not connected exit and retry with app daemon restart-delay (see snapcraft.yaml)
            if datalayer_client.is_connected() is False:

                print("ERROR Not connected to", datalayer_client_connection_string,
                      "- restarting app after a delay of 10 s ...", flush=True)
                sys.exit(2)

            # Browse the whole ctrlX Data Layer tree
            print("Browsing and reading nodes...", flush=True)
            browse_tree(datalayer_client, datalayer_system.json_converter())

        print("Stopping ctrlX Data Layer System", flush=True)
        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        print("System Stop", stop_ok, flush=True)


def browse_tree(client: ctrlxdatalayer.client.Client, converter: ctrlxdatalayer.system.Converter, address=""):
    """browse_tree
    """
    # print current address and get value of node
    node_value = get_value(client, converter, address)
    if node_value is None:
        print(address, flush=True)
    else:
        print(address, node_value, flush=True)

    # Browse ctrlX Data Layer tree
    result, data = client.browse_sync(address)
    if result != Result.OK:
        print("ERROR Browsing ctrlX Data Layer failed with: ", result, flush=True)
        return
    with data:
        # Recursive loop
        nodes = data.get_array_string()
        for node in nodes:
            browse_tree(client, converter, address + "/" + node)


def get_value(client: ctrlxdatalayer.client.Client, converter: ctrlxdatalayer.system.Converter, address: str):
    """get_value
    """
    # get data with read sync
    result, data = client.read_sync(address)
    if result != Result.OK:
        # print("ERROR Reading ctrlX Data Layer failed with: ", result, flush=True)
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
                print("ERROR Type Address is none", flush=True)
                return

            # Read type address as variant
            result, typeVar = client.read_sync(typeAddress)
            if result != Result.OK:
                print("ERROR Reading Type Value failed with: ", result, flush=True)
                return

            # Convert variant flatbuffers data to json type
            result, json = converter.converter_generate_json_complex(
                data, typeVar, -1)
            if result != Result.OK:
                print("ERROR Converting json failed with: ", result, flush=True)
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

        print("WARNING Unknown Variant Type:", vt, flush=True)
        return None


def get_typeaddress(client: ctrlxdatalayer.client.Client, address: str):
    """get_typeaddress
    """
    result, metadata = client.metadata_sync(address)
    if result != Result.OK:
        print("ERROR Reading metadata of ", address, " failed with: ", result, flush=True)
        return

    metadata_root = Metadata.Metadata.GetRootAsMetadata(
        metadata.get_flatbuffers())

    if metadata_root.ReferencesLength() == 0:
        print("ERROR Metadata references are empty", flush=True)
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

# MIT License
#
# Copyright (c) 2020-2022 Bosch Rexroth AG
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

import logging

from comm.datalayer import Metadata

import ctrlxdatalayer
from ctrlxdatalayer.variant import (Result, VariantType)

import app.datalayer_helper

log = logging.getLogger()

global global_datalayer_system
global global_datalayer_client


def data_layer_start():

    global global_datalayer_system

    global_datalayer_system = ctrlxdatalayer.system.System("")
    global_datalayer_system.start(False)
    return global_datalayer_system

def data_layer_stop():

    global global_datalayer_system

    global_datalayer_system.stop(False)


def data_layer_client_connect(ip="192.168.1.1",
                              user="boschrexroth",
                              password="boschrexroth",
                              https_port=443):

    global global_datalayer_system
    global global_datalayer_client

    global_datalayer_client, connection_string = app.datalayer_helper.get_client(
        global_datalayer_system, ip, user, password, https_port)

    if global_datalayer_client is None:
        return None, "Connecting " + connection_string + " failed!"

    return global_datalayer_client, ""


def read_node(address: str):

    result, value = get_value(address)
    return result.name, value


def write_node(address: str, value: str):

    result = set_value(address, value)
    return result.name


def get_value(address):

    global global_datalayer_system
    global global_datalayer_client

    log.debug("Reading " + address)

    result, data = global_datalayer_client.read_sync(address)
    if result != Result.OK:
        log.error("ERROR Reading Data Layer failed with: %i", result)
        return result, data

    with data:

        vt = data.get_type()

        if vt == VariantType.ARRAY_BOOL8:
            return Result.OK, data.get_array_bool8()

        if vt == VariantType.ARRAY_FLOAT32:
            return Result.OK, data.get_array_float32()

        if vt == VariantType.ARRAY_FLOAT64:
            return Result.OK, data.get_array_float64()

        if vt == VariantType.ARRAY_INT16:
            return Result.OK, data.get_array_int16()

        if vt == VariantType.ARRAY_INT32:
            return Result.OK, data.get_array_int32()

        if vt == VariantType.ARRAY_INT64:
            return Result.OK, data.get_array_int64()

        if vt == VariantType.ARRAY_INT8:
            return Result.OK, data.get_array_int8()

        if vt == VariantType.ARRAY_STRING:
            return Result.OK, data.get_array_string()

        if vt == VariantType.ARRAY_UINT16:
            return Result.OK, data.get_array_uint16()

        if vt == VariantType.ARRAY_UINT32:
            return Result.OK, data.get_array_uint32()

        if vt == VariantType.ARRAY_UINT64:
            return Result.OK, data.get_array_uint64()

        if vt == VariantType.ARRAY_UINT8:
            return Result.OK, data.get_array_uint8()

        if vt == VariantType.BOOL8:
            return Result.OK, data.get_bool8()

        if vt == VariantType.FLATBUFFERS:

            # Get type address for flatbuffers information
            type_address = get_type_address(address)
            if type_address is None:
                log.error("ERROR Type Address is none")
                return Result.INVALID_CONFIGURATION, None

            # Read type address as variant
            result, bfbs = global_datalayer_client.read_sync(type_address)
            if result != Result.OK:
                log.error("ERROR Reading Type Value failed with: ", result)
                return Result.INVALID_CONFIGURATION, None

            # Convert variant flatbuffers data to json type
            converter = global_datalayer_system.json_converter()
            result, json = converter.converter_generate_json_complex(
                data, bfbs, -1)
            if result != Result.OK:
                log.error("ERROR Converting json failed with: ", result)
                return Result.INVALID_CONFIGURATION, None

            return Result.OK, json.get_string()

        if vt == VariantType.FLOAT32:
            return Result.OK, data.get_float32()

        if vt == VariantType.FLOAT64:
            return Result.OK, data.get_float64()

        if vt == VariantType.INT16:
            return Result.OK, data.get_int16()

        if vt == VariantType.INT32:
            return Result.OK, data.get_int32()

        if vt == VariantType.INT64:
            return Result.OK, data.get_int64()

        if vt == VariantType.INT8:
            return Result.OK, data.get_int8()

        if vt == VariantType.STRING:
            return Result.OK, data.get_string()

        if vt == VariantType.UINT16:
            return Result.Ok, data.get_uint16()

        if vt == VariantType.UINT32:
            return Result.Ok, data.get_uint32()

        if vt == VariantType.UINT64:
            return Result.OK, data.get_uint64()

        if vt == VariantType.UINT8:
            return Result.OK, data.get_uint8()

        log.warning("WARNING Supported Variant Type:", vt)
        return Result.UNSUPPORTED, None


def get_type_address(address: str):

    global global_datalayer_client

    log.debug("Reading metadata " + address)

    result, metadata = global_datalayer_client.metadata_sync(address)
    if result != Result.OK:
        log.error("ERROR Reading metadata of ",
                  address, " failed with: ", result)
        return

    metadata_root = Metadata.Metadata.GetRootAsMetadata(
        metadata.get_flatbuffers())

    if metadata_root.ReferencesLength() == 0:
        log.error("ERROR Metadata references are empty")
        return

    for i in range(0, metadata_root.ReferencesLength()):
        reference = metadata_root.References(i)

        if reference is None:
            continue

        if reference.Type().decode('utf-8').lower() == "readtype":
            type_address = reference.TargetAddress().decode('utf-8')
            return type_address

    return None


def set_value(address, value):

    global global_datalayer_client

    log.debug("Checking VarianType of " + address)

    # Read first to check VariantType
    result, variant = global_datalayer_client.read_sync(address)
    if result != Result.OK:
        return result
    
    with variant:

        vt = variant.get_type()

        if vt == VariantType.BOOL8:
            variant.set_bool8(bool(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.FLOAT32:
            variant.set_float32(float(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.FLOAT64:
            variant.set_float64(float(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.INT16:
            variant.set_int16(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.INT32:
            variant.set_int32(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.INT64:
            variant.set_int64(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.INT8:
            variant.set_int8(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.STRING:
            variant.set_string(value)
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.UINT16:
            variant.set_uint16(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.UINT32:
            variant.set_uint32(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.UINT64:
            variant.set_uint64(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        if vt == VariantType.UINT8:
            variant.set_uint8(int(value))
            result, _ = global_datalayer_client.write_sync(address, variant)
            return result

        log.error("ERROR Unsupported Variant Type:", vt)
        return Result.UNSUPPORTED

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


from comm.datalayer import Metadata

import ctrlxdatalayer
from ctrlxdatalayer.variant import Result, Variant, VariantType

from app.ctrlx_datalayer_helper import get_client

class DataLayer:

    def __init__(self, ip="192.168.1.1",
                 user="boschrexroth",
                 password="boschrexroth",
                 ssl_port=443) -> None:

        self.system = ctrlxdatalayer.system.System("")
        self.client = None

    def __enter__(self):
        """
        use the python context manager
        """
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """
        use the python context manager
        """
        self.system = None
        self.client = None

    def start(self):
        self.system.start(False)

    def stop(self):
        self.system.stop(False)

    def connect_client(self, ip="192.168.1.1",
                       user="boschrexroth",
                       password="boschrexroth",
                       https_port=443):

        self.client, connection_string = get_client(
            self.system, ip, user, password, https_port)

        return self.client, connection_string

    def read_node(self, address: str):

        result, value = self.get_value(address)
        return result.name, value

    def write_node(self, address: str, value: str):

        result = self.set_value(address, value)
        return result.name

    def get_value(self, address):

        print("Reading", address, flush=True)

        result, data = self.client.read_sync(address)
        if result != Result.OK:
            print("ERROR Reading Data Layer failed with:", result, flush=True)
            return result, data

        with data:

            variant_type = data.get_type()

            if variant_type == VariantType.ARRAY_BOOL8:
                return Result.OK, data.get_array_bool8()

            if variant_type == VariantType.ARRAY_FLOAT32:
                return Result.OK, data.get_array_float32()

            if variant_type == VariantType.ARRAY_FLOAT64:
                return Result.OK, data.get_array_float64()

            if variant_type == VariantType.ARRAY_INT16:
                return Result.OK, data.get_array_int16()

            if variant_type == VariantType.ARRAY_INT32:
                return Result.OK, data.get_array_int32()

            if variant_type == VariantType.ARRAY_INT64:
                return Result.OK, data.get_array_int64()

            if variant_type == VariantType.ARRAY_INT8:
                return Result.OK, data.get_array_int8()

            if variant_type == VariantType.ARRAY_STRING:
                return Result.OK, data.get_array_string()

            if variant_type == VariantType.ARRAY_UINT16:
                return Result.OK, data.get_array_uint16()

            if variant_type == VariantType.ARRAY_UINT32:
                return Result.OK, data.get_array_uint32()

            if variant_type == VariantType.ARRAY_UINT64:
                return Result.OK, data.get_array_uint64()

            if variant_type == VariantType.ARRAY_UINT8:
                return Result.OK, data.get_array_uint8()

            if variant_type == VariantType.BOOL8:
                return Result.OK, data.get_bool8()

            if variant_type == VariantType.FLATBUFFERS:
                return self.getFlatbuffersAsJsonStr(address, data)

            if variant_type == VariantType.FLOAT32:
                return Result.OK, data.get_float32()

            if variant_type == VariantType.FLOAT64:
                return Result.OK, data.get_float64()

            if variant_type == VariantType.INT16:
                return Result.OK, data.get_int16()

            if variant_type == VariantType.INT32:
                return Result.OK, data.get_int32()

            if variant_type == VariantType.INT64:
                return Result.OK, data.get_int64()

            if variant_type == VariantType.INT8:
                return Result.OK, data.get_int8()

            if variant_type == VariantType.STRING:
                return Result.OK, data.get_string()

            if variant_type == VariantType.UINT16:
                return Result.Ok, data.get_uint16()

            if variant_type == VariantType.UINT32:
                return Result.Ok, data.get_uint32()

            if variant_type == VariantType.UINT64:
                return Result.OK, data.get_uint64()

            if variant_type == VariantType.UINT8:
                return Result.OK, data.get_uint8()

            print(
                f"WARNING Supported Variant Type: {variant_type}", flush=True)
            return Result.UNSUPPORTED, None

    def getFlatbuffersAsJsonStr(self, addr: str, fbs_value: Variant):

        result, fbs_metadata = self.client.metadata_sync(addr)
        if result != Result.OK:
            return result, ""

        metadata_root = Metadata.Metadata.GetRootAsMetadata(
            fbs_metadata.get_flatbuffers())
        if metadata_root == None:
            return result, ""

        address_read_type = ""
        for i in range(0, metadata_root.ReferencesLength()):
            reference = metadata_root.References(i)

            if reference is None:
                continue

            if reference.Type().decode('utf-8').lower() == "readtype":
                address_read_type = reference.TargetAddress().decode('utf-8')
                break

        if address_read_type == "":
            return Result.INVALID_VALUE, ""

        result, fbs_type = self.client.read_sync(address_read_type)
        if result != Result.OK:
            return result, ""

        result, jsonVariant = self.system.json_converter(
        ).converter_generate_json_complex(fbs_value, fbs_type, 2)
        if result != Result.OK:
            return result, ""

        return Result.OK, jsonVariant.get_string()

    def set_value(self, address, value):

        print("Checking VarianType of", address, flush=True)

        # Read first to check VariantType
        result, variant = self.client.read_sync(address)
        if result != Result.OK:
            return result

        with variant:

            variant_type = variant.get_type()

            if variant_type == VariantType.BOOL8:
                variant.set_bool8(bool(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.FLOAT32:
                variant.set_float32(float(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.FLOAT64:
                variant.set_float64(float(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.INT16:
                variant.set_int16(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.INT32:
                variant.set_int32(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.INT64:
                variant.set_int64(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.INT8:
                variant.set_int8(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.STRING:
                variant.set_string(value)
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.UINT16:
                variant.set_uint16(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.UINT32:
                variant.set_uint32(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.UINT64:
                variant.set_uint64(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            if variant_type == VariantType.UINT8:
                variant.set_uint8(int(value))
                result, _ = self.client.write_sync(address, variant)
                return result

            print("ERROR Unsupported Variant Type:", variant_type, flush=True)
            return Result.UNSUPPORTED


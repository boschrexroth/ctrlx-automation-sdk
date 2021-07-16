# MIT License
#
# Copyright (c) 2021, Bosch Rexroth AG
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


def read_node(address:str):

    with datalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)
        
        # Check/change user:password@ip-address
        connectionClient = "tcp://boschrexroth:boschrexroth@127.0.0.1:2069"
        
        if 'SNAP' in os.environ:
            connectionClient = "ipc://"

        print("Connecting",connectionClient)
        with datalayer_system.factory().create_client(connectionClient) as datalayer_client:
            print("Data Layer Client created")

            # Check if client is connected
            for lp in range (5):
                if datalayer_client.is_connected():
                    print("Client connected")
                    result, value = get_value(datalayer_client,datalayer_system.json_converter(),address)
                    break                       
                else:
                    print("Client not connected. Retry ",lp, " of 5")
                    result = Result.CLIENT_NOT_CONNECTED
                    value  = ""
                    time.sleep(1) 
                    continue   
            datalayer_client.close()
            
        datalayer_system.stop(True) 
        print("datalayer_system.stop(..)")

        return result.name, value

def write_node(address:str, value:str):

    with datalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)
        
        # Check/change user:password@ip-address
        connectionClient = "tcp://boschrexroth:boschrexroth@127.0.0.1:2069"
        
        if 'SNAP' in os.environ:
            connectionClient = "ipc://"

        print("Connecting",connectionClient)
        with datalayer_system.factory().create_client(connectionClient) as datalayer_client:
            print("Data Layer Client created")

            # Check if client is connected
            for lp in range (5):
                if datalayer_client.is_connected():
                    print("Client connected")
                    result = set_value(datalayer_client,address,value)
                    break                       
                else:
                    print("Client not connected. Retry ",lp, " of 5")
                    result = Result.CLIENT_NOT_CONNECTED
                    time.sleep(1) 
                    continue    
            datalayer_client.close()
        
        datalayer_system.stop(True)
        print("datalayer_system.stop(..)")

        return result.name

def get_value(client: datalayer.client.Client,converter: datalayer.system.Converter, address):
    
    # get data with read sync
    result, data = client.read_sync(address)
    if result != Result.OK:
        # print("ERROR Reading Data Layer failed with: ", result)
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
            typeAddress = get_typeaddress(client, address)
            if typeAddress is None:
                print("ERROR Type Address is none")
                return Result.INVALID_CONFIGURATION, None

            # Read type address as variant
            result, typeVar = client.read_sync(typeAddress)
            if result != Result.OK:
                print("ERROR Reading Type Value failed with: ", result)
                return Result.INVALID_CONFIGURATION, None
            # Convert variant flatbuffers data to json type
            result, json = converter.converter_generate_json_complex(data,typeVar,-1)
            if result != Result.OK:
                print("ERROR Converting json failed with: ", result)
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

        print("WARNING Unknow Variant Type:", vt)
        return None

def get_typeaddress(client: datalayer.client.Client, address: str):

    result, metadata = client.metadata_sync(address)
    if result != Result.OK:
        print("ERROR Reading metadata of ",address," failed with: ", result)
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

def set_value(client: datalayer.client.Client, address, value):

    # get data with read sync
    result, data = client.read_sync(address)
    if result != Result.OK:
        return result
    with data:
    
        vt = data.get_type()

        if vt == VariantType.ARRAY_BOOL8:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_FLOAT32:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_FLOAT64:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_INT16:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_INT32:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_INT64:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_INT8:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_STRING:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_UINT16:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_UINT32:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_UINT64:
            return Result.UNSUPPORTED

        if vt == VariantType.ARRAY_UINT8:
            return Result.UNSUPPORTED

        if vt == VariantType.BOOL8:
            data.set_bool8(bool(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.FLATBUFFERS:
            return Result.UNSUPPORTED

        if vt == VariantType.FLOAT32:
            data.set_float32(float(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.FLOAT64:
            data.set_float64(float(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.INT16:
            data.set_int16(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.INT32:
            data.set_int32(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.INT64:
            data.set_int64(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.INT8:
            data.set_int8(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.STRING:
            data.set_string(value)
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.UINT16:
            data.set_uint16(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.UINT32:
            data.set_uint32(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.UINT64:
            data.set_uint64(int(value))
            result, var = client.write_sync(address,data)
            return result

        if vt == VariantType.UINT8:
            data.set_uint8(int(value))
            result, var = client.write_sync(address,data)
            return result

        print("WARNING Unknow Variant Type:", vt)
        return Result.INVALID_ADDRESS

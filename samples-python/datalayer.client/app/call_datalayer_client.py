# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import logging
import time

from comm.datalayer import Metadata

import ctrlxdatalayer
from ctrlxdatalayer.client import Client
from ctrlxdatalayer.variant import Result, Variant, VariantType

root_node = "sdk-cpp-alldata"


class CallDataLayerClient:
    """CallDataLayerClient
    """

    def __init__(self, client: Client) -> None:
        """__init__
        """
        self.client = client
        self.waiting_for = ""
        logging.basicConfig(format='%(asctime)s %(levelname)-8s %(message)s',
                            level=logging.INFO, datefmt='%H:%M:%S.%03d')

    def __enter__(self):
        """
        use the python context manager
        """
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """
        use the python context manager
        """
        self.client = None

    def run(self):
        """run
        """
        self.auth_token()
        self.ping()
        self.read()
        self.create()
        self.remove()
        self.browse()
        self.write()
        self.metadata()

    def wait_for_async_callback(self, result: Result):
        """wait_for_async_callback
        """
        if result != Result.OK:
            return

        count = 0
        while count < 5:
            if self.waiting_for is None:
                return

            if count > 0:
                logging.debug('Waiting for %s %ss', self.waiting_for)

            count = count + 1
            time.sleep(1.0)

        logging.error("%s TIME OUT", self.waiting_for)

    def log_result(self, msg: str, result: Result):
        """log_result
        """
        if result == Result.OK:
            logging.debug('%s --> %s', msg, result)
            return
        logging.error('%s failed with: %s', msg, result)

    def auth_token(self):
        """auth_token
        """
        logging.info("get_auth_token()")
        the_auth_token = self.client.get_auth_token()
        if the_auth_token is None:
            self.log_result('get_auth_token()', Result.FAILED)
            return

        logging.debug(the_auth_token)

        logging.info("set_auth_token()")
        self.client.set_auth_token(the_auth_token)

    def ping_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """ping_async_callback
        """
        self.waiting_for = None
        logging.info(">>>ping_async_callback: %s", result)

    def ping(self):
        """ping
        """
        self.waiting_for = "ping_async_callback"
        logging.info("ping_async()")
        result = self.client.ping_async(self.ping_async_callback, 105)
        self.log_result('ping_async()', result)
        self.wait_for_async_callback(result)

        logging.info("ping_sync()")
        result = self.client.ping_sync()
        self.log_result('ping_sync()', result)

    def read_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """read_async_callback
        """
        self.waiting_for = None
        self.log_result(">>>read_async_callback(): ", result)
        self.print_data(">>>read_async_callback(): ", result, "", data)

    def read_sync(self, node: str):
        """read_sync
        """
        addressBase = root_node + "/static/"
        address = addressBase + node

        logging.info("read_async() %s", address)
        result, data = self.client.read_sync(address)
        with data:
            self.log_result("read_sync() " + address, result)

            self.print_data("read_async()", result, address, data)

    def print_data(self, msg: str, result: Result, address: str, data: Variant):
        """print_data
        """
        if result != Result.OK:
            return

        print(msg, address, result, " ", end='', flush=True)

        if data.get_type() == VariantType.ARRAY_BOOL8:
            print(data.get_array_bool8(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_FLOAT32:
            print(data.get_array_float32(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_FLOAT64:
            print(data.get_array_float64(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_INT16:
            print(data.get_array_int16(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_INT32:
            print(data.get_array_int32(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_INT64:
            print(data.get_array_int64(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_INT8:
            print(data.get_array_int8(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_STRING:
            print(data.get_array_bool8(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_UINT16:
            print(data.get_array_uint16(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_UINT32:
            print(data.get_array_uint32(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_UINT64:
            print(data.get_array_uint64(), flush=True)
            return

        if data.get_type() == VariantType.ARRAY_UINT8:
            print(data.get_array_uint8(), flush=True)
            return

        if data.get_type() == VariantType.BOOL8:
            print(data.get_bool8(), flush=True)
            return

        if data.get_type() == VariantType.FLATBUFFERS:
            print(data.get_flatbuffers(), flush=True)
            return

        if data.get_type() == VariantType.FLOAT32:
            print(data.get_float32(), flush=True)
            return

        if data.get_type() == VariantType.FLOAT64:
            print(data.get_float64(), flush=True)
            return

        if data.get_type() == VariantType.INT16:
            print(data.get_int16(), flush=True)
            return

        if data.get_type() == VariantType.INT32:
            print(data.get_int32(), flush=True)
            return

        if data.get_type() == VariantType.INT64:
            print(data.get_int64(), flush=True)
            return

        if data.get_type() == VariantType.INT8:
            print(data.get_int8(), flush=True)
            return

        if data.get_type() == VariantType.STRING:
            print(data.get_string(), flush=True)
            return

        if data.get_type() == VariantType.UINT16:
            print(data.get_uint16(), flush=True)
            return

        if data.get_type() == VariantType.UINT32:
            print(data.get_uint32(), flush=True)
            return

        if data.get_type() == VariantType.UINT64:
            print(data.get_uint64(), flush=True)
            return

        if data.get_type() == VariantType.UINT8:
            print(data.get_uint8(), flush=True)
            return

        print("UNHANDLED ---------", flush=True)

    def read(self):
        """read
        """
        addressBase = root_node + "/static/"

        address = addressBase + "bool8"
        self.waiting_for = "read_async_callback"
        logging.info("read_async() %s", address)
        result = self.client.read_async(
            address, self.read_async_callback, 239)
        self.log_result("read_async() " + address, result)
        self.wait_for_async_callback(result)

        self.read_sync("bool8")

        self.read_sync("float32")

        self.read_sync("float64")

        self.read_sync("int8")

        self.read_sync("int16")

        self.read_sync("int32")

        self.read_sync("int64")

        self.read_sync("string")

        self.read_sync("uint8")

        self.read_sync("uint16")

        self.read_sync("uint32")

        self.read_sync("uint64")

        self.read_sync("array-of-bool8")

        self.read_sync("array-of-float32")

        self.read_sync("array-of-float64")

        self.read_sync("array-of-int8")

        self.read_sync("array-of-int16")

        self.read_sync("array-of-int32")

        self.read_sync("array-of-int64")

        self.read_sync("array-of-string")

        self.read_sync("array-of-uint8")

        self.read_sync("array-of-uint16")

        self.read_sync("array-of-uint32")

        self.read_sync("array-of-uint64")

    def create_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """create_async_callback
        """
        self.waiting_for = None
        logging.info(">>>create_async_callback(): %s %s %s",
                     result, data, userdata)

    def create_async(self, path, node, data: Variant):
        """create_async
        """
        address = path + node
        # Remove node so that create will succeed
        result = self.client.remove_sync(address)  # Ignore error

        self.waiting_for = "create_async_callback"
        logging.info("create_async() %s", address)
        result = self.client.create_async(
            address, data, self.create_async_callback, 122)
        self.log_result("create_async()", result)
        self.wait_for_async_callback(result)

    def create_sync(self, path, node, data: Variant):
        """create_sync
        """
        address = path + node
        # Remove node so that create will succeed
        result = self.client.remove_sync(address)  # Ignore error

        logging.info("create_sync() %s", address)
        result, dataReturned = self.client.create_sync(address, data)
        # !!! dataReturned is a reference on data (dataReturned==data)
        self.log_result("create_sync() " + address, result)

    def create(self):
        """create
        """
        with Variant() as data:

            addressBase = root_node + "/dynamic/_py/"

            data.set_bool8(True)
            self.create_sync(addressBase, "bool8", data)
            self.create_async(addressBase, "bool8", data)

            data.set_int8(-127)
            self.create_sync(addressBase, "int8", data)

            data.set_uint8(255)
            self.create_sync(addressBase, "uint8", data)

            data.set_int16(32767)
            self.create_sync(addressBase, "int16", data)

            data.set_uint16(65535)
            self.create_sync(addressBase, "uint16", data)

            data.set_int32(2147483647)
            self.create_sync(addressBase, "int32", data)

            data.set_uint32(4294967294)
            self.create_sync(addressBase, "uint32", data)

            data.set_int64(9223372036854775807)
            self.create_sync(addressBase, "int64", data)

            data.set_uint64(9223372036854775807 * 2)
            self.create_sync(addressBase, "uint64", data)

            data.set_float32(0.123456789)
            self.create_sync(addressBase, "float32", data)

            data.set_float64(0.987654321)
            self.create_sync(addressBase, "float64", data)

            data.set_string("This is string")
            self.create_sync(addressBase, "string", data)

            # Flatbuffers
            """
        def set_flatbuffers(self, data: bytearray) -> Result:
            buf = (ctypes.c_byte * len(data)).from_buffer(data)
            c_data = ctypes.cast(buf, ctypes.POINTER(ctypes.c_byte))
            return Result(libcomm_datalayer.DLR_variantSetFlatbuffers(self.c_variant, c_data, len(data)))
            """

            data.set_array_bool8([False, True, False])
            self.create_sync(addressBase, "array-of-bool8", data)

            data.set_array_int8([-127, -1, 0, 127])
            self.create_sync(addressBase, "array-of-int8", data)

            data.set_array_uint8([0, 127, 128, 255])
            self.create_sync(addressBase, "array-of-uint8", data)

            data.set_array_int16([-32767, -1, 0, 32767])
            self.create_sync(addressBase, "array-of-int16", data)

            data.set_array_uint16([0, 32767, 32768, 65535])
            self.create_sync(addressBase, "array-of-uint16", data)

            data.set_array_int32([-2147483647, -1, 0, 2147483647])
            self.create_sync(addressBase, "array-of-int32", data)

            data.set_array_uint32([0, 2147483647, 2147483648, 4294967295])
            self.create_sync(addressBase, "array-of-uint32", data)

            data.set_array_int64(
                [-9223372036854775807, -1, 0, 9223372036854775807])
            self.create_sync(addressBase, "array-of-int64", data)

            data.set_array_uint64(
                [0, 9223372036854775807, 9223372036854775808, 18446744073709551615])
            self.create_sync(addressBase, "array-of-uint64", data)

            data.set_array_float32([32.1, 32.2, 32.3, 32.4])
            self.create_sync(addressBase, "array-of-float32", data)

            data.set_array_float64([64.1, 64.2, 64.3, 64.4])
            self.create_sync(addressBase, "array-of-float64", data)

            data.set_array_string(["Red", "Green", "Yellow", "Blue"])
            self.create_sync(addressBase, "array-of-string", data)

    def remove_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """remove_async_callback
        """
        self.waiting_for = None
        logging.info(">>>create_async_callback(): %s %s", result, userdata)

    def remove(self):
        """remove
        """
        with Variant() as data:

            addressBase = root_node + "/dynamic/_py/"
            addressNode = "xxx"
            address = addressBase + addressNode
            data.set_string("Will be removed synch")
            self.client.create_sync(address, data)

            logging.info("remove_sync() %s", address)
            result = self.client.remove_sync(address)
            self.log_result("remove_sync()" + address, result)

            self.client.create_sync(address, data)

            logging.info("remove_async() %s", address)
            self.waiting_for = "remove_async_callback"
            result = self.client.remove_async(
                address, self.remove_async_callback, 243)
            self.log_result("remove_async()", result)
            self.wait_for_async_callback(result)

    def browse_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """browse_async_callback
        """
        self.waiting_for = None
        logging.info(">>>browse_async_callback: %s %s %s",
                     result, data.get_array_string(), userdata)

    def browse(self):
        """browse
        """
        logging.info("browse_sync() /")
        result, data = self.client.browse_sync("")
        with data:
            logging.info("browse_sync: %s %s", result, data.get_array_string())

            logging.info("browse_async() /")
            result = self.client.browse_async(
                "", self.browse_async_callback, 262)
            self.log_result("browse_async()", result)
            self.wait_for_async_callback(result)

    def write_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """write_async_callback
        """
        self.waiting_for = None
        self.log_result(">>>write_async_callback:", result)

    def write_sync(self, addressBase: str, node: str, data: Variant):
        """write_sync
        """
        address = addressBase + node
        logging.info("write_sync() %s", address)
        result, _ = self.client.write_sync(address, data)
        self.log_result("write_sync()", result)

    def write(self):
        """write
        """
        with Variant() as data:

            addressBase = root_node + "/dynamic/"

            address = addressBase + "bool8"
            data.set_bool8(True)
            self.waiting_for = "write_async_callback"
            logging.info("write_async() %s", address)
            result = self.client.write_async(
                address, data, self.write_async_callback, 475)
            self.log_result("write_async()", result)
            self.wait_for_async_callback(result)

            data.set_bool8(False)
            self.write_sync(addressBase, "bool8", data)

            data.set_float32(-0.123456789)
            self.write_sync(addressBase, "float32", data)

            data.set_float64(-0.987654321)
            self.write_sync(addressBase, "float64", data)

            data.set_int8(-127)
            self.write_sync(addressBase, "int8", data)

            data.set_int16(-32767)
            self.write_sync(addressBase, "int16", data)

            data.set_int32(0x80000001)
            self.write_sync(addressBase, "int32", data)

            data.set_int64(0x8000000000000001)
            self.write_sync(addressBase, "int64", data)

            data.set_string("Changed by python ctrlX Data Layer Client")
            self.write_sync(addressBase, "string", data)

    def print_metadata(self, text: str, result: Result, data: Variant):
        """print_metadata
        """
        if result != Result.OK:
            logging.error("%s failed with %s", text, result)
            return

        if data is None:
            logging.error("%s failed: data is None", text)
            return

        logging.info("%s %s", text, result)

        # Print Metadata (Flatbuffers)
        metadata = Metadata.Metadata.GetRootAsMetadata(
            data.get_flatbuffers(), 0)
        allowedoperations = metadata.Operations()
        print("metadata.NodeClass()", metadata.NodeClass(),
              " allowedOperations",
              "read=", allowedoperations.Read(),
              "write=", allowedoperations.Write(),
              "create=", allowedoperations.Create(),
              "delete=", allowedoperations.Delete(),
              "metadata.DisplayName()", metadata.DisplayName(),
              "metadata.DisplayFormat()", metadata.DisplayFormat(), flush=True)

    def metadata_async_callback(self, result: Result, data: Variant, userdata: ctrlxdatalayer.clib.userData_c_void_p):
        """metadata_async_callback
        """
        self.waiting_for = None
        self.print_metadata(">>>metadata_async_callback", result, data)

    def metadata(self):
        """metadata
        """
        address = root_node + "/dynamic/bool8"
        self.waiting_for = "metadata_async_callback"
        logging.info("metadata_async() %s", address)
        result = self.client.metadata_async(
            address, self.metadata_async_callback, 490)
        self.log_result("metadata_async()", result)
        self.wait_for_async_callback(result)

        logging.info("metadata_sync() %s", address)
        result, data = self.client.metadata_sync(address)
        with data:
            self.log_result("metadata_async()", result)
            self.print_metadata("metadata_sync() " + address, result, data)

        address = root_node + "/static/bool8"
        logging.info("metadata_sync() %s", address)
        result, data = self.client.metadata_sync(address)
        with data:
            self.log_result("metadata_async()", result)
            self.print_metadata("metadata_sync() " + address, result, data)

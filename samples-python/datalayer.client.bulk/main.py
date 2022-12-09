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
from typing import List
import logging
from systemd.journal import JournaldLogHandler

import ctrlxdatalayer
from ctrlxdatalayer.bulk import Response
from ctrlxdatalayer.variant import Result, Variant
from comm.datalayer import Metadata

from helper.ctrlx_datalayer_helper import get_client

log = logging.getLogger('sdk-py-client-bulk')
if 'SNAP' in os.environ:
    log.addHandler(JournaldLogHandler())
else:
    log.addHandler(logging.StreamHandler(sys.stdout))
log.setLevel(logging.DEBUG)

def cb_read_async(responses: List[Response], userdata: ctrlxdatalayer.clib.userData_c_void_p):
    for response in responses:
        if response.get_result() == Result.OK:
            dt = response.get_datetime()
            addr = response.get_address()
            data = response.get_data()
            log.debug('Bulk read: {a} {b} {c}'.format(a=dt, b=addr, c=data.get_float64()))


def main():

    log.debug("=================================================================")
    log.debug("sdk-py-client-bulk - A ctrlX Data Layer Client App in Python")
    log.debug("=================================================================")

    with ctrlxdatalayer.system.System("") as datalayer_system:
        datalayer_system.start(False)

        datalayer_client, datalayer_client_connection_string = get_client(
            datalayer_system, ip="10.0.2.2", ssl_port=8443)
        if datalayer_client is None:
            log.error("Connecting '{c}' failed".format(c=datalayer_client_connection_string))
            sys.exit(1)

        with datalayer_client:  # datalayer_client is closed automatically when leaving with block

            if datalayer_client.is_connected() is False:
                log.error("Data Layer is NOT connected:" + datalayer_client_connection_string)
                sys.exit(2)

            # Bulk read sync
            bulk = datalayer_client.create_bulk()
            with bulk:
                bulk.add('framework/metrics/system/cpu-utilisation-percent')
                bulk.add('framework/metrics/system/memavailable-mb')
                result = bulk.read()
                if result == Result.OK:
                    for response in bulk.get_response():
                        if response.get_result() == Result.OK:
                            dt = response.get_datetime()
                            addr = response.get_address()
                            data = response.get_data()
                            log.debug('Bulk read: {a} {b} {c}'.format(a=dt, b=addr, c=data.get_float64()))

            # Bulk write sync
            bulk = datalayer_client.create_bulk()
            with bulk:
                with Variant() as data:
                    data.set_bool8(False)
                    bulk.add('sdk-py-provider-alldata/dynamic/bool8', data)
                    bulk.add('sdk-cpp-alldata/dynamic/bool8', data)

                    data.set_float32(1.2345)
                    bulk.add('sdk-py-provider-alldata/dynamic/float32', data)
                    bulk.add('sdk-cpp-alldata/dynamic/float32', data)

                    data.set_int8(123)
                    bulk.add('sdk-py-provider-alldata/dynamic/int8', data)
                    bulk.add('sdk-cpp-alldata/dynamic/int8', data)

                    data.set_string('Changed by bulk write :-)')
                    bulk.add('sdk-py-provider-alldata/dynamic/string', data)
                    bulk.add('sdk-cpp-alldata/dynamic/string', data)

                    result = bulk.write()
                    if result == Result.OK:
                        for response in bulk.get_response():
                            log.debug('Bulk write: {a} {b}'.format(a=response.get_address(), b=response.get_result()))

            # Bulk browse sync
            bulk = datalayer_client.create_bulk()
            with bulk:
                bulk.add('framework/metrics/system')
                bulk.add('framework/metrics/process')
                result = bulk.browse()
                if result == Result.OK:
                    for response in bulk.get_response():
                        log.debug('Bulk browse: {a} {b}'.format(a=response.get_address(), b=response.get_result()))
                        if response.get_result() == Result.OK:
                            for node in response.get_data().get_array_string():
                                log.debug('\t{a}'.format(a=node))

            # Bulk metadata sync
            bulk = datalayer_client.create_bulk()
            with bulk:
                bulk.add('framework/metrics/system/cpu-utilisation-percent')    
                bulk.add('framework/metrics/system/memavailable-mb')
                result = bulk.metadata()
                if result == Result.OK:
                    for response in bulk.get_response():
                        log.debug('Bulk metadata: {a} {b}'.format(a=response.get_address(), b=response.get_result()))
                        if response.get_result() == Result.OK:
                            metadata = Metadata.Metadata.GetRootAsMetadata(
                                response.get_data().get_flatbuffers(), 0)
                            allowedoperations = metadata.Operations()
                            log.debug("\tallowedOperations read={a} write={b} create={c} delete={d}"
                                .format(a=allowedoperations.Read(),b=allowedoperations.Write(),c=allowedoperations.Create(),d=allowedoperations.Delete()))


            # Bulk create sync
            bulk = datalayer_client.create_bulk()
            with bulk:
                with Variant() as data:
                    data.set_int8(-127)
                    bulk.add('sdk-cpp-alldata/dynamic/py-created/int8', data)
                    data.set_string('Created by bulk create')
                    bulk.add('sdk-cpp-alldata/dynamic/py-created/string', data)
                    result = bulk.create()
                    if result == Result.OK:
                        for response in bulk.get_response():
                            log.debug('Bulk create: {a} {b}'.format(a=response.get_address(), b=response.get_result()))

            # Bulk delete sync
            bulk = datalayer_client.create_bulk()
            with bulk:
                with Variant() as data:
                    data.set_int8(-127)
                    bulk.add('sdk-cpp-alldata/dynamic/py-created/int8', data)
                    data.set_string('Created by bulk create')
                    bulk.add('sdk-cpp-alldata/dynamic/py-created/string', data)
                    result = bulk.delete()
                    if result == Result.OK:
                        for response in bulk.get_response():
                            log.debug('Bulk delete: {a} {b}'.format(a=response.get_address(), b=response.get_result()))

            log.debug("-----------------------------------------------------------------")
            log.debug("Running async operations")
            log.debug("-----------------------------------------------------------------")

            # Attention: Keep bulk instance during async operation
            bulk = datalayer_client.create_bulk()
            bulk.add('framework/metrics/system/cpu-utilisation-percent')
            bulk.add('framework/metrics/system/memavailable-mb')

            while datalayer_client.is_connected():

                # Bulk read async
                result = bulk.read(cb_read_async)

                time.sleep(5.0)

            log.error("Data Layer is NOT connected")

        # Attention: Doesn't return if any provider or client instance is still running
        stop_ok = datalayer_system.stop(False)
        log.debug("System Stop {a}".format(a=stop_ok))


if __name__ == '__main__':
    main()
